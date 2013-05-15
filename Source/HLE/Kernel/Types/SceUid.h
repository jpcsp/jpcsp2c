/*
*
*
*   
*    29/5/2011 -  synced with 734
*/

#pragma once

class SceUid {
public:
	SceUid();
	SceUid(const string &purpose, u32 uid);
	~SceUid();

	string getPurpose() const;
	u32 getUid() const;

	bool operator < (const SceUid &other) const {
		return uid < other.uid;
	}
private:
	string purpose;
	u32 uid;
};
