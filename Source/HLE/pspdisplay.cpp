//started to added based on r913 (last one 893)
#include "Common.h"
#include "pspdisplay.h"
#include "HLE/Kernel/Types/ThreadWaitInfo.h"
#include "HLE/Kernel/Types/SceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/SceKernelThreadInfo.h"
#include "ThreadMan.h"

void pspdisplay::sceDisplayWaitVblankStart()
{
	// TODO: implement sceDisplayWaitVblankStart
	pspRegs.GPR[2] = 0;
	ThreadMan::yieldCurrentThread();

}