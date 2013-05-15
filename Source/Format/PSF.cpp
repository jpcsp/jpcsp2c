/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r682)
 */

#include "Common.h"
#include "Emulator.h"
#include "PSF.h"

struct PsfHeader {
	u32 magic;
	u32 psfVersion;
	u32 offsetKeyTable;
	u32 offsetValueTable;
	u32 numberOfKeyPairs;
};

struct PsfSection {
	// Offset of the key name into the key table (in bytes).
	u16 offsetKeyName;

	// Unknown, always 4. Maybe alignment requirement for the data?
	u8  alignment;

	// Datatype of the value, see below.
	u8  dataType;

	// Size of value data, in bytes.
	u32 valueSize;

	// Size of value data plus padding, in bytes.
	u32 valueSizePadding;

	// Offset of the data value into the value table (in bytes) */
	u32 offsetDataValue;
};

PsfValue::PsfValue() {
}

PsfValue::~PsfValue() {
}

u32 PsfValue::GetSize() const {
	return (type == 4 ? 4 : data.size());
}

const u8 *PsfValue::GetData() const {
	if (type != 0)
		return NULL;
	else
		return &data[0];
}

const char *PsfValue::GetString() const {
	if (type != 2)
		return NULL;
	else
		return (char *)&data[0];
}

const u32 *PsfValue::GetNumber() const {
	if (type != 4)
		return NULL;
	else
		return &number;
}

PSF::PSF() {
}

PSF::PSF(u32 p_offset_param_sfo)
	: p_offset_param_sfo(p_offset_param_sfo)
{
}

PSF::~PSF() {
}

void PSF::read(ifstream &f) {
	PsfHeader hdr;
	f.seekg(p_offset_param_sfo);
	f.read((char *)&hdr, sizeof(hdr));
	if (hdr.magic != psfident) {
		Emulator::log.error("Not PSF file!");
		return;
	}

	u32 sectPos = f.tellg();
	for (u32 i = 0; i < hdr.numberOfKeyPairs; ++i) {
		PsfSection sect;
		f.seekg(sectPos);
		f.read((char *)&sect, sizeof(sect));
		sectPos = f.tellg();

		string key;
		f.seekg(p_offset_param_sfo + hdr.offsetKeyTable + sect.offsetKeyName);

		char c;
		f.read(&c, 1);
		while (c != 0) {
			key.append(&c, 0, 1);
			f.read(&c, 1);
		}

		f.seekg(p_offset_param_sfo + hdr.offsetValueTable + sect.offsetDataValue);

		PsfValue *psfValue;
		switch (sect.dataType) {
		case 0:
			if (psfMap.find(key) != psfMap.end()) {
				Emulator::log.warn("Dublicate PSF key found %s", key.c_str());
				break;
			}

			psfValue = &psfMap[key];
			psfValue->type = 0;
			psfValue->data.resize(sect.valueSize);
			f.read((char *)&psfValue->data[0], sect.valueSize);
			break;

		case 2:
			if (psfMap.find(key) != psfMap.end()) {
				Emulator::log.warn("Dublicate PSF key found %s", key.c_str());
				break;
			}

			psfValue = &psfMap[key];
			psfValue->type = 2;
			psfValue->data.resize(sect.valueSize + 1);
			f.read((char *)&psfValue->data[0], sect.valueSize);
			psfValue->data[sect.valueSize] = 0;
			break;

		case 4:
			if (psfMap.find(key) != psfMap.end()) {
				Emulator::log.warn("Dublicate PSF key found %s", key.c_str());
				break;
			}

			psfValue = &psfMap[key];
			psfValue->type = 4;
			f.read((char *)&psfValue->number, 4);
			break;

		default:
			Emulator::log.warn(
				"%s unimplemented data type %d", key, sect.dataType);
		}
	}
}

const PsfValue *PSF::get(const string &key) const {
	map<string, PsfValue>::const_iterator i = psfMap.find(key);
	if (i == psfMap.end())
		return NULL;
	else
		return &i->second;
}

bool PSF::getNumeric(const string &key, u32 &num) const {
	map<string, PsfValue>::const_iterator i = psfMap.find(key);
	if (i == psfMap.end())
		return false;
	else {
		const u32 *val = i->second.GetNumber();
		if (val == NULL)
			return false;
		else {
			num = *val;
			return true;
		}
	}
}

bool PSF::getString(const string &key, string &str) const {
	map<string, PsfValue>::const_iterator i = psfMap.find(key);
	if (i == psfMap.end())
		return false;
	else {
		const char *val = i->second.GetString();
		if (val == NULL)
			return false;
		else {
			str = val;
			return true;
		}
	}
}

bool PSF::getPrintableString(const string &key, string &str) const {
	map<string, PsfValue>::const_iterator i = psfMap.find(key);
	if (i == psfMap.end())
		return false;
	else {
		const char *val = i->second.GetString();
		if (val == NULL)
			return false;
		else {
			size_t i, len = strlen(val);
			for (i = 0; i < len; ++i)
				if (val[i] == 0 || val[i] == '\n')
					break;
			str = string(val, 0, len);
			return true;
		}
	}
}
