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

// Included by Loader.cpp and shouldn't be included anywere else.

 /*
  * 
  *  13/12/2008 - checked to be synced with latest jpcsp change in revision 796
  */

static void loadELFProgram(
	ifstream &f, SceModule *module, u32 baseAddress,
	Elf32 &elf, u32 elfOffset, u32 fileSize)
{
	vector<Elf32ProgramHeader> &programHeaderList = elf.getProgramHeaderList();

	module->bss_size = 0;
	for (u32 i = 0; i < programHeaderList.size(); ++i) {
		Elf32ProgramHeader &phdr = programHeaderList[i];
		if (phdr.getType() != 1)
			continue;

		u32 fileOffset = phdr.getOffset();
		u32 memOffset  = baseAddress + phdr.getVAddr();
		u32 fileLen    = phdr.getFileSz();
		u32 memLen     = phdr.getMemSz();

		Memory::log.debug(
			"PH#%d: loading program 0x%08X - 0x%08X - 0x%08X",
			i, memOffset, memOffset + fileLen, memOffset + memLen);

		f.seekg(elfOffset + fileOffset);
		if (((u32)f.tellg() + fileLen) > fileSize) {
			u32 newLen = fileSize - (u32)f.tellg();
			Memory::log.warn(
				"PH#%d: program overflow clamping len 0x%08X to 0x%08X",
				i, fileLen, newLen);
			fileLen = newLen;
		}

		f.read((char *)Memory::getPointer(memOffset), fileLen);

		// update memory area consumed by the module
		if (memOffset < module->loadAddressLow) {
			module->loadAddressLow = memOffset;
			Memory::log.debug(
				"PH#%d: new loadAddressLow 0x%08X",i,
				module->loadAddressLow);
		}
		if ((memOffset + memLen) > module->loadAddressHigh) {
			module->loadAddressHigh = memOffset + memLen;
			Memory::log.debug(
				"PH#%d: new loadAddressHigh 0x%08X",i,
				module->loadAddressHigh);
		}

		module->bss_size += phdr.getMemSz() - phdr.getFileSz();
	}

	Memory::log.debug(
		"PH alloc consumption 0x%08X (mem 0x%08X)",
		module->loadAddressHigh - module->loadAddressLow,
		module->bss_size);
}

