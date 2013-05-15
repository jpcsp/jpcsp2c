/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r913)
 */




// TODO are module/thread attr interchangeable? (probably yes)
#define PSP_THREAD_ATTR_USER       0x80000000
#define PSP_THREAD_ATTR_USBWLAN    0xa0000000
#define PSP_THREAD_ATTR_VSH        0xc0000000
#define PSP_THREAD_ATTR_KERNEL     0x00001000
#define PSP_THREAD_ATTR_VFPU          0x00004000
#define PSP_THREAD_ATTR_SCRATCH_SRAM  0x00008000
#define PSP_THREAD_ATTR_NO_FILLSTACK  0x00100000 // Disables filling the stack with 0xFF on creation.
#define PSP_THREAD_ATTR_CLEAR_STACK   0x00200000 // Clear the stack when the thread is deleted.

    // PspThreadStatus
#define PSP_THREAD_RUNNING   0x00000001
#define PSP_THREAD_READY     0x00000002
#define PSP_THREAD_WAITING   0x00000004
#define PSP_THREAD_SUSPEND   0x00000008
#define PSP_THREAD_STOPPED   0x00000010
#define PSP_THREAD_KILLED    0x00000020

// callbacks, only 1 of each type can be registered per thread
#define THREAD_CALLBACK_UMD          0
#define THREAD_CALLBACK_IO           1
#define THREAD_CALLBACK_GE_SIGNAL    2
#define THREAD_CALLBACK_GE_FINISH    3
#define THREAD_CALLBACK_MEMORYSTICK  4
#define THREAD_CALLBACK_SIZE         5

#pragma pack(push,1) // i think c++ screw structure
struct SceKernelThreadInfo_t {
	int size; 
    char name[32];
	int attr;
	int status;
	int entry_addr;
	int stack_addr;
	int stackSize;
	int gpReg_addr;
	int initPriority;
	int currentPriority;
	int waitType; // 3 = sema
	int waitId;
	int wakeupCount;
	int exitStatus;
	u64 runClocks;
	int intrPreemptCount;
	int threadPreemptCount;
	int releaseCount;
};

#pragma pack(pop)

class SceKernelThreadInfo : public SceKernelThreadInfo_t {
public:
	SceKernelThreadInfo(const string& name, int entry_addr, int initPriority, int stackSize, int attr);

	void saveContext();
	void restoreContext();

	void write(u32 addr);

	pspRegisters cpuContext;
    u32 uid;
	bool do_delete;
	bool do_callbacks;
	ThreadWaitInfo wait;
	bool callbackRegistered[THREAD_CALLBACK_SIZE];
	bool callbackReady[THREAD_CALLBACK_SIZE];
	SceKernelCallbackInfo* callbackInfo[THREAD_CALLBACK_SIZE];
};