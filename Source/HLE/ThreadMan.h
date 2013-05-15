/*
 * 
 *  01/5/2011 - synced with 913
 *               (actual change was in r913)
 */

namespace ThreadMan {

	typedef map<u32, SceKernelThreadInfo *> ThreadMap;
	typedef list<SceKernelThreadInfo *> ThreadList;
	typedef map<u32, SceKernelCallbackInfo *> CallbackMap;


    void InitialiseThreadman(int entry_addr, int attr, const char* pspfilename);
	void step();
	int mallocStack(int size);

	// Syscalls
	void sceKernelCreateThread(u32 name_addr,u32 entry_addr,u32 initPriority,u32 stackSize,u32 attr,u32 option_addr);
	void sceKernelStartThread(u32 uid,u32 len,u32 data_addr);
	void sceKernelGetThreadId();
	void sceKernelGetThreadCurrentPriority();
	void sceKernelReferThreadStatus(u32 uid,u32 addr);
	void sceKernelSleepThread();
	void sceKernelSleepThreadCB();
	void sceKernelDelayThread(u32 micros);
	void sceKernelDelayThreadCB(u32 micros);
	void sceKernelCreateCallback(u32 name_addr,u32 func_addr,u32 user_arg_addr); 
	void sceKernelExitThread(int exitStatus);
	void sceKernelTerminateDeleteThread(int uid);
	void sceKernelTerminateThread(int uid);
	void sceKernelExitDeleteThread(int exitStatus);

	// Helper functions
	SceKernelCallbackInfo* hleKernelCreateCallback(const string &name, u32 func_addr, u32 user_arg_addr);
	SceKernelThreadInfo* hleKernelCreateThread(const string &name, u32 entry_addr, u32 initPriority, u32 stackSize, u32 attr, u32 option_addr);
	void hleKernelStartThread(SceKernelThreadInfo *thread, int userDataLength, int userDataAddr, int gp);
	void hleKernelSleepThread(bool do_callbacks);
	void hleKernelThreadWait(ThreadWaitInfo &wait, int micros, bool forever);
	void hleKernelDelayThread(int micros, bool do_callbacks);

	void changeThreadState(SceKernelThreadInfo *thread, int newStatus);
	void onThreadStopped(SceKernelThreadInfo *stoppedThread);
	void onWaitTimeout(SceKernelThreadInfo *thread);
	void contextSwitch(SceKernelThreadInfo *newthread);
	void deleteThread(SceKernelThreadInfo *thread);
	SceKernelThreadInfo* nextThread();
	void yieldCurrentThread();
	void yieldCurrentThreadCB();
	void checkCallbacks();
	bool checkThreadCallbacks(SceKernelThreadInfo* thread);
	
	bool isIdleThread(SceKernelThreadInfo *thread);
	
	void clearSyscallFreeCycles(); 
    void exit();
	void install_idle_threads();
	int getCurrentThreadID();
	SceKernelThreadInfo* getCurrentThread();
	string getThreadName(int uid);
	void blockCurrentThread();
    void unblockThread();
    string getCallingFunction();
	void setToBeDeletedThread(SceKernelThreadInfo *thread) ;
	bool checkThreadID(int uid);
}