static void loadELFSections(
	ifstream &f, SceModule *module, u32 baseAddress,
	Elf32 &elf, u32 elfOffset)
{
	vector<Elf32SectionHeader> &sectionHeaderList = elf.getSectionHeaderList();

	for (u32 i = 0; i < sectionHeaderList.size(); ++i) {
		Elf32SectionHeader &shdr = sectionHeaderList[i];
		if (!(shdr.getFlags() & SHF_ALLOCATE))
			continue;

		if (shdr.getType() == SHT_PROGBITS) {
			// Load this section into memory
			// now loaded using program header type 1
			// u32 fileOffset = elfOffset + shdr.getOffset();
			u32 memOffset = baseAddress + shdr.getAddr();
			u32 len = shdr.getSize();
			/*
			Memory::log.debug(
				"%s: loading section 0x%08X - 0x%08X",
				shdr.getNamez(), memOffset, memOffset + len);
			f.seekg(fileOffset);
			f.read((char *)Memory::getPointer(memOffset), len);
			*/

			// Update memory area consumed by the module
			if ((baseAddress + shdr.getAddr()) < module->loadAddressLow) {
				Memory::log.warn(
					"%s: section allocates more than program 0x%08X - 0x%08X",
					shdr.getNamez().c_str(), memOffset, memOffset + len);
				module->loadAddressLow = baseAddress + shdr.getAddr();
			}
			if ((baseAddress + shdr.getAddr() + shdr.getSize()) > module->loadAddressHigh) {
				Memory::log.warn(
					"%s: section allocates more that program 0x%08X - 0x%08X",
					shdr.getNamez().c_str(), memOffset, memOffset + len);
				module->loadAddressHigh = baseAddress + shdr.getAddr() + shdr.getSize();
			}
		}
		else if (shdr.getType() == SHT_NOBITS) {
			// zero out this portion of memory
			u32 memOffset = baseAddress + shdr.getAddr();
			u32 len = shdr.getSize();

			if (len == 0) {
				Memory::log.debug(
					"%s: ignoring zero-length SHT_NOBITS type section 0x%08X",
					shdr.getNamez().c_str(), memOffset);
			}
			else if (memOffset >= Memory::MainMemoryStart &&
				(memOffset + len) < Memory::MainMemoryEnd)
			{
                Memory::log.debug("%s: clearing section %08X - %08X (len %08X)", shdr.getNamez().c_str(), memOffset, (memOffset + len), len);

				memset(Memory::getPointer(memOffset), 0, len);

				// update memory area consumed by the module
				if (memOffset < module->loadAddressLow) {
					module->loadAddressLow = memOffset;
					Memory::log.debug(
						"%s: new loadAddressLow 0x%08X (+0x%08X)",
						shdr.getNamez().c_str(), module->loadAddressLow, len);
				}
				if ((memOffset + len) > module->loadAddressHigh) {
					module->loadAddressHigh = memOffset + len;
					Memory::log.debug(
						"%s: new loadAddressHigh 0x%08X (+0x%08X)",
						shdr.getNamez().c_str(), module->loadAddressHigh, len);
				}
			}
			else {
				Memory::log.warn(
					"SHT_NOBITS section outside valid range 0x%08X - 0x%08X",
					memOffset, (memOffset + len));
			}
		}
	}

	// save the address/size of some sections for SceModule
	Elf32SectionHeader *shdr = elf.getSectionHeader(".text");
	if (shdr != NULL) {
		module->text_addr = baseAddress + shdr->getAddr();
		module->text_size = shdr->getSize();
	}

	shdr = elf.getSectionHeader(".data");
	if (shdr != NULL)
		module->text_size = shdr->getSize();

	shdr = elf.getSectionHeader(".bss");
	if (shdr != NULL && shdr->getSize() != 0)
		module->bss_size = shdr->getSize();
}

static void loadELFReserveMemory(SceModule *module) {
	// mark the are of memory the module loaded into as used
	Memory::log.debug(
		"Reserving %u bytes at 0%08X for module '%s'",
		module->loadAddressHigh - module->loadAddressLow,
		module->loadAddressLow, module->pspFileName.c_str());

	u32 addr = pspSysMem::malloc(
		2, PSP_SMEM_Addr,
		module->loadAddressHigh - module->loadAddressLow,
		module->loadAddressLow);
	if (addr != module->loadAddressLow) {
		Memory::log.warn(
			"Failed to properly reserve memory consumed by module %s " \
			"at address 0x%08X size 0x%08X new address 0x%08X",
			module->modname, module->loadAddressLow,
			module->loadAddressHigh - module->loadAddressLow, addr);
	}
	pspSysMem::addSysMemInfo(
		2, module->modname, PSP_SMEM_Low,
		module->loadAddressHigh - module->loadAddressLow,
		module->loadAddressLow); // NOTE: shouldn't it be addr???
}

static void loadELFModuleInfo(
	ifstream &f, SceModule *module, u32 baseAddress,
	Elf32 &elf, u32 elfOffset)
{
	Elf32ProgramHeader *phdr = elf.getProgramHeader(0);
	Elf32SectionHeader *shdr = elf.getSectionHeader(".rodata.sceModuleInfo");

	if (elf.getHeader().isPRXDetected()) {
		u32 memOffset = baseAddress + (phdr->getPAddr() & 0x7FFFFFFF);

		PSPModuleInfo moduleInfo;
		moduleInfo.read(memOffset);
		module->copy(moduleInfo);
	}
	else if (shdr != NULL) {
		u32 memOffset = baseAddress + shdr->getAddr();

		PSPModuleInfo moduleInfo;
		moduleInfo.read(memOffset);
		module->copy(moduleInfo);
	}
	else {
		Emulator::log.error("ModuleInfo not found!");
		return;
	}

	Emulator::log.info(
		"Found ModuleInfo name: '%s' version: 0x%04X attr: 0x%08X gp: 0x%08X",
		module->modname, module->version, module->attribute,module->gp_value);

	if (module->attribute & 0x1000)
		Emulator::log.warn("Kernel mode module detected");
	if (module->attribute & 0x0800)
		Emulator::log.warn("VSH mode module detected");
}

