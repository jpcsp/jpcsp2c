 /*
  * 
  *  30/04/2011 -  synced with 734
  */
#pragma once

namespace NIDMapper {
	void initialize();

	// This function is only for the HLE. It allows us to HLE modules, normally
	// a module would be loaded into memory, so imports would jump to the
	// function. What we are doing here is making the import a syscall, which
	// we can trap and turn into a C++ function call.
	// code: The syscall code. This must come from the unallocated set.
	// nid:  The NID the syscall will map to.
	void addSyscallNid(u32 code, u32 nid);

	// modulename: For example "sceRtc"
	// address: Address of export (example: start of function).
	void addModuleNid(const string &moduleName, u32 nid, u32 address);

	// use this when unloading modules
	void removeModuleNids(const string &moduleName);

	// returns -1 if the nid couldn't be mapped
	u32 nidToSyscall(u32 nid);

	// returns -1 if the nid couldn't be mapped
	u32 moduleNidToAddress(const string &moduleName, u32 nid);
};
