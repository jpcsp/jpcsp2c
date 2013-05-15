/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r519)
 */

#pragma once

class Elf32StubHeader {
public:
	Elf32StubHeader();
	Elf32StubHeader(ifstream &f);
	Elf32StubHeader(u32 address);
	~Elf32StubHeader();

	static const size_t sizeOf();
	string toString() const;

	string getModuleNamez() const;
	void setModuleNamez(const string &moduleName);

	u32 getOffsetModuleName() const;
	u16 getVersion() const;
	u16 getFlags() const;
	u16 getSize() const;
	u16 getImports() const; // the number of imports from this module
	u32 getOffsetNid() const;
	u32 getOffsetText() const;

private:
	struct {
		u32 s_modulename;
		u16 s_version;
		u16 s_flags;
		u16 s_size; // var count in upper 8bits?
		u16 s_imports;
		u32 s_nid;
		u32 s_text;
	} data;

	// resolved version of s_modulename and in a C++ std::string
	string s_modulenamez;
};
