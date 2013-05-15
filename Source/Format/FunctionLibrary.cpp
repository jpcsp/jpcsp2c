/*
 * 
 *  30/04/2011 - synced with 734
 *               (actual change was in r414)
 */

#include "Common.h"
#include "FunctionLibrary.h"

class Module {
public:
	Module() {
	}

	Module(const string &name) {
		m_name = name;
	}

	~Module() {
	}

	string getName() const {
		return m_name;
	}

	bool getFunctionFromModule(const string &nid, string &name) const {
		map<string, string>::const_iterator it = m_functions.find(nid);
		if (it == m_functions.end())
			return false;
		else {
			name = it->second;
			return true;
		}
	}

	u32 countFunctions() const {
		return m_functions.size();
	}

	void addFunction(const string &nid, const string &name) {
		m_functions[nid] = name;
	}

private:
	string m_name;
	map<string, string> m_functions;
};

FunctionLibrary::FunctionLibrary() {
}

FunctionLibrary::~FunctionLibrary() {
}

string FunctionLibrary::getFunctionFromLibrary(
	const string &library, const string &nid)
{
	map<string, Module>::iterator it = m_moduleTable.find(library);
	if (it == m_moduleTable.end()) {
		addModule(library);
		it = m_moduleTable.find(library);
	}

	string ret;
	if (it->second.getFunctionFromModule(nid, ret))
		return ret;

	ret = library + "_Unknow_" + nid.substr(2);
	addFunctionToLibrary(library, nid, ret);
	return ret;
}

void FunctionLibrary::addModule(const string &module) {
	m_moduleTable[module] = Module(module);
	m_moduleCount++;
}

void FunctionLibrary::addFunctionToLibrary(
	const string &module, const string &nid, const string &name)
{
	map<string, Module>::iterator it = m_moduleTable.find(module);
	if (it == m_moduleTable.end())
		addModule(module);
	m_moduleTable[module].addFunction(nid, name);
	m_functionCount++;
}

u32 FunctionLibrary::getModuleCount() const {
	return m_moduleCount;
}

u32 FunctionLibrary::getFunctionCount() const {
	return m_functionCount;
}
