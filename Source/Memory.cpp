/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "Memory.h"

Log Memory::log;

u8 *Memory::scratchPad = NULL;
u8 *Memory::videoMemory = NULL;
u8 *Memory::mainMemory = NULL;
bool Memory::wasException;

static u8 *guardMemory = NULL;
static u8 **pageTable = NULL;

static bool guardEnabled = false;
static bool exceptionsEnabled = false;

static void makePageTable() {
	using namespace Memory;

	u32 page, i;

	for (i = 0; i < PageCount; ++i)
		pageTable[i] = guardMemory;

	page = ScratchPadStart >> PageShift;
	for (i = 0; i < (ScratchPadSize >> PageShift); ++i) {
		u8 *addr = scratchPad + (i << PageShift);
		pageTable[Mirror0 + page + i] = addr;
		pageTable[Mirror1 + page + i] = addr;
		pageTable[Mirror2 + page + i] = addr;
	}

	page = VideoMemoryStart >> PageShift;
	for (i = 0; i < (VideoMemorySize >> PageShift); ++i) {
		u8 *addr = videoMemory + (i << PageShift);
		pageTable[Mirror0 + page + i] = addr;
		pageTable[Mirror1 + page + i] = addr;
		pageTable[Mirror2 + page + i] = addr;
	}

	page = MainMemoryStart >> PageShift;
	for (i = 0; i < (MainMemorySize >> PageShift); ++i) {
		u8 *addr = mainMemory + (i << PageShift);
		pageTable[Mirror0 + page + i] = addr;
		pageTable[Mirror1 + page + i] = addr;
		pageTable[Mirror2 + page + i] = addr;
	}
}

static PVOID guardHandler = NULL;

