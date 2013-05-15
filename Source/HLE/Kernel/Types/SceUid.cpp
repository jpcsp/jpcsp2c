/*
*
*
*   
*    29/5/2011 -  synced with 734
*/
#include "Common.h"
#include "Emulator.h"
#include "SceUid.h"

SceUid::SceUid() {
}

SceUid::SceUid(const string &purpose, u32 uid)
	: purpose(purpose), uid(uid)
{
}

SceUid::~SceUid() {
}

string SceUid::getPurpose() const {
	return purpose;
}

u32 SceUid::getUid() const {
	return uid;
}