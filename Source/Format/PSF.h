/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r682)
 */

#pragma once

class PsfValue {
	friend class PSF;
public:
	PsfValue();
	~PsfValue();

	u32 GetSize() const;

	const u8 *GetData() const;
	const char *GetString() const;
	const u32 *GetNumber() const;

private:
	vector<u8> data;
	u32 number;
	u8 type;
};

class PSF {
public:
	PSF();
	PSF(u32 p_offset_param_sfo);
	~PSF();

	void read(ifstream &f);

	const PsfValue *get(const string &key) const;
	bool getNumeric(const string &key, u32 &num) const;
	bool getString(const string &key, string &str) const;

	// kxploit patcher tool adds "\nKXPloit Boot by PSP-DEV Team"
	bool getPrintableString(const string &key, string &str) const;

private:
	static const u32 psfident = 0x46535000;
	map<string, PsfValue> psfMap;
	u32 p_offset_param_sfo;
};