static LONG WINAPI memoryGuard(EXCEPTION_POINTERS *ExceptionInfo) {
	using namespace Memory;

	if (exceptionsEnabled)
		return EXCEPTION_CONTINUE_SEARCH;

	if (ExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_SEARCH;

	uptr addr = ExceptionInfo->ExceptionRecord->ExceptionInformation[1];
	sptr tmp;

	// PageTable miss
	tmp = addr - (uptr)guardMemory;
	if (tmp >= 0 && tmp < (PageSize * 2))
		goto psp_err;

	// Invalid page cross in ScratchPad area
	tmp = addr - (uptr)(scratchPad + ScratchPadSize);
	if (tmp >= 0 && tmp < PageSize)
		goto psp_err;

	// Invalid page cross in VideoMemory area
	tmp = addr - (uptr)(videoMemory + VideoMemorySize);
	if (tmp >= 0 && tmp < PageSize)
		goto psp_err;

	// Invalid page cross in MainMemory area
	tmp = addr - (uptr)(mainMemory + MainMemorySize);
	if (tmp >= 0 && tmp < PageSize)
		goto psp_err;

	return EXCEPTION_CONTINUE_SEARCH;

psp_err:
	Error::clear();
	enableGuard(false);
	wasException = true;
	return EXCEPTION_CONTINUE_EXECUTION;
}

static void *VAlloc(u32 size) {
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static void translator(u32 u, EXCEPTION_POINTERS *exp) {
	if (exp->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		throw std::exception("Access violation");
}

bool Memory::initialize() {
	assert(scratchPad == NULL);
	log = Log("Memory");

	scratchPad = (u8 *)VAlloc(ScratchPadSize + PageSize);
	if (scratchPad == NULL) goto out_of_mem;

	videoMemory = (u8 *)VAlloc(VideoMemorySize + PageSize);
	if (videoMemory == NULL) goto out_of_mem;
	
	mainMemory = (u8 *)VAlloc(MainMemorySize + PageSize);
	if (mainMemory == NULL) goto out_of_mem;
	
	guardMemory = (u8 *)VAlloc(PageSize * 2);
	if (guardMemory == NULL) goto out_of_mem;

	pageTable = (u8 **)VAlloc(sizeof(u8 *) * PageCount);
	if (pageTable == NULL) goto out_of_mem;

	guardHandler = AddVectoredExceptionHandler(0, memoryGuard);
	if (guardHandler == NULL) {
		Memory::finalize();
		Error::set("Failed to add vectored exception handler!");
		return false;
	}

	if (!enableGuard(true)) {
		Memory::finalize();
		return false;
	}

	_set_se_translator(translator);

	exceptionsEnabled = true;
	wasException = false;

	makePageTable();
	return true;

out_of_mem:
	Memory::finalize();
	Error::set("Out of memory!");
	return false;
}

void Memory::finalize() {
	if (scratchPad) {
		VirtualFree(scratchPad, 0, MEM_RELEASE);
		scratchPad = NULL;
	}
	if (videoMemory) {
		VirtualFree(videoMemory, 0, MEM_RELEASE);
		videoMemory = NULL;
	}
	if (mainMemory) {
		VirtualFree(mainMemory, 0, MEM_RELEASE);
		mainMemory = NULL;
	}
	if (guardMemory) {
		VirtualFree(guardMemory, 0, MEM_RELEASE);
		guardMemory = NULL;
	}
	if (pageTable) {
		VirtualFree(pageTable, 0, MEM_RELEASE);
		pageTable = NULL;
	}
	if (guardHandler) {
		RemoveVectoredExceptionHandler(guardHandler);
		guardHandler = NULL;
	}
}

bool Memory::enableGuard(bool enable) {
	if (guardEnabled == enable) return true;

	DWORD dwOldProtect;
	BOOL status;

	status = VirtualProtect(
		guardMemory, PageSize * 2,
		enable ? PAGE_NOACCESS : PAGE_READWRITE,
		&dwOldProtect);
	if (status == FALSE)
		goto error;

	status = VirtualProtect(
		scratchPad + ScratchPadSize, PageSize,
		enable ? PAGE_NOACCESS : PAGE_READWRITE,
		&dwOldProtect);
	if (status == FALSE)
		goto error;

	status = VirtualProtect(
		videoMemory + VideoMemorySize, PageSize,
		enable ? PAGE_NOACCESS : PAGE_READWRITE,
		&dwOldProtect);
	if (status == FALSE)
		goto error;

	status = VirtualProtect(
		mainMemory + MainMemorySize, PageSize,
		enable ? PAGE_NOACCESS : PAGE_READWRITE,
		&dwOldProtect);
	if (status == FALSE)
		goto error;

	guardEnabled = enable;
	return true;

error:
	Error::set("Failed to change page protection status!");
	return false;
}

void Memory::enableExceptions(bool enable) {
	exceptionsEnabled = enable;
}

void Memory::reset() {
	assert(scratchPad != NULL);

	memset(scratchPad, 0, ScratchPadSize);
	memset(videoMemory, 0, VideoMemorySize);
	memset(mainMemory, 0, MainMemorySize);

	wasException = false;
}

u8 *Memory::getPointer(u32 addr) {
	return &(pageTable[addr >> PageShift][addr & PageMask]);
}

bool Memory::isValid(u32 addr) {
	return pageTable[addr >> PageShift] != guardMemory;
}

u8 Memory::read8(u32 addr) {
	return pageTable[addr >> PageShift][addr & PageMask];
}

u16 Memory::read16(u32 addr) {
	return *(u16 *)&(pageTable[addr >> PageShift][addr & PageMask]);
}

u32 Memory::read32(u32 addr) {
	return *(u32 *)&(pageTable[addr >> PageShift][addr & PageMask]);
}

void Memory::write8(u32 addr, u8 value) {
	pageTable[addr >> PageShift][addr & PageMask] = value;
}

void Memory::write16(u32 addr, u16 value) {
	*(u16 *)&(pageTable[addr >> PageShift][addr & PageMask]) = value;
}

void Memory::write32(u32 addr, u32 value) {
	*(u32 *)&(pageTable[addr >> PageShift][addr & PageMask]) = value;
}
