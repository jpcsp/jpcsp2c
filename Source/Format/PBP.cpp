/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r615)
 */

#include "Common.h"
#include "PBP.h"

const char *PBP::pbpUnpackPath = "unpacked-pbp";

bool PBP::deleteDir(const string &path) {
	// TODO: implement
	return true;
}

void PBP::unpackOne(const string &path, ifstream &f, u32 offset, u32 size) {
	if (size == 0)
		return;

	auto_ptr<u8> data(new u8[size]);
	f.seekg(offset);
	f.read((char *)data.get(), size);

	ofstream ofs;
	ofs.open(path.c_str(), ios::out | ios::binary);
	if (ofs.is_open()) {
		ofs.write((char *)data.get(), size);
		ofs.close();
	}
}

void PBP::unpackPBP(ifstream &f) {
	f.seekg(0);
	PBP pbp(f);

	if (!pbp.isValid())
		return;
	
	deleteDir(pbpUnpackPath);
	mkdir(pbpUnpackPath);

	string path;

	path = pbpUnpackPath;
	path.append("\\param.sfo");
	unpackOne(path, f, pbp.data.p_offset_param_sfo, pbp.size_param_sfo);

	path = pbpUnpackPath;
	path.append("\\icon0.png");
	unpackOne(path, f, pbp.data.p_offset_icon0_png, pbp.size_icon0_png);

	path = pbpUnpackPath;
	path.append("\\icon1.pmf");
	unpackOne(path, f, pbp.data.p_offset_icon1_pmf, pbp.size_icon1_pmf);

	path = pbpUnpackPath;
	path.append("\\pic0.png");
	unpackOne(path, f, pbp.data.p_offset_pic0_png, pbp.size_pic0_png);

	path = pbpUnpackPath;
	path.append("\\pic1.png");
	unpackOne(path, f, pbp.data.p_offset_pic1_png, pbp.size_pic1_png);

	path = pbpUnpackPath;
	path.append("\\snd0.at3");
	unpackOne(path, f, pbp.data.p_offset_snd0_at3, pbp.size_snd0_at3);

	path = pbpUnpackPath;
	path.append("\\data.psp");
	unpackOne(path, f, pbp.data.p_offset_psp_data, pbp.size_psp_data);

	path = pbpUnpackPath;
	path.append("\\data.psar");
	unpackOne(path, f, pbp.data.p_offset_psar_data, pbp.size_psar_data);
}

PBP::PBP()
	: elf32(NULL), psf(NULL)
{
}

PBP::PBP(ifstream &f)
	: elf32(NULL), psf(NULL)
{
	u32 pos = f.tellg();
	f.seekg(0, ios::end);
	size_pbp = f.tellg();
	f.seekg(pos, ios::beg);

	if (size_pbp == 0)
		return;

	f.read((char *)&data, sizeof(data));
	if (isValid()) {
		size_param_sfo = data.p_offset_icon0_png - data.p_offset_param_sfo;
		size_icon0_png = data.p_offset_icon1_pmf - data.p_offset_icon0_png;
		size_icon1_pmf = data.p_offset_pic0_png  - data.p_offset_icon1_pmf;
		size_pic0_png  = data.p_offset_pic1_png  - data.p_offset_pic0_png;
		size_pic1_png  = data.p_offset_snd0_at3  - data.p_offset_pic1_png;
		size_snd0_at3  = data.p_offset_psp_data  - data.p_offset_snd0_at3;
		size_psp_data  = data.p_offset_psar_data - data.p_offset_psp_data;
		size_psar_data = size_pbp - data.p_offset_psar_data;

		info = toString();
	}
}

PBP::PBP(const PBP &other) {
	*this = other;
}

PBP::~PBP() {
	if (elf32 != NULL) {
		delete elf32;
		elf32 = NULL;
	}
	if (psf != NULL) {
		delete psf;
		psf = NULL;
	}
}

