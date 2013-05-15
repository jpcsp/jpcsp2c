/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r615)
 */


#pragma once

class Elf32Header {
public:
	Elf32Header();
	Elf32Header(ifstream &f);
	~Elf32Header();

	void read(ifstream &f);

	bool isValid() const;
	bool isMIPSExecutable() const;
	bool isPRXDetected() const;
	bool requiresRelocation() const;

	static const size_t sizeOf();
	string toString() const;

	u32 getMagic() const;
	u8 getClass() const;
	u8 getData() const;
	u8 getIdVer() const;
	vector<u8> getPad() const;
	u16 getType() const;
    u16 getMachine() const;
	u32 getVersion() const;
	u32 getEntry() const;
	u32 getPhOff() const;
	u32 getShOff() const;
	u32 getFlags() const;
	u16 getEhSize() const;
	u16 getPhEntSize() const;
	u16 getPhNum() const;
	u16 getShEntSize();
	u16 getShNum() const;
	u16 getShStrNdx() const;

private:
	struct {
		u32 e_magic;
		u8  e_class;
		u8  e_data;
		u8  e_idver;
		u8  e_pad[9];
		u16 e_type;
		u16 e_machine;
		u32 e_version;
		u32 e_entry;
		u32 e_phoff;
		u32 e_shoff;
		u32 e_flags;
		u16 e_ehsize;
		u16 e_phentsize;
		u16 e_phnum;
		u16 e_shentsize;
		u16 e_shnum;
		u16 e_shstrndx;
	} data;
};
