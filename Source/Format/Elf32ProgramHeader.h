/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r368)
 */

#pragma once

class Elf32ProgramHeader {
public:
	Elf32ProgramHeader();
	Elf32ProgramHeader(ifstream &f);
	~Elf32ProgramHeader();

	void read(ifstream &f);

	static const size_t sizeOf();
	string toString() const;

	u32 getType() const;
	u32 getOffset() const;
	u32 getVAddr() const;
	u32 getPAddr() const;
	u32 getFileSz() const;
	u32 getMemSz() const;
	u32 getFlags() const;
	u32 getAlign() const;

private:
	struct {
		u32 p_type;
		u32 p_offset;
		u32 p_vaddr;
		u32 p_paddr;
		u32 p_filesz;
		u32 p_memsz;

		// Bits:
		// 0x1 = executable
		// 0x2 = writable
		// 0x4 = readable
		// demo PRX's were found to be not writable
		u32 p_flags;

		u32 p_align;
	} data;
};
