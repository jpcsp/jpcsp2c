/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r633)
 */


#include "Common.h"
#include "Memory.h"
#include "Elf32SectionHeader.h"

Elf32SectionHeader::Elf32SectionHeader() {
}

Elf32SectionHeader::Elf32SectionHeader(ifstream &f) {
	f.read((char *)&data, sizeof(data));
}

Elf32SectionHeader::Elf32SectionHeader(u32 address) {
	memcpy(&data, Memory::getPointer(address), sizeof(data));
}

Elf32SectionHeader::~Elf32SectionHeader() {
}

const size_t Elf32SectionHeader::sizeOf() {
	return 40;
}

string Elf32SectionHeader::toString() const {
	string str;
	char tmp[128];

	sprintf(tmp, "sh_name      %08X\n", data.sh_name     ); str.append(tmp);
	sprintf(tmp, "sh_type      %08X\n", data.sh_type     ); str.append(tmp);
	sprintf(tmp, "sh_flags     %08X\n", data.sh_flags    ); str.append(tmp);
	sprintf(tmp, "sh_addr      %08X\n", data.sh_addr     ); str.append(tmp);
	sprintf(tmp, "sh_offset    %08X\n", data.sh_offset   ); str.append(tmp);
	sprintf(tmp, "sh_size      %08X\n", data.sh_size     ); str.append(tmp);
	sprintf(tmp, "sh_link      %08X\n", data.sh_link     ); str.append(tmp);
	sprintf(tmp, "sh_info      %08X\n", data.sh_info     ); str.append(tmp);
	sprintf(tmp, "sh_addralign %08X\n", data.sh_addralign); str.append(tmp);
	sprintf(tmp, "sh_entsize   %08X\n", data.sh_entsize  ); str.append(tmp);

	return str;
}

string Elf32SectionHeader::getNamez() const {
	return sh_namez;
}

void Elf32SectionHeader::setNamez(const string &namez) {
	sh_namez = namez;
}

u32 Elf32SectionHeader::getName() const {
	return data.sh_name;
}

u32 Elf32SectionHeader::getType() const {
	return data.sh_type;
}

u32 Elf32SectionHeader::getFlags() const {
	return data.sh_flags;
}

u32 Elf32SectionHeader::getAddr() const {
	return data.sh_addr;
}

u32 Elf32SectionHeader::getOffset() const {
	return data.sh_offset;
}

u32 Elf32SectionHeader::getSize() const {
	return data.sh_size;
}

u32 Elf32SectionHeader::getLink() const {
	return data.sh_link;
}

u32 Elf32SectionHeader::getInfo() const {
	return data.sh_info;
}

u32 Elf32SectionHeader::getAddrAlign() const {
	return data.sh_addralign;
}

u32 Elf32SectionHeader::getEntSize() const {
	return data.sh_entsize;
}
