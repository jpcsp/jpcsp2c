/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r639)
 */

#pragma once

class Elf32EntHeader {
public:
	Elf32EntHeader();
	Elf32EntHeader(ifstream &f);
	Elf32EntHeader(u32 address);
	~Elf32EntHeader();

	static const size_t sizeOf();
	string toString() const;

	string getModuleNamez() const;
	void setModuleNamez(const string &moduleName);

	u32 getOffsetModuleName() const;
	u16 getVersion() const;
	u16 getAttr() const;
	u8 getSize() const;
	u8 getVariableCount() const;
	u16 getFunctionCount() const;
	u32 getOffsetResident() const;

private:
	// resolved version of modulename and in a C++ std::string
	string modulenamez;

	struct {
		u32 modulename;
		u16 version;
		u16 attr;
		u8  size;
		u8  vcount;
		u16 fcount;
		u32 resident;
	} data;
};