void PBP::operator =(const PBP &other) {
	data = other.data;

	info = other.info;

	size_pbp = other.size_pbp;
	size_param_sfo = other.size_param_sfo;
	size_icon0_png = other.size_icon0_png;
	size_icon1_pmf = other.size_icon1_pmf;
	size_pic0_png = other.size_pic0_png;
	size_pic1_png = other.size_pic1_png;
	size_snd0_at3 = other.size_snd0_at3;
	size_psp_data = other.size_psp_data;
	size_psar_data = other.size_psar_data;

	elf32 = NULL;
	if (other.elf32 != NULL) {
		elf32 = new Elf32();
		*elf32 = *other.elf32;
	}

	psf = NULL;
	if (other.psf != NULL) {
		psf = new PSF();
		*psf = *other.psf;
	}
}

PSF *PBP::readPSF(ifstream &file) {
	if (data.p_offset_param_sfo == 0)
		return NULL;
	else {
		psf = new PSF(data.p_offset_param_sfo);
		psf->read(file);
		return psf;
	}
}

string PBP::toString() const {
	string str = "--------PBP HEADER--------\n";
	char tmp[128];

	sprintf(tmp, "p_magic            0x%08X\n", data.p_magic           ); str.append(tmp);
	sprintf(tmp, "p_version          0x%08X\n", data.p_version         ); str.append(tmp);
	sprintf(tmp, "p_offset_param_sfo 0x%08X\n", data.p_offset_param_sfo); str.append(tmp);
	sprintf(tmp, "p_offset_icon0_png 0x%08X\n", data.p_offset_icon0_png); str.append(tmp);
	sprintf(tmp, "p_offset_icon1_pmf 0x%08X\n", data.p_offset_icon1_pmf); str.append(tmp);
	sprintf(tmp, "p_offset_pic0_png  0x%08X\n", data.p_offset_pic0_png ); str.append(tmp);
	sprintf(tmp, "p_offset_pic1_png  0x%08X\n", data.p_offset_pic1_png ); str.append(tmp);
	sprintf(tmp, "p_offset_snd0_at3  0x%08X\n", data.p_offset_snd0_at3 ); str.append(tmp);
	sprintf(tmp, "p_offset_psp_data  0x%08X\n", data.p_offset_psp_data ); str.append(tmp);
	sprintf(tmp, "p_offset_psar_data 0x%08X\n", data.p_offset_psar_data); str.append(tmp);

	return str;
}

bool PBP::isValid() const {
	return (size_pbp != 0 && data.p_magic == pbpMagic);
}

void PBP::setElf32(const Elf32 &elf) {
	if (elf32 != NULL) {
		delete elf32;
		elf32 = NULL;
	}

	elf32 = new Elf32();
	*elf32 = elf;
}

void PBP::setInfo(const string &info) {
	this->info = info;
}

Elf32 *PBP::getElf32() const {
	return elf32;
}

PSF *PBP::getPsf() const {
	return psf;
}

string PBP::getInfo() const {
	return info;
}

u32 PBP::getMagic() const {
	return data.p_magic;
}

u32 PBP::getVersion() const {
	return data.p_version;
}

u32 PBP::getOffsetParam() const {
	return data.p_offset_param_sfo;
}

u32 PBP::getOffsetIcon0() const {
	return data.p_offset_icon0_png;
}

u32 PBP::getOffsetIcon1() const {
	return data.p_offset_icon1_pmf;
}

u32 PBP::getOffsetPic0() const {
	return data.p_offset_pic0_png;
}

u32 PBP::getOffsetPic1() const {
	return data.p_offset_pic1_png;
}

u32 PBP::getOffsetSnd0() const {
	return data.p_offset_snd0_at3;
}

u32 PBP::getOffsetPspData() const {
	return data.p_offset_psp_data;
}

u32 PBP::getOffsetPsarData() const {
	return data.p_offset_psar_data;
}

u32 PBP::getSizeIcon0() const {
	return size_icon0_png;
}
