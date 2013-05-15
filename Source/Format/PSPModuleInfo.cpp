/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r663)
 */

#include "Common.h"
#include "Memory.h"
#include "PSPModuleInfo.h"

PSPModuleInfo::PSPModuleInfo() {
}

PSPModuleInfo::PSPModuleInfo(ifstream &f) {
	read(f);
}

PSPModuleInfo::PSPModuleInfo(u32 address) {
	read(address);
}

PSPModuleInfo::~PSPModuleInfo() {
}

void PSPModuleInfo::read(ifstream &f) {
	f.read((char *)&data, sizeof(data));

	// Convert the array of bytes used for the module to C++ std::string.
	// Calculate the length of printable portion of the string, otherwise
	// any extra trailing characters may be printed as garbage.
	size_t len = 0;
	while (len < 28 && data.m_name[len] != 0)
		++len;
	m_namez = string(data.m_name, 0, len);
}

void PSPModuleInfo::read(u32 address) {
	memcpy(&data, Memory::getPointer(address), sizeof(data));

	size_t len = 0;
	while (len < 28 && data.m_name[len] != 0)
		++len;
	m_namez = string(data.m_name, 0, len);
}

u16 PSPModuleInfo::getAttr() const {
	return data.m_attr;
}

u16 PSPModuleInfo::getVersion() const {
	return data.m_version;
}

const char *PSPModuleInfo::getName() const {
    return &data.m_name[0];
}

u32 PSPModuleInfo::getGp() const {
	return data.m_gp;
}

u32 PSPModuleInfo::getExports() const {
	return data.m_exports;
}

u32 PSPModuleInfo::getExpEnd() const {
	return data.m_exp_end;
}

u32 PSPModuleInfo::getImports() const {
	return data.m_imports;
}

u32 PSPModuleInfo::getImpEnd() const {
    return data.m_imp_end;
}

string PSPModuleInfo::getNamez() const {
	return m_namez;
}
