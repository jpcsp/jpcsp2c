/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 /*
  * 
  *  30/04/2011 - synced with 734
  */

#pragma once

#include "HLE/Kernel/Types/SceModule.h"

namespace ModuleManager {
	void reset();

	// -------------------------- helpers --------------------------

	void addModule(SceModule *module);
	void removeModule(u32 uid);

	// used by the loader to fixup deferred imports
	map<u32, SceModule *>::iterator values();
	map<u32, SceModule *>::iterator valuesEnd();

	SceModule *getModuleByUid(u32 uid);

	// used by sceKernelFindModuleByName
	SceModule *getModuleByName(const string &name);

	SceModule *getModuleByAddress(u32 address);
};
