/* WIP**********************************
 * 
 *  02/05/2011 - WIP synced with 913
 *               (actual change was in r891)
 */

#include "Common.h"
#include "HLE/Kernel/Types/sceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/PspGeList.h"
#include "pspge.h"

Log pspge::log;

bool DEFER_CALLBACKS = true;
int syncThreadId;
bool waitingForSync;
bool syncDone;
list<PspGeList *> listQueue; 
int listIdAllocator;
    //private HashMap<Integer, SceKernelCallbackInfo> signalCallbacks; TODO
    //private HashMap<Integer, SceKernelCallbackInfo> finishCallbacks; TODO
 //   private ConcurrentLinkedQueue<DeferredCallbackInfo> deferredSignalCallbackQueue; TODO
 //   private ConcurrentLinkedQueue<DeferredCallbackInfo> deferredFinishCallbackQueue; TODO

void pspge::Initialise()
{
        syncThreadId = -1;
        waitingForSync = false;
        syncDone = false;
        listIdAllocator = 0;
		log = Log("pspge");
}
void pspge::step(){}
void pspge::sceGeEdramGetSize()
{
	pspRegs.GPR[2] = 0x00200000;
}
void pspge::sceGeEdramGetAddr()
{
	pspRegs.GPR[2]=0x04000000;
	
}
void pspge::sceGeListEnQueue(u32 list_addr, u32 stall_addr, u32 cbid, u32 arg_addr)
{
	pspge::log.debug("sceGeListEnQueue(list=0x%x , stall=0x%x, cbid=0x%x , arg=0x%x) result id %u",list_addr,stall_addr,cbid,arg_addr,listIdAllocator);

    PspGeList *list = new PspGeList(list_addr, stall_addr, cbid, arg_addr);
    list->id = listIdAllocator++;
	listQueue.push_back(list);
	pspRegs.GPR[2]=list->id;
}
void pspge::sceGeListDeQueue(u32 id){}
void pspge::sceGeListUpdateStallAddr(u32 id, u32 stall_addr)
{

	bool found = false;
	for(list<PspGeList *>::iterator it = listQueue.begin(); it != listQueue.end(); ++it) {
		PspGeList *list = *it;
		if(list->id== id)
		{
			list->stall_addr = stall_addr;
			if(list->currentStatus == PSP_GE_LIST_STALL_REACHED)
                    list->currentStatus = PSP_GE_LIST_QUEUED;
			pspge::log.trace("sceGeListUpdateStallAddr(id=%u ,stall=0x%x) ok",id,stall_addr);
			pspRegs.GPR[2]=0;
			found=true;
			break;
		}
	}
	 if (!found) {
      pspge::log.warn("sceGeListUpdateStallAddr(id=%u ,stall=0x%x) failed",id,stall_addr);
      pspRegs.GPR[2] = -1;
	 }
}
void pspge::sceGeListSync(u32 id, u32 syncType){}
void pspge::sceGeDrawSync(u32 syncType){}
void pspge::sceGeSetCallback(u32 cbdata_addr){}
void pspge::sceGeUnsetCallback(u32 cbid){}


void pspge::triggerCallback(int cbid, int callbackNotifyArg1, int callbackIndex, map<u32, SceKernelCallbackInfo *> callbacks){}
void pspge::triggerFinishCallback(int cbid, int callbackNotifyArg1){}
void pspge::triggerSignalCallback(int cbid, int callbackNotifyArg1){}
void pspge::hleGeListSyncDone(PspGeList list){}
bool pspge::hleGeListSync(PspGeList list, int syncType){ return false;}