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

 /*
  * 
  *  13/12/2008 - checked to be synced with latest jpcsp change in revision 796
  */

#include "Common.h"
#include "Emulator.h"
#include "Memory.h"
#include "Loader.h"
#include "NIDMapper.h"
#include "Format/PBP.h"
#include "Format/PSF.h"
#include "Format/PSP.h"
#include "Format/Elf32.h"
#include "Format/Elf32Relocate.h"
#include "Format/Elf32StubHeader.h"
#include "Format/Elf32EntHeader.h"
#include "HLE/pspSysMem.h"
#include "HLE/Kernel/Managers/ModuleManager.h"

static bool loadedFirstModule;

// returns true on success
static bool loadPBP(ifstream &f, SceModule *module, u32 baseAddress) {
	PBP pbp(f);
	if (!pbp.isValid()) 
		return false;
	else {
		module->fileFormat |= FormatPBP;

		// dump PSF info
		if (pbp.getOffsetParam() > 0)
			module->setPsf(*pbp.readPSF(f));

		bool pbpUnpack;
		wxConfigEmu::get("Emulator", "UnpackPBP", &pbpUnpack);
		if(pbpUnpack)
			PBP::unpackPBP(f);

		// Setup position for chaining loaders
		f.seekg(pbp.getOffsetPspData());
		return true;
	}
}

// returns true on success
static bool loadSCE(ifstream &f, SceModule *module, u32 baseAddress) {
	u32 magic;
	f.read((char *)&magic, 4);
	if (magic != 0x4543537E)
		return false;
	else {
		module->fileFormat |= FormatSCE;
		Emulator::log.warn("Encrypted file not supported! (~SCE)");
		return true;
	}
}

// returns true on success
static bool loadPSP(ifstream &f, SceModule *module, u32 baseAddress) {
	PSP psp(f);
	if (!psp.isValid())
		return false;
	else {
		module->fileFormat |= FormatPSP;
		Emulator::log.warn("Encrypted file not supported! (PSP)");
		return true;
	}
}

#include "ElfLoader.h"

// returns true on success
static void loadUNK(ifstream &f, SceModule *module, u32 baseAddress) {
	u32 magic;
	f.read((char *)&magic, 4);
	Emulator::log.info(
		"Unrecognized file format (magic = 0x%08X)", magic);
}

static void loadPSF(SceModule *module) {
	if (module->getPsf() != NULL)
		return;

	// TODO: implement
}

void Loader::reset() {
	loadedFirstModule = false;
}

SceModule *Loader::loadModule(
	const char *pspFileName, ifstream &f, u32 baseAddress)
{
	auto_ptr<SceModule> module(new SceModule(false));

	// init context

	u32 currentOffset = f.tellg();
	f.seekg(0, ios::end);
	u32 size = f.tellg();
	f.seekg(currentOffset);

	module->fileFormat = FormatUnknown;
	module->pspFileName = pspFileName;

	// safety check
	if ((size - currentOffset) == 0) {
		Emulator::log.error("LoadModule: no data.");
		return NULL;
	}

	// chain loaders
	do {
		f.seekg(currentOffset);
		if (loadPBP(f, module.get(), baseAddress)) {
			currentOffset = f.tellg();
			if (currentOffset == size)
				break;
		}

		f.seekg(currentOffset);
		if (loadSCE(f, module.get(), baseAddress))
			break;

		f.seekg(currentOffset);
		if (loadPSP(f, module.get(), baseAddress))
			break;

		f.seekg(currentOffset);
		if (loadELF(f, module.get(), baseAddress, size))
			break;

		f.seekg(currentOffset);
		loadUNK(f, module.get(), baseAddress);
	} while (false);

	loadPSF(module.get());

	SceModule *ret = ModuleManager::getModuleByUid(module->modid);
	if (ret != NULL)
		module.release();

	return ret;
}
