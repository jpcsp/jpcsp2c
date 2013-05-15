/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r913)
 */



#include "Common.h"
#include "Emulator.h"
#include "Memory.h"
#include "ThreadWaitInfo.h"
#include "sceKernelCallbackInfo.h"
#include "SceKernelThreadInfo.h"
#include "Allegrex/Allegrex.h"
#include "SceKernelErrors.h"

#include "HLE/Kernel/Managers/SceUidManager.h"
#include "HLE/ThreadMan.h"


SceKernelThreadInfo::SceKernelThreadInfo(const string& name, int entry_addr, int initPriority, int stackSize, int attr)
{
	memset((SceKernelThreadInfo_t *)this, 0, sizeof(SceKernelThreadInfo_t));
	memset(&cpuContext,0,sizeof(cpuContext));
	// Ignore 0 size from the idle threads (don't want them stealing space)
	if (stackSize != 0) {
		if (stackSize < 512) {
			// 512 byte min
			stackSize = 512;
		} 
		else 
		{
			// 256 byte size alignment (should be 16?)
			stackSize = (stackSize + 0xFF) & ~0xFF;
		}
	}

	strncpy(this->name, name.c_str(), 32);

	this->entry_addr = entry_addr;
	this->initPriority = initPriority;
	this->stackSize = stackSize;
	this->attr = attr;

	status = PSP_THREAD_SUSPEND;
	stack_addr =  ThreadMan::mallocStack(stackSize);
	if (stack_addr != 0 && stackSize > 0 && (attr & PSP_THREAD_ATTR_NO_FILLSTACK) != PSP_THREAD_ATTR_NO_FILLSTACK)
	{
		memset(Memory::getPointer(stack_addr),0xFF,stackSize);          
	}
	gpReg_addr = pspRegs.GPR[28]; // inherit gpReg // TODO addr into ModuleInfo struct?
	currentPriority = initPriority;
	waitType = 0; // not yet used by us
	waitId = 0; // ?
	wakeupCount = 0;
	exitStatus = ERROR_THREAD_IS_NOT_DORMANT;
	runClocks = 0;
	intrPreemptCount = 0;
	threadPreemptCount = 0;
	releaseCount = 0;

	// internal state
	uid = SceUidManager::getNewUid("ThreadMan-thread");

	cpuContext=pspRegs; //copy cpuregs to cpuContext
	// Thread specific registers
	cpuContext.pc = entry_addr;
	cpuContext.npc = entry_addr; // + 4;

	// sp, 512 byte padding at the top for user data, this will get re-jigged when we call start thread
	cpuContext.GPR[29] = stack_addr + stackSize - 512;
	cpuContext.GPR[26] = cpuContext.GPR[29]; // k0 mirrors initial sp (almost)

	// We'll hook "jr ra" where ra = 0 as the thread exiting
	cpuContext.GPR[31] = 0; // ra

	do_delete = false;
	do_callbacks = false;
	for (int i = 0; i < THREAD_CALLBACK_SIZE; i++) {
		callbackRegistered[i] = false;
		callbackReady[i] = false;
		callbackInfo[i] = NULL;
	}
}


void SceKernelThreadInfo::saveContext()
{
	cpuContext=pspRegs;
	
}

void SceKernelThreadInfo::restoreContext()
{
	// Assuming context switching only happens on syscall,
	// we always execute npc after a syscall,
	// so we can set pc = npc regardless of cop0.status.bd.
	//if (!cpu.cop0_status_bd)
	cpuContext.pc = cpuContext.npc;

	pspRegs = cpuContext;  
}
    /** For use in the scheduler */  //TODO???
/*    @Override
    public int compare(SceKernelThreadInfo o1, SceKernelThreadInfo o2) {
        return o1.currentPriority - o2.currentPriority;
    }*/
void SceKernelThreadInfo::write(u32 address)
{
	size=sizeof(SceKernelThreadInfo_t);
	memcpy(Memory::getPointer(address), (SceKernelThreadInfo_t *)this, sizeof(SceKernelThreadInfo_t));

}