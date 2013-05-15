#define PSP_GE_LIST_DONE           0
#define PSP_GE_LIST_QUEUED         1
#define PSP_GE_LIST_DRAWING_DONE   2
#define PSP_GE_LIST_STALL_REACHED  3
#define PSP_GE_LIST_CANCEL_DONE    4

namespace pspge
{
	 extern Log log;
	 void Initialise();
	 void step();
	 void sceGeEdramGetSize();
	 void sceGeEdramGetAddr();
	 void sceGeListEnQueue(u32 list_addr, u32 stall_addr, u32 cbid,u32 arg_addr);
	 void sceGeListDeQueue(u32 id);
	 void sceGeListUpdateStallAddr(u32 id, u32 stall_addr);
	 void sceGeListSync(u32 id, u32 syncType);
	 void sceGeDrawSync(u32 syncType);
	 void sceGeSetCallback(u32 cbdata_addr);
	 void sceGeUnsetCallback(u32 cbid);


	 void triggerCallback(int cbid, int callbackNotifyArg1, int callbackIndex, map<u32, SceKernelCallbackInfo *> callbacks);
	 void triggerFinishCallback(int cbid, int callbackNotifyArg1);
	 void triggerSignalCallback(int cbid, int callbackNotifyArg1);
	 void hleGeListSyncDone(PspGeList list);
	 bool hleGeListSync(PspGeList list, int syncType);
}