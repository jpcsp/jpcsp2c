/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r414)
 */

#pragma once

class FunctionLibrary {
public:
	FunctionLibrary();
	~FunctionLibrary();

	string getFunctionFromLibrary(const string &library, const string &nid);
	void addModule(const string &module);
	void addFunctionToLibrary(
		const string &module, const string &nid, const string &name);

	u32 getModuleCount() const;
	u32 getFunctionCount() const;

private:
	u32 m_moduleCount;
	u32 m_functionCount;
	map<string, class Module> m_moduleTable;
	vector<class Module> m_moduleList;
};
