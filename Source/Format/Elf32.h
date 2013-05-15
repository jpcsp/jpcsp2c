/*
 * 
 *  30/04/2011 - synced with 734
 *               
 */
#pragma once

#include "Elf32Header.h"
#include "Elf32ProgramHeader.h"
#include "Elf32SectionHeader.h"

class Elf32 {
public:
	Elf32();
	Elf32(ifstream &f);
	~Elf32();

	// returns: The elf was loaded from some kind of file or buffer. The elf
	// offset is an offset into this buffer where the elf actually starts. If
	// the returned offset is non-zero this is typically due to the elf being
	// embedded inside a pbp.
	u32 getElfOffset() const;

	Elf32Header &getHeader();
	vector<Elf32ProgramHeader> &getProgramHeaderList();
	Elf32ProgramHeader *getProgramHeader(u32 index);
	vector<Elf32SectionHeader> &getSectionHeaderList();
	Elf32SectionHeader *getSectionHeader(u32 index);
	Elf32SectionHeader *getSectionHeader(const string &name);

	string getElfInfo() const;
	string getProgInfo() const;
	string getSectInfo() const;

private:
	void loadHeader(ifstream &f);
	void loadProgramHeaders(ifstream &f);
	void loadSectionHeaders(ifstream &f);

	// file offset
	u32 elfOffset;

	// headers
	Elf32Header header;
	vector<Elf32ProgramHeader> programHeaderList;
	vector<Elf32SectionHeader> sectionHeaderList;
	map<string, Elf32SectionHeader> sectionHeaderMap;
	Elf32SectionHeader *shstrtab;

	// debug info
	string ElfInfo;  // ELF header
	string ProgInfo; // ELF program headers
	string SectInfo; // ELF section headers
};