// loads from memory
static void loadELFImports(SceModule *module, u32 baseAddress, Elf32 &elf) {
	u32 stubHeadersAddress = module->stub_top;
	u32 stubHeadersCount = module->stub_size / Elf32StubHeader::sizeOf();

	// n modules to import, 1 stub header per module to import
	for (u32 i = 0; i < stubHeadersCount; ++i) {
		Elf32StubHeader stubHeader(stubHeadersAddress);

		char name[64];
		memcpy(name, Memory::getPointer(stubHeader.getOffsetModuleName()), 64);

		stubHeader.setModuleNamez(name);
		stubHeadersAddress += Elf32StubHeader::sizeOf();

		// n stubs per module to import
		for (u32 j = 0; j < stubHeader.getImports(); ++j) {
			u32 nid = Memory::read32(stubHeader.getOffsetNid() + j * 4);
			u32 importAddress = stubHeader.getOffsetText() + j * 8;
			DeferredStub deferredStub(
				stubHeader.getModuleNamez(), importAddress, nid);
			module->unresolvedImports.push_back(deferredStub);

			// add a 0xFFFFF syscall so we can detect
			// if an unresolved import is called
			u32 instruction = 0x03FFFFCC;
			Memory::write32(importAddress + 4, instruction);
		}
	}

	Emulator::log.info(
		"Found %d imports from %d modules",
		module->unresolvedImports.size(),
		stubHeadersCount);
}

// loads from memory
static void loadELFExports(SceModule *module, u32 baseAddress, Elf32 &elf) {
	u32 entHeadersAddress = module->ent_top;
	u32 entHeadersCount = module->ent_size / Elf32EntHeader::sizeOf();
	u32 entCount = 0;

	// n modules to export, 1 ent header per module to import
	for (u32 i = 0; i < entHeadersCount; ++i) {
		Elf32EntHeader entHeader(entHeadersAddress);
		string moduleName;

		if (entHeader.getOffsetModuleName() != 0) {
			char tmp[64];
			memcpy(tmp, Memory::getPointer(entHeader.getOffsetModuleName()), 64);
			moduleName = tmp;
		}
		else {
			// generate a module name
			moduleName = module->modname;
		}

		entHeader.setModuleNamez(moduleName);
		entHeadersAddress += Elf32EntHeader::sizeOf();

		//n ents per module to export
		u32 functionCount = entHeader.getFunctionCount();
		for (u32 j = 0; j < functionCount; ++j) {
			u32 nid = Memory::read32(entHeader.getOffsetResident() + j * 4);
			u32 exportAddress = Memory::read32(
				entHeader.getOffsetResident() + (j + functionCount) * 4);

			switch (nid) {
			// magic export nids from yapspd
			case 0xD3744BE0: // module_bootstart
			case 0x2F064FA6: // module_reboot_before
			case 0xADF12745: // module_reboot_phase
			case 0xD632ACDB: // module_start
			case 0xCEE8593C: // module_stop
			case 0xF01D73A7: // module_stop
			case 0x0F7C276C: // ?
				// ignore magic exports
				break;
			default:
				// save export
				NIDMapper::addModuleNid(moduleName, nid, exportAddress);
			}

			++entCount;
		}
	}

	if (entCount > 0)
		Emulator::log.info("Found %d exports", entCount);
}

