/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r845)
 */



#include "Common.h"
#include "Memory.h"
#include "ThreadWaitInfo.h"
#include "SceKernelCallbackInfo.h"
#include "SceKernelThreadInfo.h"

#include "HLE/Kernel/Managers/SceUidManager.h"

SceKernelCallbackInfo::SceKernelCallbackInfo(const string& name, int threadId, int callback_addr, int callback_arg_addr)
{
	memset((SceKernelCallbackInfo_t *)this, 0, sizeof(SceKernelCallbackInfo_t));
           
	    strncpy(this->name, name.c_str(), 32);
        this->threadId = threadId;
        this->callback_addr = callback_addr;
        this->callback_arg_addr = callback_arg_addr;
        this->notifyCount = 0;
        this->notifyArg1 = 0;
        this->notifyArg2 = 0;

        // internal state
        uid = SceUidManager::getNewUid("ThreadMan-callback");
}
void SceKernelCallbackInfo::write(int address)
{
  	size=sizeof(SceKernelCallbackInfo_t);
	memcpy(Memory::getPointer(address), (SceKernelCallbackInfo_t*)this, sizeof(SceKernelCallbackInfo_t));

}
/** Call this to switch in the callback.
* Sets up a copy of the parent thread's context for the callback to run in.
* @param cpu Should be the cpuContext from the thread this callback belongs to.
* @param thread the thread this callback belongs to.
*/

void SceKernelCallbackInfo::startContext(pspRegisters cpuContext/*, SceKernelThreadInfo *thread*/) 
{     
        cpuContext.pc = cpuContext.npc = callback_addr;
        cpuContext.GPR[4] = notifyArg1;
		cpuContext.GPR[5] = notifyArg2;
		cpuContext.GPR[6] = callback_arg_addr;
		cpuContext.GPR[31] = 0; // ra

		//TODO check this.. we copy callback cpu to cpu??
        pspRegs = cpuContext;
		//RuntimeContext.executedCallback(thread); //TODO recompiler stuff
}