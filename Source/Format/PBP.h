/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r615)
 */

#pragma once

#include "Elf32.h"
#include "PSF.h"

class PBP {
public:
	PBP();
	PBP(ifstream &f);
	PBP(const PBP &other);
	~PBP();

	void operator =(const PBP &other);

	static bool deleteDir(const string &path);
	static void unpackPBP(ifstream &f);

	PSF *readPSF(ifstream &file);

	string toString() const;

	bool isValid() const;

	void setElf32(const Elf32 &elf);
	void setInfo(const string &info);

	Elf32 *getElf32() const;
	PSF *getPsf() const;
	string getInfo() const;

	u32 getMagic() const;
	u32 getVersion() const;
	u32 getOffsetParam() const;
	u32 getOffsetIcon0() const;
	u32 getOffsetIcon1() const;
	u32 getOffsetPic0() const;
	u32 getOffsetPic1() const;
	u32 getOffsetSnd0() const;
	u32 getOffsetPspData() const;
	u32 getOffsetPsarData() const;
	u32 getSizeIcon0() const;

private:
	static void unpackOne(const string &path, ifstream &f, u32 offset, u32 size);

	static const u32 pbpMagic = 0x50425000;
	static const char *pbpUnpackPath;

	struct {
		u32 p_magic;
		u32 p_version;
		u32 p_offset_param_sfo;
		u32 p_offset_icon0_png;
		u32 p_offset_icon1_pmf;
		u32 p_offset_pic0_png;
		u32 p_offset_pic1_png;
		u32 p_offset_snd0_at3;
		u32 p_offset_psp_data;
		u32 p_offset_psar_data;
	} data;

	string info;

	u32 size_pbp;
	u32 size_param_sfo;
	u32 size_icon0_png;
	u32 size_icon1_pmf;
	u32 size_pic0_png;
	u32 size_pic1_png;
	u32 size_snd0_at3;
	u32 size_psp_data;
	u32 size_psar_data;

	Elf32 *elf32;
	PSF   *psf;
};
