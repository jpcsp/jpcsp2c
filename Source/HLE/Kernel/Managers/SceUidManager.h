/*
*
*
*   
*    29/5/2011 -  synced with 734
*/

#pragma once

namespace SceUidManager {
	// classes should call getNewUid to get a new unique SceUID
	u32 getNewUid(const string &purpose);

	// classes should call releaseUid when they are finished with a SceUID
	// return true on success
	bool releaseUid(u32 uid, const string &purpose);

	// classes should call checkUidPurpose before using a SceUID
	// returns true if the uid is ok
	bool checkUidPurpose(u32 uid, const string &purpose, bool allowUnknown);
};