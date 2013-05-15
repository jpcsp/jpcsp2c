/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r688)
 */

#include "Common.h"
#include "Emulator.h"
#include "Elf32.h"

Elf32::Elf32()
	: shstrtab(NULL)
{
}

Elf32::Elf32(ifstream &f)
	: shstrtab(NULL)
{
	elfOffset = f.tellg();
	loadHeader(f);
	if (header.isValid()) {
		loadProgramHeaders(f);
		loadSectionHeaders(f);
	}
}

Elf32::~Elf32() {
}

void Elf32::loadHeader(ifstream &f) {
	header = Elf32Header(f);
	ElfInfo = header.toString();
}

void Elf32::loadProgramHeaders(ifstream &f) {	
	programHeaderList.clear();
	ProgInfo.clear();

	for (u16 i = 0; i < header.getPhNum(); ++i) {
		f.seekg(elfOffset + header.getPhOff() + (i * header.getPhEntSize()));
		Elf32ProgramHeader phdr(f);

		// save loaded header
		programHeaderList.push_back(phdr);

		// construct ELF program header info for debugger
		char tmp[128];
		sprintf(tmp, "-----PROGRAM HEADER #%02d-----\n", i);
		ProgInfo.append(tmp);
		ProgInfo.append(phdr.toString());

		// yapspd: If the PRX file is a kernel module then the most significant
		// bit must be set in the physical address of the first program header.
		if (i == 0 && (phdr.getPAddr() & 0x80000000) == 0x80000000)
			Emulator::log.debug("Kernel mode PRXdetected");
	}
}

void Elf32::loadSectionHeaders(ifstream &f) {
	sectionHeaderList.clear();
	sectionHeaderMap.clear();

	// 1st pass
	// - save headers
	// - find .shstrtab
	for (u16 i = 0; i < header.getShNum(); ++i) {
		f.seekg(elfOffset + header.getShOff() + (i * header.getShEntSize()));
		Elf32SectionHeader shdr(f);

		// save loaded header
		sectionHeaderList.push_back(shdr);

		// find the .shstrtab section
		if (shdr.getType() == SHT_STRTAB && // 0x00000003
			shstrtab == NULL &&
			// Some programs have 2 STRTAB headers,
			// the header with size 1 has to be ignored.
			shdr.getSize() > 1)
		{
			shstrtab = &sectionHeaderList[i];
		}
	}

	if (shstrtab == NULL) {
		Emulator::log.warn(".shstrtab section not found");
		return;
	}

	// 2nd pass
	// - Construct ELF section header info for debugger
	for (size_t i = 0; i < sectionHeaderList.size(); ++i) {
		Elf32SectionHeader &shdr = sectionHeaderList[i];
		f.seekg(elfOffset + shstrtab->getOffset() + shdr.getName());

		// number the section
		char tmp[128];
		sprintf(tmp, "-----SECTION HEADER #%02d-----\n", i);
		SectInfo.append(tmp);

		string SectionName;
		char c;
		f.read(&c, 1);
		while (c != 0) {
			SectionName.append(&c, 0, 1);
			f.read(&c, 1);
		}

		if (SectionName.length() > 0) {
			shdr.setNamez(SectionName);
			SectInfo.append(SectionName);
			SectInfo.append("\n");
			sectionHeaderMap[SectionName] = shdr;
		}

		// add this section header's info
		SectInfo.append(shdr.toString());
	}
}

u32 Elf32::getElfOffset() const {
	return elfOffset;
}

Elf32Header &Elf32::getHeader() {
	return header;
}

vector<Elf32ProgramHeader> &Elf32::getProgramHeaderList() {
	return programHeaderList;
}

Elf32ProgramHeader *Elf32::getProgramHeader(u32 index) {
	if (index >= programHeaderList.size())
		return NULL;
	else
		return &programHeaderList[index];
}

vector<Elf32SectionHeader> &Elf32::getSectionHeaderList() {
	return sectionHeaderList;
}

Elf32SectionHeader *Elf32::getSectionHeader(u32 index) {
	if (index >= sectionHeaderList.size())
		return NULL;
	else
		return &sectionHeaderList[index];
}

Elf32SectionHeader *Elf32::getSectionHeader(const string &name) {
	map<string, Elf32SectionHeader>::iterator it =
		sectionHeaderMap.find(name);
	if (it == sectionHeaderMap.end())
		return NULL;
	else
		return &it->second;
}

string Elf32::getElfInfo() const {
	return ElfInfo;
}

string Elf32::getProgInfo() const {
	return ProgInfo;
}

string Elf32::getSectInfo() const {
	return SectInfo;
}
