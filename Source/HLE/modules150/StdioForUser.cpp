/*
*
*
*   
*    5/5/2011 -  synced with 913
*/
#include "Common.h"
#include "HLE/modules/HLEModuleFunction.h"
#include "HLE/modules/HLEThread.h"
#include "HLE/modules/HLEModuleManager.h"
#include "HLE/modules/HLEModule.h"
#include "StdioForUser.h"
#include "HLE/Modules.h"


void StdioForUser::sceKernelStdioRead()
{
	Modules::log.error("Unimplemented NID function sceKernelStdioRead [0x3054D478]");
}
void StdioForUser::sceKernelStdioLseek()
{
	Modules::log.error("Unimplemented NID function sceKernelStdioLseek [0x0CBB0571]");
}
void StdioForUser::sceKernelStdioSendChar()
{
	Modules::log.error("Unimplemented NID function sceKernelStdioSendChar [0xA46785C9]");
}
void StdioForUser::sceKernelStdioWrite()
{
	 Modules::log.error("Unimplemented NID function sceKernelStdioWrite [0xA3B931DB]");
}
void StdioForUser::sceKernelStdioClose()
{
	 Modules::log.error("Unimplemented NID function sceKernelStdioClose [0x9D061C19]");
}
void StdioForUser::sceKernelStdioOpen()
{
	Modules::log.error("Unimplemented NID function sceKernelStdioOpen [0x924ABA61]");
}
void StdioForUser::sceKernelStdin()
{
	pspRegs.GPR[2]=3;
}
void StdioForUser::sceKernelStdout()
{
	pspRegs.GPR[2]=1;
}
void StdioForUser::sceKernelStderr()
{
	pspRegs.GPR[2]=2;
}


StdioForUser::sceKernelStdioReadFunc *sceKernelStdioReadFunction = new StdioForUser::sceKernelStdioReadFunc("StdioForUser", "sceKernelStdioRead");
StdioForUser::sceKernelStdioLseekFunc *sceKernelStdioLseekFunction = new StdioForUser::sceKernelStdioLseekFunc("StdioForUser", "sceKernelStdioLseek");
StdioForUser::sceKernelStdioSendCharFunc *sceKernelStdioSendCharFunction = new StdioForUser::sceKernelStdioSendCharFunc("StdioForUser", "sceKernelStdioSendChar");
StdioForUser::sceKernelStdioWriteFunc *sceKernelStdioWriteFunction = new StdioForUser::sceKernelStdioWriteFunc("StdioForUser", "sceKernelStdioWrite") ;
StdioForUser::sceKernelStdioCloseFunc *sceKernelStdioCloseFunction = new StdioForUser::sceKernelStdioCloseFunc("StdioForUser", "sceKernelStdioClose");
StdioForUser::sceKernelStdioOpenFunc *sceKernelStdioOpenFunction = new StdioForUser::sceKernelStdioOpenFunc("StdioForUser", "sceKernelStdioOpen");
StdioForUser::sceKernelStdinFunc *sceKernelStdinFunction = new StdioForUser::sceKernelStdinFunc("StdioForUser", "sceKernelStdin");
StdioForUser::sceKernelStdoutFunc *sceKernelStdoutFunction = new StdioForUser::sceKernelStdoutFunc("StdioForUser", "sceKernelStdout") ;
StdioForUser::sceKernelStderrFunc *sceKernelStderrFunction = new StdioForUser::sceKernelStderrFunc("StdioForUser", "sceKernelStderr");

string StdioForUser::getName()
{
    return "StdioForUser";
}
void StdioForUser::installModule(int version)
{
	if (version >= 150) {

            HLEModuleManager::addFunction(sceKernelStdioReadFunction, 0x3054D478);
            HLEModuleManager::addFunction(sceKernelStdioLseekFunction, 0x0CBB0571);
            HLEModuleManager::addFunction(sceKernelStdioSendCharFunction, 0xA46785C9);
            HLEModuleManager::addFunction(sceKernelStdioWriteFunction, 0xA3B931DB);
            HLEModuleManager::addFunction(sceKernelStdioCloseFunction, 0x9D061C19);
            HLEModuleManager::addFunction(sceKernelStdioOpenFunction, 0x924ABA61);
            HLEModuleManager::addFunction(sceKernelStdinFunction, 0x172D316E);
            HLEModuleManager::addFunction(sceKernelStdoutFunction, 0xA6BAB2E9);
            HLEModuleManager::addFunction(sceKernelStderrFunction, 0xF78BA90A);

        }
}
void StdioForUser::uninstallModule(int version)
{
	 if (version >= 150) {

            HLEModuleManager::removeFunction(sceKernelStdioReadFunction);
            HLEModuleManager::removeFunction(sceKernelStdioLseekFunction);
            HLEModuleManager::removeFunction(sceKernelStdioSendCharFunction);
            HLEModuleManager::removeFunction(sceKernelStdioWriteFunction);
            HLEModuleManager::removeFunction(sceKernelStdioCloseFunction);
            HLEModuleManager::removeFunction(sceKernelStdioOpenFunction);
            HLEModuleManager::removeFunction(sceKernelStdinFunction);
            HLEModuleManager::removeFunction(sceKernelStdoutFunction);
            HLEModuleManager::removeFunction(sceKernelStderrFunction);

        }
}
 