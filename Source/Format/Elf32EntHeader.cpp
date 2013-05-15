/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r639)
 */

#include "Common.h"
#include "Memory.h"
#include "Elf32EntHeader.h"

Elf32EntHeader::Elf32EntHeader() {
}

Elf32EntHeader::Elf32EntHeader(ifstream &f) {
	modulenamez.clear();
	f.read((char *)&data, sizeof(data));
}

Elf32EntHeader::Elf32EntHeader(u32 address) {
	modulenamez.clear();
	memcpy(&data, Memory::getPointer(address), sizeof(data));
}

Elf32EntHeader::~Elf32EntHeader() {
}

const size_t Elf32EntHeader::sizeOf() {
	return 16;
}

string Elf32EntHeader::toString() const {
	string str;
	char tmp[128];

	if (modulenamez.length() > 0) {
		str.append(modulenamez);
		str.append("\n");
	}

	sprintf(tmp, "modulename %08X\n", data.modulename); str.append(tmp);
	sprintf(tmp, "version    %04X\n", data.version   ); str.append(tmp);
	sprintf(tmp, "attr       %04X\n", data.attr      ); str.append(tmp);
	sprintf(tmp, "size       %02X\n", data.size      ); str.append(tmp);
	sprintf(tmp, "vcount     %02X\n", data.vcount    ); str.append(tmp);
	sprintf(tmp, "fcount     %04X\n", data.fcount    ); str.append(tmp);
	sprintf(tmp, "resident   %08X\n", data.resident  ); str.append(tmp);

	return str;
}

string Elf32EntHeader::getModuleNamez() const {
	return modulenamez;
}

void Elf32EntHeader::setModuleNamez(const string &moduleName) {
	modulenamez = moduleName;
}

u32 Elf32EntHeader::getOffsetModuleName() const {
	return data.modulename;
}

u16 Elf32EntHeader::getVersion() const {
	return data.version;
}

u16 Elf32EntHeader::getAttr() const {
	return data.attr;
}

u8 Elf32EntHeader::getSize() const {
	return data.size;
}

u8 Elf32EntHeader::getVariableCount() const {
	return data.vcount;
}

u16 Elf32EntHeader::getFunctionCount() const {
	return data.fcount;
}

u32 Elf32EntHeader::getOffsetResident() const {
	return data.resident;
}
