/*
*
*
*   
*    5/5/2011 -  synced with 913
*/
#include "Common.h"
#include "HLEModuleFunction.h"


HLEModuleFunction::HLEModuleFunction(string moduleName, string functionName) 
{
        this->moduleName = moduleName;
        this->functionName = functionName;
}
void HLEModuleFunction::setSyscallCode(int syscallCode)
{
	this->syscallCode = syscallCode;
}
int HLEModuleFunction::getSyscallCode()
{
    return syscallCode;
}
string HLEModuleFunction::getModuleName()
{
    return moduleName;
}
string HLEModuleFunction::getFunctionName()
{
   return functionName;
}
void HLEModuleFunction::setNid(int nid)
{
     this->nid = nid;
}
int HLEModuleFunction::getNid()
{
	return nid;
}