static void loadELFDebuggerInfo(SceModule *module, u32 baseAddress, Elf32 &elf) {
	Elf32SectionHeader *shdr = elf.getSectionHeader(".init");
	if (shdr != NULL) {
		module->initSection[0] = baseAddress + shdr->getAddr();
		module->initSection[1] = shdr->getSize();
	}

	shdr = elf.getSectionHeader(".fini");
	if (shdr != NULL) {
		module->finiSection[0] = baseAddress + shdr->getAddr();
		module->finiSection[1] = shdr->getSize();
	}

	shdr = elf.getSectionHeader(".sceStub.text");
	if (shdr != NULL) {
		module->stubTextSection[0] = baseAddress + shdr->getAddr();
		module->stubTextSection[1] = shdr->getSize();
	}
}

static void relocateFromBuffer(
	ifstream &f, SceModule *module, u32 baseAddress,
	Elf32 &elf, u32 relCount)
{
// TODO: implement
}

// uses info from the elf program headres and
// elf section headers to relocate a PRX.
static void relocateFromHeaders(
	ifstream &f, SceModule *module, u32 baseAddress,
	Elf32 &elf, u32 elfOffset)
{
	// relocate from program headeres
	for (u32 i = 0; i < elf.getProgramHeaderList().size(); ++i) {
		Elf32ProgramHeader &phdr = elf.getProgramHeaderList()[i];
		if (phdr.getType() != 0x700000A0)
			continue;

		u32 relCount = phdr.getFileSz() / Elf32Relocate::sizeOf();
		Memory::log.debug("PH#%d: relocating %d entries", i, relCount);

		f.seekg(elfOffset + phdr.getOffset());
		relocateFromBuffer(f, module, baseAddress, elf, relCount);
		// TODO: now skip relocate from section headers?
		return;
	}

	// relocate from section headers
	for (u32 i = 0; i < elf.getSectionHeaderList().size(); ++i) {
		Elf32SectionHeader &shdr = elf.getSectionHeaderList()[i];
		if (shdr.getType() == SHT_REL) {
			Memory::log.warn(
				"%s: not relocating section",
				shdr.getNamez().c_str());
		}

		if (shdr.getType() == SHT_PRXREL) {
			u32 relCount = shdr.getSize() / Elf32Relocate::sizeOf();
			Memory::log.debug(
				"%s: relocating %d entries",
				shdr.getNamez().c_str(), relCount);

			f.seekg(elfOffset + shdr.getOffset());
			relocateFromBuffer(f, module, baseAddress, elf, relCount);
		}
	}
}

static void processUnresolvedImports() {
	u32 numberOfFailedNIDS = 0;
	u32 numberOfMappedNIDS = 0;

	map<u32, SceModule *>::iterator module = ModuleManager::values();
	while (module != ModuleManager::valuesEnd()) {
		module->second->importFixupAttempts++;
		for (list<DeferredStub>::iterator it =
			module->second->unresolvedImports.begin();
			it != module->second->unresolvedImports.end();)
		{
			string moduleName = it->getModuleName();
			u32 nid           = it->getNid();
			u32 importAddress = it->getImportAddress();

			// attempt to fixub stub to point to already loaded module export
			u32 exportAddress = NIDMapper::moduleNidToAddress(moduleName, nid);
			if (exportAddress != -1) {
				
				u32 instruction = // j <jumpAddress>
                        ((0x2/*J*/ & 0x3f) << 26)
                        | ((exportAddress >> 2) & 0x03ffffff);

				Memory::write32(importAddress, instruction);
				Memory::write32(importAddress + 4, 0);

				list<DeferredStub>::iterator tmp = it;
				tmp++;
				module->second->unresolvedImports.erase(it);
				it = tmp;
				numberOfMappedNIDS++;

				Emulator::log.debug(
					"Mapped import at 0x%08X to export at 0x%08X [0x%08X] (attempt %d)",
					importAddress, exportAddress, nid, module->second->importFixupAttempts);
			}
			else if (nid == 0) {
				Emulator::log.warn("Ignoring import at 0x%08X [0x%08X] (attempt %d)",
					importAddress, nid, module->second->importFixupAttempts);

				list<DeferredStub>::iterator tmp = it;
				tmp++;
				module->second->unresolvedImports.erase(it);
				it = tmp;
				Memory::write32(importAddress + 4, 0);
			}
			else {
				u32 code = NIDMapper::nidToSyscall(nid);
				if (code != -1) {
					u32 instruction =  // syscall <code>
                            ((0x0 & 0x3f) << 26)
                            | (0xc & 0x3f)
                            | ((code & 0x000fffff) << 6);
					Memory::write32(importAddress + 4, instruction);

					list<DeferredStub>::iterator tmp = it;
					tmp++;
					module->second->unresolvedImports.erase(it);
					it = tmp;
					numberOfMappedNIDS++;

					if (loadedFirstModule) {
						Emulator::log.debug(
							"Mapped import at 0x%08X to syscall 0x%05X [0x%08X] (attempt %d)",
							importAddress, code, nid, module->second->importFixupAttempts);
					}
				}
				else {
					Emulator::log.warn(
						"Failed to map import at 0x%08X [0x%08X] (attempt %d)",
						importAddress, nid, module->second->importFixupAttempts);
					numberOfFailedNIDS++;
					it++;
				}
			}
		}
		module++;
	}

	Emulator::log.info("%d NIDS mapped", numberOfMappedNIDS);
	if (numberOfFailedNIDS)
		Emulator::log.info("%d remaining unmapped NIDS", numberOfFailedNIDS);
}

