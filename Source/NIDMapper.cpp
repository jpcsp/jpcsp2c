
 /*
  * 
  *  30/04/2011 - synced with 734
  */

#include "Common.h"
#include "NIDMapper.h"

#include "syscallsFirm15.h"

typedef map<u32, u32>       nidmap;
typedef map<string, nidmap> modmap;

static nidmap nidToSyscallMap;
static modmap moduleToNidTable;

void NIDMapper::initialize() {
	nidToSyscallMap.clear();
	moduleToNidTable.clear();

	for (size_t i = 0; i < sizeof(calls) / sizeof(HLEFunction); ++i)
		nidToSyscallMap[calls[i].nid] = calls[i].syscall;
}
/** returns -1 if the nid couldn't be mapped */
u32 NIDMapper::nidToSyscall(u32 nid) {
	nidmap::iterator i = nidToSyscallMap.find(nid);
	return i != nidToSyscallMap.end() ? i->second : (u32)-1;
}

    /**
     * This function is only for the HLE. It allows us to HLE modules, normally
     * a module would be loaded into memory, so imports would jump to the
     * function. What we are doing here is making the import a syscall, which
     * we can trap and turn into a Java function call.
     * @param code The syscall code. This must come from the unallocated set.
     * @param nid The NID the syscall will map to. */
void NIDMapper::addSyscallNid(u32 nid, u32 code) {
	nidToSyscallMap[nid] = code;
}

    /** @param modulename Example: sceRtc
     * @param address Address of export (example: start of function). */
void NIDMapper::addModuleNid(const string &moduleName, u32 nid, u32 address) {
	moduleToNidTable[moduleName][nid] = address;
}

/** Use this when unloading modules */
void NIDMapper::removeModuleNids(const string &moduleName) {
	moduleToNidTable.erase(moduleName);
}


 /** returns -1 if the nid couldn't be mapped */
u32 NIDMapper::moduleNidToAddress(const string &moduleName, u32 nid) {
	modmap::iterator nidToAddress = moduleToNidTable.find(moduleName);
	if (nidToAddress == moduleToNidTable.end())
		return (u32)-1; // module is not loaded

	nidmap::iterator addr = nidToAddress->second.find(nid);
	if (addr == nidToAddress->second.end())
		return (u32)-1; // nid is not recognized

	return addr->second;
}
