/*
*
*
*   
*    29/5/2011 -  synced with 734
*/

#include "Common.h"
#include "Emulator.h"



#include "SceUidManager.h"
#include "../Types/SceUid.h"

typedef map<u32, SceUid> uidmap;


static uidmap uidMap;
static u32 nextUid = 0x1000;

/** classes should call getUid to get a new unique SceUID */
u32 SceUidManager::getNewUid(const string &purpose) 
{
	uidMap[nextUid] = SceUid(purpose, nextUid);
	nextUid++;
	return nextUid - 1;
}
/** classes should call checkUidPurpose before using a SceUID
 * @return true is the uid is ok. */
bool SceUidManager::checkUidPurpose(u32 uid, const string &purpose, bool allowUnknown)
{
	uidmap::const_iterator found = uidMap.find(uid);
	if (found == uidMap.end()) {
		if (!allowUnknown) {
			Emulator::log.warn("Attempt to use unknown SceUID (purpose = %s)", purpose);
			return false;
		}
		return true;
	}
	else if (found->second.getPurpose() != purpose) {
		Emulator::log.error(	"Attempt to use SceUID for different purpose (purpose = %s) original = %s",purpose,found->second.getPurpose());
		return false;
	}
	return true;
}
/** classes should call releaseUid when they are finished with a SceUID
 * @return true on success. */
bool SceUidManager::releaseUid(u32 uid, const string &purpose) {
	uidmap::const_iterator found = uidMap.find(uid);
	if (found == uidMap.end()) {
		Emulator::log.warn("Attempt to release unknown SceUID (purpose = %s)", purpose);
		return false;
	}
	else {
		if (found->second.getPurpose() == purpose) {
			uidMap.erase(found);
			return true;
		}
		else {
			Emulator::log.warn(	"Attempt to release SceUID for different purpose (purpose = %s) original = %s",purpose,found->second.getPurpose());
			return false;
		}
	}
}

