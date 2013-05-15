/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r845)
 */

#pragma once

#pragma pack(push,1)
struct SceKernelCallbackInfo_t {
	int size;
    char name[32];
	int threadId;
	int callback_addr;
	int callback_arg_addr;
	int notifyCount;
	int notifyArg2;
};

#pragma pack(pop)

class SceKernelCallbackInfo : public SceKernelCallbackInfo_t {
public:
	SceKernelCallbackInfo(const string& name, int threadId, int callback_addr, int callback_arg_addr);
	void write(int address);
	void startContext(pspRegisters cpuContext/*, SceKernelThreadInfo *thread*/); 
    int uid;
	int notifyArg1;//that shouldn't be on the struct to avoid false write
};