/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r898)
 */
#include "Common.h"
#include "HLE/Kernel/Types/sceKernelCallbackInfo.h"
#include "PspGeList.h"
#include "HLE/pspge.h"


PspGeList::PspGeList(int list_addr, int stall_addr, int cbid, int arg_addr)
{
        this->list_addr = list_addr;
        this->stall_addr = stall_addr;
        this->cbid = cbid;
        this->arg_addr = arg_addr;

        context_addr = (arg_addr != 0) ? arg_addr + 4 : 0;

        base = 0x08000000; // old

        pc = list_addr;

        currentStatus = (pc == stall_addr) ? PSP_GE_LIST_STALL_REACHED : PSP_GE_LIST_QUEUED;
        syncStatus = currentStatus;
}
void PspGeList::pushSignalCallback(int arg)
{
	pspge::triggerSignalCallback(cbid, arg);
}
void PspGeList::pushFinishCallback(int arg)
{
	pspge::triggerFinishCallback(cbid, arg);
}