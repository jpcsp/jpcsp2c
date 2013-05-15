/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r169)
 */

#pragma once

class DeferredStub {
public:
	DeferredStub();
	DeferredStub(const string &moduleName, u32 importAddress, u32 nid);
	~DeferredStub();

	string getModuleName() const;
	u32 getImportAddress() const;
	u32 getNid() const;

private:
	string moduleName;
	u32 importAddress;
	u32 nid;
};
