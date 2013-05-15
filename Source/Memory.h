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

#pragma once

namespace Memory {
	bool initialize();
	void finalize();
	void reset();

	bool enableGuard(bool enable);
	void enableExceptions(bool enable);

	u8 *getPointer(u32 addr);
	bool isValid(u32 addr);

	u8  read8 (u32 addr);
	u16 read16(u32 addr);
	u32 read32(u32 addr);

	void write8 (u32 addr, u8  value);
	void write16(u32 addr, u16 value);
	void write32(u32 addr, u32 value);

	extern Log log;

	extern u8 *scratchPad;
	extern u8 *videoMemory;
	extern u8 *mainMemory;

	extern bool wasException;
	
	const u32 ScratchPadStart = 0x00010000;
	const u32 ScratchPadSize  = 0x00004000;
	const u32 ScratchPadEnd   = ScratchPadStart - ScratchPadSize - 1;

	const u32 VideoMemoryStart = 0x04000000;
	const u32 VideoMemorySize  = 0x00200000;
	const u32 VideoMemoryEnd   = VideoMemoryStart + VideoMemorySize - 1;

	const u32 MainMemoryStart  = 0x08000000;
	const u32 MainMemorySize   = 0x02000000;
	const u32 MainMemoryEnd    = MainMemoryStart + MainMemorySize - 1;

	const u32 PageShift = 12;
	const u32 PageSize = 1 << PageShift;
	const u32 PageMask = PageSize - 1;
	const u32 PageCount = 1 << (32 - PageShift);

	const u32 Mirror0 = 0x00000;
	const u32 Mirror1 = 0x40000;
	const u32 Mirror2 = 0x80000;
};
