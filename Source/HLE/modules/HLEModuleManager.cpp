#include "Common.h"
#include "HLEModuleFunction.h"
#include "HLEThread.h"
#include "HLEModuleManager.h"
#include "HLE/modules/HLEModule.h"
#include "HLE/modules150/Sample.h"
#include "HLE/modules150/StdioForUser.h"
#include "HLE/modules150/sceRtc.h"
#include "NIDMapper.h"

	map<u32, HLEModuleFunction *> syscallCodeToFunction;
    u32 syscallCodeAllocator;

    list<HLEThread*> hleThreadList;

    map<string, list<HLEModule*>> flash0prxMap;

    /** The current firmware version we are using
     * was supposed to be one of pspSysMem.PSP_FIRMWARE_* but there's a mistake
     * in the module autogen and now its an int in this format:
     * ABB, where A = major and B = minor, for example 271 */
    u32 firmwareVersion;

int HLEModuleManager::psfFirmwareVersionToInt(string firmwareVersion)
{
    int version = 150;

    /*if (firmwareVersion != null) { //TODO!!!
            version = (int)(Float.parseFloat(firmwareVersion) * 100);

            // HACK we started implementing stuff under 150 even if it existed in 100
            if (version < 150)
                version = 150;
        }*/

        return version;
}
void HLEModuleManager::Initialise(int firmwareVersionparam)
{
	// Official syscalls start at 0x2000,
    // so we'll put the HLE syscalls far away at 0x4000.
    syscallCodeAllocator = 0x4000;

    

   firmwareVersion = firmwareVersionparam;

   installDefaultModules();
   initialiseFlash0PRXMap();

}
//static Sample *test = new Sample();
static StdioForUser *Stdio =new StdioForUser();
static sceRtc *rtc = new sceRtc();

static HLEModule* defaultModules[] = {
	{rtc},
	{Stdio}
};

void HLEModuleManager::installDefaultModules()
{
    //Modules.log.debug("Loading HLE firmware up to version " + firmwareVersion);
	 for (int i = 0; i < 2/*sizeof(defaultModules)*/; i++)//hmm something is really bad here!!
	 {
	   defaultModules[i]->installModule(firmwareVersion);
	 }
}
void HLEModuleManager::initialiseFlash0PRXMap()
{
 //TODO
}
void HLEModuleManager::addFunction(HLEModuleFunction *func, int nid)
{
	func->setNid(nid);
	int code = NIDMapper::nidToSyscall(func->getNid());
    if(code ==-1)
	{
            // Allocate an arbitrary syscall code to the function
            code = syscallCodeAllocator;
            // Add the new code to the NIDMapper
            NIDMapper::addSyscallNid(func->getNid(), syscallCodeAllocator);
            syscallCodeAllocator++;
	}
        /*
        System.out.println("HLEModuleManager - registering "
                + func.getModuleName() + "_"
                + String.format("%08x", func.getNid()) + "_"
                + func.getFunctionName()
                + " to " + Integer.toHexString(code));
        */
	func->setSyscallCode(code);
	syscallCodeToFunction.insert(make_pair(code, func));
}
void HLEModuleManager::removeFunction(HLEModuleFunction *func)
{
  //TODO
}
void HLEModuleManager::addThread(HLEThread *thread)
{
 //TODO
}
void HLEModuleManager::removeThread(HLEThread *thread)
{
 //TODO
}
void HLEModuleManager::step()
{
 //TODO
}
bool HLEModuleManager::handleSyscall(int code)
{
	 map<u32, HLEModuleFunction *>::iterator func = syscallCodeToFunction.find(code);
     if (func == syscallCodeToFunction.end())
	 {
		 return false;
	 }
	 else
	 {
		 func->second->execute();
		 return true;
	 }

}

