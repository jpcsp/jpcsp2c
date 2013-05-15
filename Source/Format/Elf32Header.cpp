/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r615)
 */


#include "Common.h"
#include "Memory.h"
#include "Elf32Header.h"

Elf32Header::Elf32Header() {
}

Elf32Header::Elf32Header(ifstream &f) {
	read(f);
}

Elf32Header::~Elf32Header() {
}

void Elf32Header::read(ifstream &f) {
	f.read((char *)&data, sizeof(data));
}

bool Elf32Header::isValid() const {
	return data.e_magic == 0x464C457F;
}

bool Elf32Header::isMIPSExecutable() const {
	return data.e_machine == 8;
}
bool Elf32Header::isPRXDetected() const {
	return data.e_type == 0xFFA0;
}

bool Elf32Header::requiresRelocation() const {
	return isPRXDetected() || (data.e_entry < Memory::MainMemoryStart);
}

const size_t Elf32Header::sizeOf() {
	return 32;
}

string Elf32Header::toString() const {
	string str = "--------ELF HEADER--------\n";
	char tmp[128];

	sprintf(tmp, "e_magic     0x%08X\n", data.e_magic    ); str.append(tmp);
	sprintf(tmp, "e_class     0x%02X\n", data.e_class    ); str.append(tmp);
	sprintf(tmp, "e_data      0x%02X\n", data.e_data     ); str.append(tmp);
	sprintf(tmp, "e_idver     0x%02X\n", data.e_idver    ); str.append(tmp);
	sprintf(tmp, "e_type      0x%04X\n", data.e_type     ); str.append(tmp);
	sprintf(tmp, "e_machine   0x%04X\n", data.e_machine  ); str.append(tmp);
	sprintf(tmp, "e_version   0x%08X\n", data.e_version  ); str.append(tmp);
	sprintf(tmp, "e_entry     0x%08X\n", data.e_entry    ); str.append(tmp);
	sprintf(tmp, "e_phoff     0x%08X\n", data.e_phoff    ); str.append(tmp);
	sprintf(tmp, "e_shoff     0x%08X\n", data.e_shoff    ); str.append(tmp);
	sprintf(tmp, "e_flags     0x%08X\n", data.e_flags    ); str.append(tmp);
	sprintf(tmp, "e_ehsize    0x%04X\n", data.e_ehsize   ); str.append(tmp);
	sprintf(tmp, "e_phentsize 0x%04X\n", data.e_phentsize); str.append(tmp);
	sprintf(tmp, "e_phnum     0x%04X\n", data.e_phnum    ); str.append(tmp);
	sprintf(tmp, "e_shentsize 0x%04X\n", data.e_shentsize); str.append(tmp);
	sprintf(tmp, "e_shnum     0x%04X\n", data.e_shnum    ); str.append(tmp);
	sprintf(tmp, "e_shstrndx  0x%04X\n", data.e_shstrndx ); str.append(tmp);

	return str;
}

u32 Elf32Header::getMagic() const {
	return data.e_magic;
}

u8 Elf32Header::getClass() const {
	return data.e_class;
}

u8 Elf32Header::getData() const {
	return data.e_data;
}

u8 Elf32Header::getIdVer() const {
	return data.e_idver;
}

vector<u8> Elf32Header::getPad() const {
	vector<u8> pad(9);
	memcpy(&pad[0], &data.e_pad[0], 9);
	return pad;
}

u16 Elf32Header::getType() const {
	return data.e_type;
}

u16 Elf32Header::getMachine() const {
	return data.e_machine;
}

u32 Elf32Header::getVersion() const {
	return data.e_version;
}

u32 Elf32Header::getEntry() const {
	return data.e_entry;
}

u32 Elf32Header::getPhOff() const {
	return data.e_phoff;
}

u32 Elf32Header::getShOff() const {
	return data.e_shoff;
}

u32 Elf32Header::getFlags() const {
	return data.e_flags;
}

u16 Elf32Header::getEhSize() const {
	return data.e_ehsize;
}

u16 Elf32Header::getPhEntSize() const {
	return data.e_phentsize;
}

u16 Elf32Header::getPhNum() const {
	return data.e_phnum;
}

u16 Elf32Header::getShEntSize() {
	return data.e_shentsize;
}

u16 Elf32Header::getShNum() const {
	return data.e_shnum;
}

u16 Elf32Header::getShStrNdx() const {
	return data.e_shstrndx;
}
