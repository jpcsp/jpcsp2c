/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r368)
 */


#pragma once

class Elf32Relocate {
public:
	Elf32Relocate();
	Elf32Relocate(ifstream &f);
	~Elf32Relocate();

	static const size_t sizeOf();
	string toString() const;

	u32 getOffset() const;
	u32 getInfo() const;

	void setOffset(u32 offset);
	void setInfo(u32 info);

private:
	struct {
		u32 r_offset;
		u32 r_info;
	} data;
};
