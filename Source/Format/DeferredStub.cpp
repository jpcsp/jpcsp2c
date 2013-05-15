/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r169)
 */

#include "Common.h"
#include "DeferredStub.h"

DeferredStub::DeferredStub() {
}

DeferredStub::DeferredStub(const string &moduleName, u32 importAddress, u32 nid)
	: moduleName(moduleName), importAddress(importAddress), nid(nid)
{
}

DeferredStub::~DeferredStub() {
}

string DeferredStub::getModuleName() const {
	return moduleName;
}

u32 DeferredStub::getImportAddress() const {
	return importAddress;
}

u32 DeferredStub::getNid() const {
	return nid;
}