static bool loadELF(ifstream &f, SceModule *module, u32 baseAddress, u32 fileSize) {
	u32 elfOffset = f.tellg();
	Elf32 elf(f);
	if (!elf.getHeader().isValid()) {
		Emulator::log.debug("Loader: not a ELF");
		return false;
	}
	else {
		module->fileFormat |= FormatELF;

		if (!elf.getHeader().isMIPSExecutable()) {
			Emulator::log.error("Loader: NOT a MIPS executable");
			return false;
		}

		if (elf.getHeader().isPRXDetected()) {
			Emulator::log.debug("Loader: Relocation required (PRX)");
			module->fileFormat |= FormatPRX;
		}
		else if (elf.getHeader().requiresRelocation()) {
			// seen in .elf's generated by pspsdk with BUILD_PRX = 1
			// before conversion to prx
			Emulator::log.info("Loader: Relocation required (ELF)");
		}
		else {
			// After the user chooses a game to run and we load it, then
			// we can't load another PBP at the same time. We can only load
			// relocatable modules (PRX's) after the user loaded app.
			if (baseAddress > 0x08900000) {
				Emulator::log.warn(
					"Loader: probably trying to load PBP ELF while" \
					"another PBP ELF is loaded");
			}

			baseAddress = 0;
		}

		module->baseAddress = baseAddress;
		module->entry_addr = baseAddress + elf.getHeader().getEntry();

		// Note: base address is 0 unless we are loading a PRX
		module->loadAddressLow = (baseAddress != 0) ? baseAddress : 0x08900000;
		module->loadAddressHigh = baseAddress;

		// load into mem
		loadELFProgram(f, module, baseAddress, elf, elfOffset, fileSize);
		loadELFSections(f, module, baseAddress, elf, elfOffset);
		loadELFReserveMemory(module);

		// relocate PRX
		if (elf.getHeader().requiresRelocation()) {
			relocateFromHeaders(f, module, baseAddress, elf, elfOffset);
		}

		// The following can only be done after relocation
		// Load .rodata.sceModuleInfo
		loadELFModuleInfo(f, module, baseAddress, elf, elfOffset);

		// save imports
		loadELFImports(module, baseAddress, elf);
		// save exports
		loadELFExports(module, baseAddress, elf);

		// try to fixup imports from all modules
		ModuleManager::addModule(module);
		processUnresolvedImports();

		// save some debugger stuff
		loadELFDebuggerInfo(module, baseAddress, elf);

		// flush module struct to psp mem
		module->write(module->address);

		loadedFirstModule = true;
		return true;
	}
}
