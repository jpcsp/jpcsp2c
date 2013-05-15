#include "Common.h"
#include "SyscallHandler.h"
#include "syscallsFirm15.h"
#include "HLE/Modules.h"
#include "Memory.h"
#include "Video.h"
#include "HLE/Kernel/Types/ThreadWaitInfo.h"
#include "HLE/Kernel/Types/SceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/SceKernelThreadInfo.h"
#include "HLE/Kernel/Types/PspGeList.h"
#include "pspge.h"
#include "ThreadMan.h"
#include "pspdisplay.h"
#include "HLE/modules/HLEModuleFunction.h"
#include "HLE/modules/HLEThread.h"
#include "HLE/modules/HLEModuleManager.h"

void SyscallHandler::syscall(int code)
{
        
        ThreadMan::clearSyscallFreeCycles();

        // Some syscalls implementation throw GeneralJpcspException,
        // and Processor isn't setup to catch exceptions so we'll do it
        // here for now, or we could just stop throwing exceptions.
        // Also we need to decide whether to pass arguments to the functions,
        // or let them read the registers they want themselves.
        
            // Currently using FW1.50 codes
            switch(code) {
                case 0x200d:
                   ThreadMan::sceKernelCreateCallback(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                /*case 0x2012:
                    ThreadMan::sceKernelCheckCallback();
                    break;*/
                case 0x2014:
                    ThreadMan::sceKernelSleepThread();
                    break;
                case 0x2015:
                     ThreadMan::sceKernelSleepThreadCB();
                    break;
               /* case 0x2016:
                    ThreadMan.getInstance().ThreadMan_sceKernelWakeupThread(pspRegs.pspRegs.GPR[4]);
                    break;
                case 0x201a:
                    ThreadMan.getInstance().ThreadMan_sceKernelWaitThreadEnd(pspRegs.pspRegs.GPR[4], pspRegs.pspRegs.GPR[5]);
                    break;*/
                case 0x201c:
                    ThreadMan::sceKernelDelayThread(pspRegs.GPR[4]);
                    break;
                /*case 0x201d:
                    ThreadMan.getInstance().ThreadMan_sceKernelDelayThreadCB(pspRegs.pspRegs.GPR[4]);
                    break;

                case 0x2020:
                    ThreadMan.getInstance().ThreadMan_sceKernelCreateSema(pspRegs.pspRegs.GPR[4], pspRegs.pspRegs.GPR[5], pspRegs.pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8]);
                    break;
                case 0x2021:
                    ThreadMan.getInstance().ThreadMan_sceKernelDeleteSema(pspRegs.GPR[4]);
                    break;
                case 0x2022:
                    ThreadMan.getInstance().ThreadMan_sceKernelSignalSema(pspRegs.pspRegs.GPR[4], pspRegs.pspRegs.GPR[5]);
                    break;
                case 0x2023:
                    ThreadMan.getInstance().ThreadMan_sceKernelWaitSema(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2024:
                    ThreadMan.getInstance().ThreadMan_sceKernelWaitSemaCB(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2025:
                    ThreadMan.getInstance().ThreadMan_sceKernelPollSema(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2026:
                    ThreadMan.getInstance().ThreadMan_sceKernelCancelSema(pspRegs.GPR[4]); // not in pspsdk, params guessed
                    break;
                case 0x2027:
                    ThreadMan.getInstance().ThreadMan_sceKernelReferSemaStatus(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;

                case 0x2028:
                    ThreadMan.getInstance().ThreadMan_sceKernelCreateEventFlag(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7]);
                    break;
                case 0x2029:
                    ThreadMan.getInstance().ThreadMan_sceKernelDeleteEventFlag(pspRegs.GPR[4]);
                    break;
                case 0x202a:
                    ThreadMan.getInstance().ThreadMan_sceKernelSetEventFlag(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x202b:
                    ThreadMan.getInstance().ThreadMan_sceKernelClearEventFlag(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x202c:
                    ThreadMan.getInstance().ThreadMan_sceKernelWaitEventFlag(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8]);
                    break;
                case 0x202d:
                    ThreadMan.getInstance().ThreadMan_sceKernelWaitEventFlagCB(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8]);
                    break;
                case 0x202e:
                    ThreadMan.getInstance().ThreadMan_sceKernelPollEventFlag(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7]);
                    break;
                case 0x202f:
                    ThreadMan.getInstance().ThreadMan_sceKernelCancelEventFlag(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]); // not in pspsdk, params guessed
                    break;
                case 0x2030:
                    ThreadMan.getInstance().ThreadMan_sceKernelReferEventFlagStatus(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2043:
                    Managers.vpl.sceKernelCreateVpl(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8]);
                    break;
                case 0x2044:
                    Managers.vpl.sceKernelDeleteVpl(pspRegs.GPR[4]);
                    break;
                case 0x2045:
                    Managers.vpl.sceKernelAllocateVpl(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[8]);
                    break;
                case 0x2046:
                    Managers.vpl.sceKernelAllocateVplCB(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[8]);
                    break;
                case 0x2047:
                    Managers.vpl.sceKernelTryAllocateVpl(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[8]);
                    break;
                case 0x2048:
                    Managers.vpl.sceKernelFreeVpl(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2049:
                    Managers.vpl.sceKernelCancelVpl(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x204a:
                    Managers.vpl.sceKernelReferVplStatus(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x204b:
                    Managers.fpl.sceKernelCreateFpl(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8], pspRegs.GPR[9]);
                    break;
                case 0x204c:
                    Managers.fpl.sceKernelDeleteFpl(pspRegs.GPR[4]);
                    break;
                case 0x204d:
                    Managers.fpl.sceKernelAllocateFpl(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x204e:
                    Managers.fpl.sceKernelAllocateFplCB(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x204f:
                    Managers.fpl.sceKernelTryAllocateFpl(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2050:
                    Managers.fpl.sceKernelFreeFpl(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2051:
                    Managers.fpl.sceKernelCancelFpl(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2052:
                    Managers.fpl.sceKernelReferFplStatus(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2054:
                    Managers.systime.sceKernelUSec2SysClock(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2056:
                    Managers.systime.sceKernelSysClock2USec(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2058:
                    Managers.systime.sceKernelGetSystemTime(pspRegs.GPR[4]);
                    break;
                case 0x2059:
                    Managers.systime.sceKernelGetSystemTimeWide();
                    break;
                case 0x205a:
                    Managers.systime.sceKernelGetSystemTimeLow();
                    break;*/
                case 0x206d:
					ThreadMan::sceKernelCreateThread(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8], pspRegs.GPR[9]);
                    break;
               /* case 0x206e:
					ThreadMan::sceKernelDeleteThread(pspRegs.GPR[4]);
                    break;*/
                case 0x206f:
					ThreadMan::sceKernelStartThread(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2070:
                case 0x2071:
					ThreadMan::sceKernelExitThread(pspRegs.GPR[4]);
                    break;/*
                case 0x2072:
                    ThreadMan.getInstance().ThreadMan_sceKernelExitDeleteThread(pspRegs.GPR[4]);
                    break;
                case 0x2073:
                    ThreadMan.getInstance().ThreadMan_sceKernelTerminateThread(pspRegs.GPR[4]);
                    break;
                case 0x2074:
                    ThreadMan.getInstance().ThreadMan_sceKernelTerminateDeleteThread(pspRegs.GPR[4]);
                    break;
                case 0x2077:
                    ThreadMan.getInstance().ThreadMan_sceKernelChangeCurrentThreadAttr(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2078:
                    ThreadMan.getInstance().ThreadMan_sceKernelChangeThreadPriority(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;*/
                case 0x207b:
                    ThreadMan::sceKernelGetThreadId();
                    break;
                /*case 0x207c:
                    ThreadMan.getInstance().ThreadMan_sceKernelGetThreadCurrentPriority();
                    break;
                case 0x207d:
                    ThreadMan.getInstance().ThreadMan_sceKernelGetThreadExitStatus(pspRegs.GPR[4]);
                    break;
                case 0x207e:
                    ThreadMan.getInstance().ThreadMan_sceKernelCheckThreadStack();
                    break;
                case 0x207f:
                    ThreadMan.getInstance().ThreadMan_sceKernelGetThreadStackFreeSize(pspRegs.GPR[4]);
                    break;*/
                case 0x2080:
                    ThreadMan::sceKernelReferThreadStatus(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                /*case 0x2083:
                    ThreadMan.getInstance().ThreadMan_sceKernelGetThreadmanIdList(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7]);
                    break;
                case 0x2087:
                    pspiofilemgr.getInstance().sceIoPollAsync(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2088:
                    pspiofilemgr.getInstance().sceIoWaitAsync(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x2089:
                    pspiofilemgr.getInstance().sceIoWaitAsyncCB(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x208a:
                    pspiofilemgr.getInstance().sceIoGetAsyncStat(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x208d:
                    pspiofilemgr.getInstance().sceIoClose(pspRegs.GPR[4]);
                    break;
                case 0x208e:
                    pspiofilemgr.getInstance().sceIoCloseAsync(pspRegs.GPR[4]);
                    break;
                case 0x208f:
                    pspiofilemgr.getInstance().sceIoOpen(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2090:
                    pspiofilemgr.getInstance().sceIoOpenAsync(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2091:
                    pspiofilemgr.getInstance().sceIoRead(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2092:
                    pspiofilemgr.getInstance().sceIoReadAsync(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2093:
                    pspiofilemgr.getInstance().sceIoWrite(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;
                case 0x2094:
                    pspiofilemgr.getInstance().sceIoWriteAsync(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;

                case 0x2095:
                    pspiofilemgr.getInstance().sceIoLseek(
                            pspRegs.GPR[4],
                            ((((long)pspRegs.GPR[6]) & 0xFFFFFFFFL) | (((long)pspRegs.GPR[7])<<32)),
                            pspRegs.GPR[8]);
                    break;
                case 0x2096:
                    pspiofilemgr.getInstance().sceIoLseekAsync(
                            pspRegs.GPR[4],
                            ((((long)pspRegs.GPR[6]) & 0xFFFFFFFFL) | (((long)pspRegs.GPR[7])<<32)),
                            pspRegs.GPR[8]);
                    break;

                case 0x2097:
                    pspiofilemgr.getInstance().sceIoLseek32(
                            pspRegs.GPR[4],
                            pspRegs.GPR[5],
                            pspRegs.GPR[6]);
                    break;
                case 0x2098:
                    pspiofilemgr.getInstance().sceIoLseek32Async(
                            pspRegs.GPR[4],
                            pspRegs.GPR[5],
                            pspRegs.GPR[6]);
                    break;

            case 0x209b:
                    pspiofilemgr.getInstance().sceIoDopen(pspRegs.GPR[4]);
                    break;
            case 0x209c:
                    pspiofilemgr.getInstance().sceIoDread(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
            case 0x209d:
                    pspiofilemgr.getInstance().sceIoDclose(pspRegs.GPR[4]);
                    break;
            case 0x209f:
                    pspiofilemgr.getInstance().sceIoMkdir(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20a1:
                    pspiofilemgr.getInstance().sceIoChdir(pspRegs.GPR[4]);
                    break;
                case 0x20a2:
                    pspiofilemgr.getInstance().sceIoSync(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20a3:
                    pspiofilemgr.getInstance().sceIoGetstat(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20a6:
                    pspiofilemgr.getInstance().sceIoDevctl(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8], pspRegs.GPR[9]);
                    break;
                case 0x20a8:
                    pspiofilemgr.getInstance().sceIoAssign(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7], pspRegs.GPR[8], pspRegs.GPR[9]);
                    break;
                case 0x20b5:
                    psputils.getInstance().sceKernelDcacheInvalidateRange(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;*/
                case 0x20bf:
					psputils::sceKernelUtilsMt19937Init(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20c0:
					psputils::sceKernelUtilsMt19937UInt(pspRegs.GPR[4]);
                    break;
               /* case 0x20c1:
                    psputils.getInstance().sceKernelGetGPI();
                    break;
                case 0x20c2:
                    psputils.getInstance().sceKernelSetGPO(pspRegs.GPR[4]);
                    break;
                case 0x20c3:
                    psputils.getInstance().sceKernelLibcClock();
                    break;
                case 0x20c4:
                    psputils.getInstance().sceKernelLibcTime(pspRegs.GPR[4]);
                    break;
                case 0x20c5:
                    psputils.getInstance().sceKernelLibcGettimeofday(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20c6:
                    psputils.getInstance().sceKernelDcacheWritebackAll();
                    break;
                case 0x20c7:
                    psputils.getInstance().sceKernelDcacheWritebackInvalidateAll();
                    break;
                case 0x20c8:
                    psputils.getInstance().sceKernelDcacheWritebackRange(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20c9:
                    psputils.getInstance().sceKernelDcacheWritebackInvalidateRange(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;*/
                case 0x20dc:
                    pspSysMem::sceKernelMaxFreeMemSize();
                    break;
                case 0x20dd:
                    pspSysMem::sceKernelTotalFreeMemSize();
                    break;
                case 0x20de:
                    pspSysMem::sceKernelAllocPartitionMemory(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7],pspRegs.GPR[8]);
                    break;
                case 0x20df:
					pspSysMem::sceKernelFreePartitionMemory(pspRegs.GPR[4]);
                    break;
                case 0x20e0:
					pspSysMem::sceKernelGetBlockHeadAddr(pspRegs.GPR[4]);
                    break;
                case 0x20e2:
					pspSysMem::sceKernelDevkitVersion();
                    break;
               /* case 0x20e9:
                    LoadExec.getInstance().sceKernelLoadExec(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
                case 0x20eb:
                    LoadExec.getInstance().sceKernelExitGame();
                    break;
                case 0x20ec:
                    LoadExec.getInstance().sceKernelRegisterExitCallback(pspRegs.GPR[4]);
                    break;*/
               case 0x20ef:
				   pspge::sceGeEdramGetSize();
                   break;
               case 0x20f0:
				   pspge::sceGeEdramGetAddr();
                   break;
               case 0x20f6:
				   pspge::sceGeListEnQueue(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7]);
                    break;
               /* case 0x20f8:
                    pspge.getInstance().sceGeListDeQueue(pspRegs.GPR[4]);
                    break;*/
                case 0x20f9:
					pspge::sceGeListUpdateStallAddr(pspRegs.GPR[4], pspRegs.GPR[5]);
                    break;
               /* case 0x20fb:
                    pspge.getInstance().sceGeDrawSync(pspRegs.GPR[4]);
                    break;*/
                case 0x213a://TODO
                   // pspdisplay.getInstance().sceDisplaySetMode(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
					pspRegs.GPR[2] = video::sceSetDisplayMode(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6]);
                    break;

                case 0x213f://TODO
                    //pspdisplay.getInstance().sceDisplaySetFrameBuf(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7]);
					pspRegs.GPR[2] = video::sceDisplaySetFrameBuf(pspRegs.GPR[4], pspRegs.GPR[5],(PspDisplayPixelFormat)pspRegs.GPR[6],(PspDisplaySetBufSync)pspRegs.GPR[7]);
                    break;
                /*case 0x2140:
                    pspdisplay.getInstance().sceDisplayGetFrameBuf(pspRegs.GPR[4], pspRegs.GPR[5], pspRegs.GPR[6], pspRegs.GPR[7]);
                    break;
                case 0x2143:
                    pspdisplay.getInstance().sceDisplayGetVcount();
                    break;
                case 0x2145:
                    pspdisplay.getInstance().sceDisplayWaitVblank();
                    break;
                case 0x2146:
                    pspdisplay.getInstance().sceDisplayWaitVblankCB();
                    break;*/
                case 0x2147:
					pspdisplay::sceDisplayWaitVblankStart();
                    break;
               /* case 0x2148:
                    pspdisplay.getInstance().sceDisplayWaitVblankStartCB();
                    break;
                case 0x2149:
                    pspdisplay.getInstance().sceDisplayGetCurrentHcount();
                    break;
                case 0x214a:
                    pspdisplay.getInstance().sceDisplayGetAccumulatedHcount();
                    break;*/
////////////DUMMY FUNCTIONS DOESN"T EXIST HERE ON JPCSP
				case 0x2150: // sceCtrlPeekBufferPositive  //DUMMY to be removed!!!
					{
						u32 i;
						u32 addr = pspRegs.GPR[4];
						for (i = 0; i < pspRegs.GPR[5]; ++i) {
							Memory::write32(addr, 0);
							Memory::write32(addr + 4, 0);
							Memory::write8(addr + 8, 0);
							Memory::write8(addr + 9, 0);
							addr += 16;
						}
						pspRegs.GPR[2] = i;
					}
					break;
				 case 0x2152://sceCtrlReadBufferPositive
					{
					u32 i;
					u32 addr= pspRegs.GPR[4];
					for (i = 0; i < pspRegs.GPR[5]; ++i) {
							Memory::write32(addr, 0);
							Memory::write32(addr + 4, 0);
							Memory::write8(addr + 8, 0);
							Memory::write8(addr + 9, 0);
							addr += 16;
					}
					pspRegs.GPR[2] = i;
					}
					break;
				case 0x214C://sceCtrlSetSamplingCycle
					pspRegs.GPR[2]=0;
					break;
				case 0x214E://sceCtrlSetSamplingMode
					pspRegs.GPR[2]=0;
					break;
				/*case 0x20b2:  //sceKernelStdin
					pspRegs.GPR[2]=3;
					break;
				case 0x20b3:  //sceKernelStdout 
					pspRegs.GPR[2]=1;
					break;
				case 0x20b4:  //sceKernelStderr 
					pspRegs.GPR[2]=2;
					break;*/
////////////DUMMY FUNCTIONS DOESN"T EXIST HERE ON JPCSP ENDOF
                case 0xfffff: // special code for unmapped imports
					Modules::log.error("Unmapped import @ 0x%08X", pspRegs.pc);
                    //Emulator.PauseEmu();
                    break;
                default:
                {
                    // Try and handle as an HLE module export
					bool handled = HLEModuleManager::handleSyscall(code);
                    if (!handled) {

                        // At least set a decent return value
                         pspRegs.GPR[2] = 0;
                       

						for (int i = 0; i < sizeof(calls); i++)
						{
							 if(calls[i].syscall ==code)
							 {
								Modules::log.warn("Syscall %x  %s not supported  %08x %08x %08x",code,calls[i].name,pspRegs.GPR[4],pspRegs.GPR[5], pspRegs.GPR[6]);
								return;
							 }
						}
                        Modules::log.warn("Unsupported syscall %x %08x %08x %08x",code,pspRegs.GPR[4],pspRegs.GPR[5],pspRegs.GPR[6]);					
                    }
                }
                break;
            }
}

