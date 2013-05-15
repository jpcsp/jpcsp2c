/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r727)
 */

#include "Common.h"
#include "ModuleManager.h"

static map<u32, SceModule *> moduleUidToModule;
static map<string, SceModule *> moduleNameToModule;

void ModuleManager::reset() {
	// TODO:does that really needs??
	//map<u32, SceModule *>::iterator it;
	//for (it = moduleUidToModule.begin(); it != moduleUidToModule.end(); ++it)
	//	delete it->second;

	moduleUidToModule.clear();
	moduleNameToModule.clear();
}

void ModuleManager::addModule(SceModule *module) {
	moduleUidToModule[module->modid] = module;
	moduleNameToModule[module->modname] = module;
}

void ModuleManager::removeModule(u32 uid) {
	map<u32, SceModule *>::iterator &sceModule = moduleUidToModule.find(uid);
	if (sceModule != moduleUidToModule.end()) {
		moduleNameToModule.erase(sceModule->second->modname);
		moduleUidToModule.erase(sceModule);
	}
}
// used by the loader to fixup deferred imports
map<u32, SceModule *>::iterator ModuleManager::values() {
	return moduleUidToModule.begin();
}

map<u32, SceModule *>::iterator ModuleManager::valuesEnd() {
	return moduleUidToModule.end();
}


SceModule *ModuleManager::getModuleByUid(u32 uid) {
	if (moduleUidToModule.find(uid) == moduleUidToModule.end())
		return NULL;
	else
		return moduleUidToModule[uid];
}
// used by sceKernelFindModuleByName
SceModule *ModuleManager::getModuleByName(const string &name) {
	if (moduleNameToModule.find(name) == moduleNameToModule.end())
		return NULL;
	else
		return moduleNameToModule[name];
}

SceModule *ModuleManager::getModuleByAddress(u32 address) {
	static map<u32, SceModule *>::iterator i;
	for (i = moduleUidToModule.begin(); i != moduleUidToModule.end(); ++i) {
		if (address >= i->second->loadAddressLow &&
			address < i->second->loadAddressHigh)
		{
			return i->second;
		}
	}
	return NULL;
}
