/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r368)
 */


#include "Common.h"
#include "Elf32ProgramHeader.h"

Elf32ProgramHeader::Elf32ProgramHeader() {
}

Elf32ProgramHeader::Elf32ProgramHeader(ifstream &f) {
	read(f);
}

Elf32ProgramHeader::~Elf32ProgramHeader() {
}

void Elf32ProgramHeader::read(ifstream &f) {
	f.read((char *)&data, sizeof(data));
}

const size_t Elf32ProgramHeader::sizeOf() {
	return 32;
}

string Elf32ProgramHeader::toString() const {
	string str;
	char tmp[128];

	sprintf(tmp, "p_type   0x%08X\n", data.p_type  ); str.append(tmp);
	sprintf(tmp, "p_offset 0x%08X\n", data.p_offset); str.append(tmp);
	sprintf(tmp, "p_vaddr  0x%08X\n", data.p_vaddr ); str.append(tmp);
	sprintf(tmp, "p_paddr  0x%08X\n", data.p_paddr ); str.append(tmp);
	sprintf(tmp, "p_filesz 0x%08X\n", data.p_filesz); str.append(tmp);
	sprintf(tmp, "p_memsz  0x%08X\n", data.p_memsz ); str.append(tmp);
	sprintf(tmp, "p_flags  0x%08X\n", data.p_flags ); str.append(tmp);
	sprintf(tmp, "p_align  0x%08X\n", data.p_align ); str.append(tmp);

	return str;
}

u32 Elf32ProgramHeader::getType() const {
	return data.p_type;
}

u32 Elf32ProgramHeader::getOffset() const {
	return data.p_offset;
}

u32 Elf32ProgramHeader::getVAddr() const {
	return data.p_vaddr;
}

u32 Elf32ProgramHeader::getPAddr() const {
	return data.p_paddr;
}

u32 Elf32ProgramHeader::getFileSz() const {
	return data.p_filesz;
}

u32 Elf32ProgramHeader::getMemSz() const {
	return data.p_memsz;
}

u32 Elf32ProgramHeader::getFlags() const {
	return data.p_flags;
}

u32 Elf32ProgramHeader::getAlign() const {
	return data.p_align;
}
