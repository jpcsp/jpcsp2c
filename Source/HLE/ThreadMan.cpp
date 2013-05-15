/*
 * 
 *  01/5/2011 - synced with 913
 *               (actual change was in r913)
 */


#include "Common.h"
#include "Memory.h"
#include <algorithm>

#include "HLE/Modules.h"
#include "HLE/Kernel/Types/ThreadWaitInfo.h"
#include "HLE/Kernel/Types/SceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/SceKernelThreadInfo.h"
#include "ThreadMan.h"

#include "HLE/Kernel/Managers/SceUidManager.h"
#include "HLE/Kernel/Types/SceKernelErrors.h"
#define WDT_THREAD_IDLE_CYCLES 1000000
#define WDT_THREAD_HOG_CYCLES ((0x0A000000 - 0x08400000) * 3)



SceKernelThreadInfo *current_thread, *idle0, *idle1;
ThreadMan::ThreadMap threadMap;
ThreadMan::ThreadList waitingThreads;
ThreadMan::ThreadList toBeDeletedThreads;
ThreadMan::CallbackMap callbackMap;

int continuousIdleCycles; // watch dog timer - number of continuous cycles in any idle thread
int syscallFreeCycles; // watch dog timer - number of cycles since last syscall
bool insideCallback;

bool USE_THREAD_BANLIST = false;
bool LOG_CONTEXT_SWITCHING = false;
bool IGNORE_DELAY = false;

    // see sceKernelGetThreadmanIdList
#define SCE_KERNEL_TMID_Thread              1
#define SCE_KERNEL_TMID_Semaphore           2
#define SCE_KERNEL_TMID_EventFlag           3
#define SCE_KERNEL_TMID_Mbox                4
#define SCE_KERNEL_TMID_Vpl                 5
#define SCE_KERNEL_TMID_Fpl                 6
#define SCE_KERNEL_TMID_Mpipe               7
#define SCE_KERNEL_TMID_Callback            8
#define SCE_KERNEL_TMID_ThreadEventHandler  9
#define SCE_KERNEL_TMID_Alarm               10
#define SCE_KERNEL_TMID_VTimer              11
#define SCE_KERNEL_TMID_SleepThread         64
#define SCE_KERNEL_TMID_DelayThread         65
#define SCE_KERNEL_TMID_SuspendThread       66
#define SCE_KERNEL_TMID_DormantThread       67

/*
    //TODO!
    public Iterator<SceKernelThreadInfo> iterator() {
        return threadMap.values().iterator();
    }

    public Iterator<SceKernelThreadInfo> iteratorByPriority() {
        Collection<SceKernelThreadInfo> c = threadMap.values();
        List<SceKernelThreadInfo> list = new LinkedList<SceKernelThreadInfo>(c);
        Collections.sort(list, idle0); // We need an instance of SceKernelThreadInfo for the comparator, so we use idle0
        return list.iterator();
    } */       
extern u64 emuReferenceTime; //TODO WIP timer it might work might not..

/** call this when resetting the emulator
* @param entry_addr entry from ELF header
* @param attr from sceModuleInfo ELF section header */
void ThreadMan::InitialiseThreadman(int entry_addr, int attr, const char* pspfilename)
{
	 //statistics = new Statistics();
	insideCallback=false;
	install_idle_threads();

	// Create a thread the program will run inside
	current_thread = new SceKernelThreadInfo("root", entry_addr, 0x20, 0x4000, attr);
	threadMap.insert(make_pair(current_thread->uid, current_thread));

	// Set user mode bit if kernel mode bit is not present
    if ((current_thread->attr & PSP_THREAD_ATTR_KERNEL) != PSP_THREAD_ATTR_KERNEL) {
        current_thread->attr |= PSP_THREAD_ATTR_USER;
    }
	// Setup args by copying them onto the stack
	int len = strlen(pspfilename);
	int address = current_thread->cpuContext.GPR[29];
	Utilities::writeString(address, pspfilename);
	current_thread->cpuContext.GPR[4] = len + 1; // a0 = len + string terminator
	current_thread->cpuContext.GPR[5] = address; // a1 = pointer to arg data in stack
	
	current_thread->status = PSP_THREAD_READY;

    // Switch in the thread
    
	current_thread->status = PSP_THREAD_RUNNING;
	current_thread->restoreContext();
	syscallFreeCycles = 0;
}

void ThreadMan::install_idle_threads() {
	// Generate 2 idle threads which can toggle between each other when there are no ready threads

	u8 idleCode[] = {0x00, 0x00, 0x04, 0x24, // addiu a0, zr, 0
					 0x00, 0x08, 0x1f, 0x3c, // lui ra, 0x08000000
					 0x08, 0x00, 0xe0, 0x03, // jr ra
					 0x0c, 0x07, 0x08, 0x00}; // syscall 0x0201c [sceKernelDelayThread]
	// TODO
	//pspSysMem.getInstance().malloc(1, pspSysMem.PSP_SMEM_Addr, 16, MemoryMap.START_RAM);

	memcpy(Memory::getPointer(0x08000000), idleCode, sizeof(idleCode));

	idle0 = new SceKernelThreadInfo("idle0", 0x08000000 | 0x80000000, 0x7f, 0x0, PSP_THREAD_ATTR_KERNEL);
	idle0->status = PSP_THREAD_READY;
	threadMap.insert(make_pair(idle0->uid, idle0));

	idle1 = new SceKernelThreadInfo("idle1", 0x08000000 | 0x80000000, 0x7f, 0x0, PSP_THREAD_ATTR_KERNEL);
	idle1->status = PSP_THREAD_READY;
	threadMap.insert(make_pair(idle1->uid, idle1));

	continuousIdleCycles = 0;
}
/*to be called when exiting the emulation*/
void ThreadMan::exit()
{
   //TODO!
}

/** to be called from the main emulation loop */
void ThreadMan::step()
{
	if (current_thread != NULL) {
		current_thread->runClocks++;

		
		// Hook jr ra to 0 (thread function returned)
		if (pspRegs.pc == 0 && pspRegs.GPR[31] == 0) {
			if (insideCallback) {
				// Callback has exited
				Modules::log.debug("Callback exit detected");
				current_thread->restoreContext(); // also sets pc = npc
				// keep processing callbacks
				checkCallbacks();
			} 
			else 
			{
				// Thread has exited
				Modules::log.debug("Thread exit detected sceUID= %x name: %s return: %x ", current_thread->uid, current_thread->name , pspRegs.GPR[2]);
				current_thread->exitStatus = pspRegs.GPR[2]; // v0
				changeThreadState(current_thread, PSP_THREAD_STOPPED);
				contextSwitch(nextThread());
			}
		}

		// Watch dog timer
		if (isIdleThread(current_thread)) {
			continuousIdleCycles++;
			if (continuousIdleCycles > WDT_THREAD_IDLE_CYCLES) {
				Modules::log.info("Watch dog timer - pausing emulator (idle)");
//TODO				//Emulator.PauseEmuWithStatus(Emulator.EMU_STATUS_WDT_IDLE); 
			}
		} else {
			continuousIdleCycles = 0;
			syscallFreeCycles++;
			if (syscallFreeCycles > WDT_THREAD_HOG_CYCLES) {
				Modules::log.info("Watch dog timer - pausing emulator (thread hogging)");
//TODO				Emulator.PauseEmuWithStatus(Emulator.EMU_STATUS_WDT_HOG);

			}
		}
	} else {
		// We always need to be in a thread! we shouldn't get here.

		Modules::log.error("No ready threads!");
	}
	if (!waitingThreads.empty()) {
		u64 microTimeNow = emuReferenceTime; //Emulator.getClock().microTime();
		// Access waitingThreads using array indexing because
		// this is more efficient than iterator access for short lists
		for (ThreadList::iterator it = waitingThreads.begin(); it != waitingThreads.end(); ) {
			SceKernelThreadInfo *thread = *it;
			++it;
			if (!thread->wait.forever && microTimeNow >= thread->wait.microTimeTimeout) {
				onWaitTimeout(thread);
				changeThreadState(thread, PSP_THREAD_READY);
			}
		}
	}
	// Cleanup stopped threads (deferred deletion)
	for (ThreadList::iterator it = toBeDeletedThreads.begin(); it != toBeDeletedThreads.end(); ) {
		SceKernelThreadInfo *thread = *it;
		++it;
		// this check shouldn't be necessary anymore, can be inferred by the thread existing in the toBeDeletedThreads list
		if (thread->do_delete) {
			deleteThread(thread);
		} else {
			Modules::log.warn("thread: %s in toBeDeletedThreads list with do_delete = false",thread->name);
		}
	}
}
    /** Part of watch dog timer */
void ThreadMan::clearSyscallFreeCycles() 
{
        syscallFreeCycles = 0;
}

   /** @param newthread The thread to switch in. */
void ThreadMan::contextSwitch(SceKernelThreadInfo *newthread) {
        if (insideCallback) {
			Modules::log.warn("contextSwitch called from inside callback");
//TODO better?	//Modules.log.warn("contextSwitch called from inside callback. caller:" + getCallingFunction());
                //Emulator.PauseEmu();
        }

        if (current_thread != NULL) {
            // Switch out old thread
            if (current_thread->status == PSP_THREAD_RUNNING) {
                changeThreadState(current_thread, PSP_THREAD_READY);
            }

            // save registers
            current_thread->saveContext();

            /*
            Modules.log.debug("saveContext SceUID=" + Integer.toHexString(current_thread.uid)
                + " name:" + current_thread.name
                + " PC:" + Integer.toHexString(current_thread.pcreg)
                + " NPC:" + Integer.toHexString(current_thread.npcreg));
            */
        }

        if (newthread != NULL) {
            // Switch in new thread
            changeThreadState(newthread, PSP_THREAD_RUNNING);
            newthread->wakeupCount++; // check
            // restore registers
            newthread->restoreContext();

            if (LOG_CONTEXT_SWITCHING && !isIdleThread(newthread)) {
               
 //TODO               Modules::log.debug("---------------------------------------- SceUID=" + Integer.toHexString(newthread.uid) + " name:'" + newthread.name + "'");
                /*
                Modules.log.debug("restoreContext SceUID=" + Integer.toHexString(newthread.uid)
                    + " name:" + newthread.name
                    + " PC:" + Integer.toHexString(newthread.cpuContext.pc)
                    + " NPC:" + Integer.toHexString(newthread.cpuContext.npc));
                */
            }

        } else {
            // Shouldn't get here now we are using idle threads
			Modules::log.info("No ready threads - pausing emulator");
//TODO	    Emulator.PauseEmuWithStatus(Emulator.EMU_STATUS_UNKNOWN);
        }

        current_thread = newthread;
        syscallFreeCycles = 0;

//TODO when we gonna have filemanager    //   pspiofilemgr.getInstance().onContextSwitch();
		//RuntimeContext.update();

}

bool compare (SceKernelThreadInfo *thread1,SceKernelThreadInfo *thread2)
{
	if(thread1->currentPriority < thread2->currentPriority) return true;
	else if(thread1->currentPriority > thread2->currentPriority) return false;
	else return false;

}
/** This function must have the property of never returning current_thread,
* unless current_thread is already null.
* @return The next thread to schedule (based on thread priorities). */
SceKernelThreadInfo* ThreadMan::nextThread() {  
	//TODO check if that is okay
	SceKernelThreadInfo *next = NULL;
    list<SceKernelThreadInfo *> ReadyList;
	// Find the thread with status PSP_THREAD_READY and the highest priority
	// In this implementation low priority threads can get starved
	for(ThreadMap::iterator it = threadMap.begin(); it != threadMap.end(); ++it) {
		SceKernelThreadInfo *thread = it->second;
		//Modules::log.debug("name: %s status %x currpri =%x",thread->name,thread->status,thread->currentPriority);
		if(thread !=current_thread && thread->status == PSP_THREAD_READY)
		{
			ReadyList.push_back(thread);
			
		}
		//JPCSP way i think it's okay....
		/*if (thread != current_thread &&	thread->status == PSP_THREAD_READY &&
			(next == NULL || next->currentPriority < thread->currentPriority)) {
			next = thread;
			break;
		}*/
	}
	stable_sort(ReadyList.begin(),ReadyList.end(),compare);
	
	for(list<SceKernelThreadInfo *>::iterator it = ReadyList.begin(); it != ReadyList.end(); ++it) {
    	SceKernelThreadInfo *thread = *it;
		//Modules::log.debug("name: %s status %x currpri =%x",thread->name,thread->status,thread->currentPriority);
		next=thread;
		break;
	}

	return next;
}
int ThreadMan::getCurrentThreadID()
{
	if(current_thread==NULL)
	{
		return -1;
	}
	return current_thread->uid;
}
SceKernelThreadInfo* ThreadMan::getCurrentThread()
{
	return current_thread;
}
bool ThreadMan::isIdleThread(SceKernelThreadInfo *thread) {
    return thread == idle0 || thread == idle1;
}
string ThreadMan::getThreadName(int uid)
{
    /*       SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            return "NOT A THREAD";
        } else {
            return thread.name;
        }*/
	return "";
}

 /** Moves the current thread from running to ready and switches in the next
* ready thread (which may be an idle thread if no other threads are ready). */
void ThreadMan::yieldCurrentThread()
{

	if(LOG_CONTEXT_SWITCHING &&  !isIdleThread(current_thread))
		Modules::log.debug("-------------------- yield SceUID= %x name: %s",current_thread->uid,current_thread->name);

	contextSwitch(nextThread());
}

/* The same as yieldCurrentThread, except combined with sceKernelCheckCallback */
void ThreadMan::yieldCurrentThreadCB()
{

	if(LOG_CONTEXT_SWITCHING && !isIdleThread(current_thread))
		Modules::log.debug("-------------------- yield CB SceUID= %x name: %s",current_thread->uid,current_thread->name);

	current_thread->do_callbacks = true;
	contextSwitch(nextThread());

	// The above context switch may have triggered an IO callback
	if (!insideCallback)
		checkCallbacks();
}
void ThreadMan::blockCurrentThread()
{
   //TODO
}
void ThreadMan::unblockThread()
{
  //TODO
}
string ThreadMan::getCallingFunction()
{
	//TODO
	return "";
}
/** Call this when a thread's wait timeout has expired.
* You can assume the calling function will set thread.status = ready. */
void ThreadMan::onWaitTimeout(SceKernelThreadInfo *thread) {
	// ThreadEnd
	if (thread->wait.waitingOnThreadEnd) {
		Modules::log.debug("ThreadEnd timedout");

		// Untrack
		thread->wait.waitingOnThreadEnd = false;

		// Return WAIT_TIMEOUT
		thread->cpuContext.GPR[2] = ERROR_WAIT_TIMEOUT;
	}

	// EventFlag
	else if (thread->wait.waitingOnEventFlag) {
		Modules::log.debug("EventFlag timedout");

		// Untrack
		thread->wait.waitingOnEventFlag = false;
//TODO!!!!
		// Update numWaitThreads
		/*SceKernelEventFlagInfo event = Managers.eventFlags.get(thread.wait.EventFlag_id);
		if (event != null) {
			event.numWaitThreads--;

			// Return WAIT_TIMEOUT
			thread.cpuContext.gpr[2] = ERROR_WAIT_TIMEOUT;
		} else {
			Modules.log.warn("EventFlag deleted while we were waiting for it! (timeout expired)");

			// Return WAIT_DELETE
			thread.cpuContext.gpr[2] = ERROR_WAIT_DELETE;
		}*/
	}

}

void ThreadMan::deleteThread(SceKernelThreadInfo *thread) {
	Modules::log.debug("really deleting thread: %s",thread->name);
	// cleanup thread - free the stack
	if (thread->stack_addr != 0) {
		Modules::log.debug("thread: %s freeing stack %x ",thread->name,thread->stack_addr);
		pspSysMem::free(thread->stack_addr);
	}

	waitingThreads.remove(thread);
	toBeDeletedThreads.remove(thread);
	threadMap.erase(thread->uid);
	SceUidManager::releaseUid(thread->uid, "ThreadMan-thread");
	//statistics.addThreadStatistics(thread);
}
void ThreadMan::setToBeDeletedThread(SceKernelThreadInfo *thread) 
{
	thread->do_delete=true;
	if(thread->status== PSP_THREAD_STOPPED)
	{
		toBeDeletedThreads.push_back(thread);
	}
}
/* Use this to change a thread's state (ready, running, etc)
* This function manages some lists such as waiting list and
* deferred deletion list. */

void ThreadMan::changeThreadState(SceKernelThreadInfo *thread, int newStatus)
{
	if (thread == NULL) {
		return;
	}

	if (thread->status == PSP_THREAD_WAITING) {
		waitingThreads.remove(thread);
		thread->do_callbacks = false;
	} else if (thread->status == PSP_THREAD_STOPPED) {
		toBeDeletedThreads.remove(thread);
	}

	thread->status = newStatus;

	if (thread->status == PSP_THREAD_WAITING) {
		if (!thread->wait.forever) {
			waitingThreads.push_back(thread);
		}
	} else if (thread->status == PSP_THREAD_STOPPED) {
		if (thread->do_delete) {
			toBeDeletedThreads.push_back(thread);
		}
		onThreadStopped(thread);
	}
}
void ThreadMan::onThreadStopped(SceKernelThreadInfo *stoppedThread) {
	for (ThreadMap::iterator it = threadMap.begin(); it != threadMap.end(); ++it) {
		SceKernelThreadInfo *thread = it->second;

		// Wakeup threads that are in sceKernelWaitThreadEnd
		// We're assuming if waitingOnThreadEnd is set then thread.status = waiting
		if (thread->wait.waitingOnThreadEnd && thread->wait.ThreadEnd_id == stoppedThread->uid) {
			// Untrack
			thread->wait.waitingOnThreadEnd = false;

			// Return success
			thread->cpuContext.GPR[2] = 0;

			// Wakeup
			changeThreadState(thread, PSP_THREAD_READY);
		}
	}
}
    /** Note: Some functions allow uid = 0 = current thread, others don't.
     * if uid = 0 then $v0 is set to ERROR_ILLEGAL_THREAD and false is returned
     * if uid < 0 then $v0 is set to ERROR_NOT_FOUND_THREAD and false is returned */
bool ThreadMan::checkThreadID(int uid) {
        if (uid == 0) {
			pspRegs.GPR[2] = ERROR_ILLEGAL_THREAD;
            return false;
        } else if (uid < 0) {
			pspRegs.GPR[2] = ERROR_NOT_FOUND_THREAD;
            return false;
        } else {
            return true;
        }
}
SceKernelThreadInfo* ThreadMan::hleKernelCreateThread(const string &name, u32 entry_addr, u32 initPriority, u32 stackSize, u32 attr, u32 option_addr)
{
	if (option_addr != 0)
		Modules::log.warn("hleKernelCreateThread unhandled SceKernelThreadOptParam");

	SceKernelThreadInfo *thread = new SceKernelThreadInfo(name, entry_addr, initPriority, stackSize, attr);
    threadMap.insert(make_pair(thread->uid, thread));

	Modules::log.debug("hleKernelCreateThread SceUID=%x name='%s' pc=%x attr=0x%x pri=0x%x", thread->uid, thread->name, thread->cpuContext.pc, attr, initPriority);
    return thread;
}
void ThreadMan::sceKernelCreateThread(u32 name_addr,u32 entry_addr,u32 initPriority,u32 stackSize,u32 attr,u32 option_addr)
{

	string name = Utilities::readString(name_addr, 32);

	Modules::log.debug("sceKernelCreateThread redirecting to hleKernelCreateThread");
    SceKernelThreadInfo *thread = hleKernelCreateThread(name, entry_addr, initPriority, stackSize, attr, option_addr);

    // Inherit kernel mode if user mode bit is not set
    if ((current_thread->attr & PSP_THREAD_ATTR_KERNEL) == PSP_THREAD_ATTR_KERNEL &&
        (attr & PSP_THREAD_ATTR_USER) != PSP_THREAD_ATTR_USER) {
        Modules::log.debug("sceKernelCreateThread inheriting kernel mode");
        thread->attr |= PSP_THREAD_ATTR_KERNEL;
    }

    // Inherit user mode
    if ((current_thread->attr & PSP_THREAD_ATTR_USER) == PSP_THREAD_ATTR_USER) {
        if ((thread->attr & PSP_THREAD_ATTR_USER) != PSP_THREAD_ATTR_USER)
            Modules::log.debug("sceKernelCreateThread inheriting user mode");
        thread->attr |= PSP_THREAD_ATTR_USER;
        // Always remove kernel mode bit
        thread->attr &= ~PSP_THREAD_ATTR_KERNEL;
    }
	 pspRegs.GPR[2] = thread->uid;
}
void ThreadMan::sceKernelTerminateThread(int uid)
{
	//TODO
}
void ThreadMan::sceKernelTerminateDeleteThread(int uid)
{
	//TODO
}
//TODO
#if 0  
    public void setThreadBanningEnabled(boolean enabled) {
        USE_THREAD_BANLIST = enabled;
        Modules.log.info("Audio threads disabled: " + USE_THREAD_BANLIST);
    }

    /** use lower case in this list */
    private final String[] threadNameBanList = new String[] {
        "bgm thread", "sgx-psp-freq-thr", "sgx-psp-pcm-th", "ss playthread",
        "spcbgm", "scemainsamplebgmmp3"
    };
    /* suspected sound thread names:
     * SndMain, SoundThread, At3Main, Atrac3PlayThread,
     * bgm thread, SceWaveMain, SasCore thread, soundThread,
     * ATRAC3 play thread, SAS Thread, XomAudio, sgx-psp-freq-thr,
     * sgx-psp-at3-th, sgx-psp-pcm-th, sgx-psp-sas-th, snd_tick_timer_thread,
     * snd_stream_service_thread_1, SAS / Main Audio, AudioMixThread,
     * snd_stream_service_thread_0, sound_poll_thread, stream_sound_poll_thread,
     * sndp thread, Ss PlayThread, SndSsThread, SPCBGM
     *
     * keywords:
     * snd, sound, at3, atrac3, sas, wave, pcm, audio
     *
     * false positives:
     * pcm: SPCMain (Skate Park City Main)
     *
     * ambiguous keywords:
     * bgm, freq, sgx
     */
    private boolean isBannedThread(SceKernelThreadInfo thread) {
        if (USE_THREAD_BANLIST) {
            String name = thread.name.toLowerCase();
            if (name.contains("snd") || name.contains("sound") ||
                name.contains("at3") || name.contains("atrac") ||
                name.contains("sas") || name.contains("wave") ||
                name.contains("audio")) {
                return true;
            }

            for (String threadName : threadNameBanList) {
                if (name.equals(threadName)) {
                    return true;
                }
            }
        }
        return false;
    }
#endif
void ThreadMan::hleKernelStartThread(SceKernelThreadInfo *thread, int userDataLength, int userDataAddr, int gp) {


	Modules::log.debug("hleKernelStartThread SceUID= %x name= %s , dataLen=0x%x data=0x%x gp=0x%x",thread->uid,thread->name,userDataLength,userDataAddr,gp);
	// Setup args by copying them onto the stack
	//int address = thread->cpuContext.gpr[29];
	// 256 bytes padding between user data top and real stack top
	u32 address = (thread->stack_addr + thread->stackSize - 0x100) - ((userDataLength + 0xF) & ~0xF);
	if (userDataAddr != 0) {
		memcpy(Memory::getPointer(address), Memory::getPointer(userDataAddr), userDataLength);
		thread->cpuContext.GPR[4] = userDataLength; // a0 = user data len
		thread->cpuContext.GPR[5] = address; // a1 = pointer to arg data in stack
	} else {
		// Set the pointer to NULL when none is provided
		thread->cpuContext.GPR[4] = 0; // a0 = user data len
		thread->cpuContext.GPR[5] = 0; // a1 = pointer to arg data in stack
	}

	// 64 bytes padding between program stack top and user data
	thread->cpuContext.GPR[29] = address - 0x40;
	// $k0 set just past the user data
	thread->cpuContext.GPR[26] = thread->stack_addr + thread->stackSize - 0x100;

	// testing
	if (thread->cpuContext.GPR[28] != gp) {
		Modules::log.debug("hleKernelStartThread oldGP=0x%x newGP=0x%x",thread->cpuContext.GPR[28],gp);
	}
	thread->cpuContext.GPR[28] = gp;

	// Start thread immediately
	changeThreadState(thread, PSP_THREAD_READY);
	contextSwitch(thread);
}
void ThreadMan::sceKernelStartThread(u32 uid,u32 len,u32 data_addr)
{

	SceUidManager::checkUidPurpose(uid, "ThreadMan-thread", true);
	ThreadMap::iterator it = threadMap.find(uid);
	if (it == threadMap.end()) {
		Modules::log.error("Start thread can't find the corrent thread to start");
		pspRegs.GPR[2] = ERROR_NOT_FOUND_THREAD;
//TODO
	} /*else if (isBannedThread(thread)) {
		Modules.log.warn("sceKernelStartThread SceUID=" + Integer.toHexString(thread->uid) + " name:'" + thread->name + "' banned, not starting");
		// Banned, fake start
		Emulator.getProcessor().cpu.gpr[2] = 0;
		contextSwitch(nextThread());
	} */else {
		Modules::log.debug("sceKernelStartThread redirecting to hleKernelStartThread");
		SceKernelThreadInfo *thread = it->second;
		hleKernelStartThread(thread, len, data_addr, thread->gpReg_addr);
	}
}
/** exit the current thread */
void ThreadMan::sceKernelExitThread(int exitStatus) {
 
	   Modules::log.debug("sceKernelExitThread SceUID=%x name: %s exitStatus:0x%x",current_thread->uid,current_thread->name,exitStatus);
		   
        current_thread->exitStatus = exitStatus;
        pspRegs.GPR[2] = 0;
        changeThreadState(current_thread, PSP_THREAD_STOPPED);

        contextSwitch(nextThread());
    }
/** exit the current thread and then delete it */
void ThreadMan::sceKernelExitDeleteThread(int exitStatus)
{
	//TODO
}
void ThreadMan::hleKernelSleepThread(bool do_callbacks) {
	// Go to wait state, callbacks
	current_thread->do_callbacks = do_callbacks;

	// Wait forever (another thread will call sceKernelWakeupThread)
	hleKernelThreadWait(current_thread->wait, 0, true);

	changeThreadState(current_thread, PSP_THREAD_WAITING);

	pspRegs.GPR[2] = 0;
	yieldCurrentThread(); // should be contextSwitch(nextThread()) but we get more logging this way
}
/** sleep the current thread (using wait) */
void ThreadMan::sceKernelSleepThread() {
	Modules::log.debug("sceKernelSleepThread SceUID=%x name:%s",current_thread->uid,current_thread->name);
    hleKernelSleepThread(false);
}
/** sleep the current thread and handle callbacks (using wait)
     * in our implementation we have to use wait, not suspend otherwise we don't handle callbacks. */
void ThreadMan::sceKernelSleepThreadCB() {
	Modules::log.debug("sceKernelSleepThreadCB SceUID=%x name:%s",current_thread->uid,current_thread->name);
	hleKernelSleepThread(true);
	checkCallbacks();
}

//TODO 
#if 0
   public void ThreadMan_sceKernelWakeupThread(int uid) {
        SceUidManager.checkUidPurpose(uid, "ThreadMan-thread", true);
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("sceKernelWakeupThread SceUID=" + Integer.toHexString(uid) + " unknown thread");
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else if (thread.status != PSP_THREAD_WAITING) {
            Modules.log.warn("sceKernelWakeupThread SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "' not sleeping/waiting (status=" + thread.status + ")");
            Emulator.getProcessor().cpu.gpr[2] = ERROR_THREAD_IS_NOT_WAIT;
        } else if (isBannedThread(thread)) {
            Modules.log.warn("sceKernelWakeupThread SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "' banned, not waking up");
            Emulator.getProcessor().cpu.gpr[2] = 0;
        } else {
            Modules.log.debug("sceKernelWakeupThread SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "'");
            changeThreadState(thread, PSP_THREAD_READY);
            Emulator.getProcessor().cpu.gpr[2] = 0;
        }
    }

    public void ThreadMan_sceKernelSuspendThread(int uid) {
        SceUidManager.checkUidPurpose(uid, "ThreadMan-thread", true);
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("sceKernelSuspendThread SceUID=" + Integer.toHexString(uid) + " unknown thread");
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else {
            Modules.log.debug("sceKernelSuspendThread SceUID=" + Integer.toHexString(uid));
            changeThreadState(thread, PSP_THREAD_SUSPEND);
            Emulator.getProcessor().cpu.gpr[2] = 0;
        }
    }

    public void ThreadMan_sceKernelResumeThread(int uid) {
        SceUidManager.checkUidPurpose(uid, "ThreadMan-thread", true);
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("sceKernelResumeThread SceUID=" + Integer.toHexString(uid) + " unknown thread");
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else if (thread.status != PSP_THREAD_SUSPEND) {
            Modules.log.warn("sceKernelResumeThread SceUID=" + Integer.toHexString(uid) + " not suspended (status=" + thread.status + ")");
            Emulator.getProcessor().cpu.gpr[2] = ERROR_THREAD_IS_NOT_SUSPEND;
        } else if (isBannedThread(thread)) {
            Modules.log.warn("sceKernelResumeThread SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "' banned, not resuming");
            Emulator.getProcessor().cpu.gpr[2] = 0;
        } else {
            Modules.log.debug("sceKernelResumeThread SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "'");
            changeThreadState(thread, PSP_THREAD_READY);
            Emulator.getProcessor().cpu.gpr[2] = 0;
        }
    }

    private void hleKernelWaitThreadEnd(int uid, int micros, boolean forever, boolean callbacks) {
        Modules.log.debug("hleKernelWaitThreadEnd SceUID=" + Integer.toHexString(uid)
            + " micros=" + micros
            + " forever=" + forever
            + " callbacks=" + callbacks);

        SceUidManager.checkUidPurpose(uid, "ThreadMan-thread", true);
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("hleKernelWaitThreadEnd unknown thread 0x" + Integer.toHexString(uid));
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else if (isBannedThread(thread)) {
            Modules.log.warn("hleKernelWaitThreadEnd SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "' banned, not waiting");
            Emulator.getProcessor().cpu.gpr[2] = 0;
            contextSwitch(nextThread()); // yield
        } else if (thread.status == PSP_THREAD_STOPPED) {
            Modules.log.debug("hleKernelWaitThreadEnd SceUID=" + Integer.toHexString(uid) + " name:'" + thread.name + "' thread already stopped, not waiting");
            Emulator.getProcessor().cpu.gpr[2] = 0;
            contextSwitch(nextThread()); // yield
        } else {
            // Do callbacks?
            current_thread.do_callbacks = callbacks;

            // Go to wait state
            hleKernelThreadWait(current_thread.wait, micros, forever);

            // Wait on a specific thread end
            current_thread.wait.waitingOnThreadEnd = true;
            current_thread.wait.ThreadEnd_id = uid;

            changeThreadState(current_thread, PSP_THREAD_WAITING);

            contextSwitch(nextThread());
        }
    }

    public void ThreadMan_sceKernelWaitThreadEnd(int uid, int timeout_addr) {
        Modules.log.debug("sceKernelWaitThreadEnd redirecting to hleKernelWaitThreadEnd(callbacks=false)");

        int micros = 0;
        boolean forever = true;

        if (timeout_addr != 0) { // psp does not check for valid address here
            micros = Memory.getInstance().read32(timeout_addr);
            forever = false;
        }

        hleKernelWaitThreadEnd(uid, micros, forever, false);
    }

    // disabled in syscall handler for TOE
    public void ThreadMan_sceKernelWaitThreadEndCB(int uid, int timeout_addr) {
        Modules.log.debug("sceKernelWaitThreadEndCB redirecting to hleKernelWaitThreadEnd(callbacks=true)");

        int micros = 0;
        boolean forever = true;

        if (timeout_addr != 0) { // psp does not check for address inside a valid range, just 0 or not 0
            micros = Memory.getInstance().read32(timeout_addr);
            forever = false;
        }

        hleKernelWaitThreadEnd(uid, micros, forever, true);
        checkCallbacks();
    }

#endif
void ThreadMan::hleKernelThreadWait(ThreadWaitInfo &wait, int micros, bool forever) {
	wait.forever = forever;
	wait.microTimeTimeout = emuReferenceTime/1000 + micros; //TODO CHECK!!!  Emulator.getClock().microTime() + micros;

	/*if (LOG_CONTEXT_SWITCHING && !isIdleThread(current_thread))
		Modules.log.debug("-------------------- hleKernelThreadWait micros=" + micros + " forever:" + forever + " thread:'" + current_thread.name + "' caller:" + getCallingFunction());*/
}
void ThreadMan::hleKernelDelayThread(int micros, bool do_callbacks) {
	
	if(IGNORE_DELAY)
		micros=0;

	// Wait on a timeout only
	hleKernelThreadWait(current_thread->wait, micros, false);

	changeThreadState(current_thread, PSP_THREAD_WAITING);

	pspRegs.GPR[2] = 0;

	// should be contextSwitch(nextThread()) but we get more logging this way
	// also current_thread.do_callbacks = do_callbacks;
	if (do_callbacks) {
		yieldCurrentThreadCB();
	} else {
		yieldCurrentThread();
	}
}
/** wait the current thread for a certain number of microseconds */
void ThreadMan::sceKernelDelayThread(u32 micros) {

	hleKernelDelayThread(micros, false);
}

/** wait the current thread for a certain number of microseconds */
void ThreadMan::sceKernelDelayThreadCB(u32 micros) {
	
	// This check is required
	if (!insideCallback) {
		hleKernelDelayThread(micros, true);
	} else {
		Modules::log.warn("sceKernelDelayThreadCB called from inside callback!");
	}
}
SceKernelCallbackInfo* ThreadMan::hleKernelCreateCallback(const string &name, u32 func_addr, u32 user_arg_addr) 
{
        SceKernelCallbackInfo *callback = new SceKernelCallbackInfo(name, current_thread->uid, func_addr, user_arg_addr);
        callbackMap.insert(make_pair(callback->uid, callback));
		Modules::log.debug("hleKernelCreateCallback SceUID=%x name: %s PC=%x arg=%x thread: %s",callback->uid,callback->name,func_addr,user_arg_addr,current_thread->name);
        return callback;
}
void ThreadMan::sceKernelCreateCallback(u32 name_addr,u32 func_addr,u32 user_arg_addr) 
{
        string name = Utilities::readString(name_addr, 32);
        SceKernelCallbackInfo *callback = hleKernelCreateCallback(name, func_addr, user_arg_addr);

        pspRegs.GPR[2] = callback->uid;
}

//TODO
#if 0

    /** @return true if successful. */
    public boolean hleKernelDeleteCallback(int uid) {
        SceKernelCallbackInfo info = callbackMap.remove(uid);

        if (info != null) {
            Modules.log.debug("hleKernelDeleteCallback SceUID=" + Integer.toHexString(uid)
                    + " name:'" + info.name + "'");
        } else {
            Modules.log.warn("hleKernelDeleteCallback not a callback uid 0x" + Integer.toHexString(uid));
        }

        return info != null;
    }

    public void ThreadMan_sceKernelDeleteCallback(int uid) {
        if (hleKernelDeleteCallback(uid)) {
            // TODO automatically unregister the callback if it was registered with another system?
            // example: sceKernelDeleteCallback called before sceUmdUnRegisterUMDCallBack
            Emulator.getProcessor().cpu.gpr[2] = 0;
        } else {
            Emulator.getProcessor().cpu.gpr[2] = -1;
        }
    }
	  public void ThreadMan_sceKernelCheckCallback() {
        Modules.log.debug("sceKernelCheckCallback");
        current_thread.do_callbacks = true;
        checkCallbacks();
    }

    public void ThreadMan_sceKernelReferCallbackStatus(int uid, int info_addr) {
        Modules.log.debug("sceKernelReferCallbackStatus SceUID=" + Integer.toHexString(uid)
            + " info=" + Integer.toHexString(info_addr));

        Memory mem = Memory.getInstance();
        SceKernelCallbackInfo info = callbackMap.get(uid);
        if (info == null) {
            Modules.log.warn("sceKernelReferCallbackStatus unknown uid 0x" + Integer.toHexString(uid));
            Emulator.getProcessor().cpu.gpr[2] = -1;
        } else if (!mem.isAddressGood(info_addr)) {
            Modules.log.warn("sceKernelReferCallbackStatus bad info address 0x" + Integer.toHexString(info_addr));
            Emulator.getProcessor().cpu.gpr[2] = -1;
        } else {
            int size = mem.read32(info_addr);
            if (size != SceKernelCallbackInfo.size) {
                Modules.log.warn("sceKernelReferCallbackStatus bad info size got " + size + " want " + SceKernelCallbackInfo.size);
                Emulator.getProcessor().cpu.gpr[2] = -1;
            } else {
                info.write(mem, info_addr);
                Emulator.getProcessor().cpu.gpr[2] = 0;
            }
        }
    }
#endif

void ThreadMan::sceKernelGetThreadId() {
	pspRegs.GPR[2] = current_thread->uid;
}

void ThreadMan::sceKernelGetThreadCurrentPriority() {
	pspRegs.GPR[2] = current_thread->currentPriority;
}
//TODO
#if 0 
    public void ThreadMan_sceKernelGetThreadExitStatus(int uid) {
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("sceKernelGetThreadExitStatus unknown uid=0x" + Integer.toHexString(uid));
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else  {
            Modules.log.debug("sceKernelGetThreadExitStatus uid=0x" + Integer.toHexString(uid) + " exitStatus=0x" + Integer.toHexString(thread.exitStatus));
            Emulator.getProcessor().cpu.gpr[2] = thread.exitStatus;
        }
    }

    /** @return amount of free stack space.
     * TODO this isn't quite right */
    public void ThreadMan_sceKernelCheckThreadStack() {
        int size = current_thread.stackSize
            - (Emulator.getProcessor().cpu.gpr[29] - current_thread.stack_addr)
            - 0x130;
        if (size < 0)
            size = 0;
        Emulator.getProcessor().cpu.gpr[2] = size;
    }

    /** @return amount of free stack space? up to 0x1000 lower?
     * TODO this isn't quite right */
    public void ThreadMan_sceKernelGetThreadStackFreeSize(int uid) {
        if (uid == 0) uid = current_thread.uid;
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("sceKernelGetThreadStackFreeSize unknown uid=0x" + Integer.toHexString(uid));
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else  {
            int size = current_thread.stackSize
                - (Emulator.getProcessor().cpu.gpr[29] - current_thread.stack_addr)
                - 0x130
                - 0xfb0;
            if (size < 0)
                size = 0;
            Emulator.getProcessor().cpu.gpr[2] = size;
        }
    }
#endif

void ThreadMan::sceKernelReferThreadStatus(u32 uid,u32 addr) 
{

	//Get the status information for the specified thread
	if (uid == 0) uid = current_thread->uid;
	ThreadMap::iterator it = threadMap.find(uid);
	if (it == threadMap.end()) {
		Modules::log.warn("sceKernelReferThreadStatus unknown uid=0x%x",uid);
		pspRegs.GPR[2] = ERROR_NOT_FOUND_THREAD;
	} else  {
		SceKernelThreadInfo *thread = it->second;
		Modules::log.debug("sceKernelReferThreadStatus uid=0x%x addr=0x%x",uid,addr);
		thread->write(addr);
		pspRegs.GPR[2] = 0;
	}
}
//TODO
#if 0
    /** Write uid's to buffer */
    public void ThreadMan_sceKernelGetThreadmanIdList(int type,
        int readbuf_addr, int readbufsize, int idcount_addr) {
        Memory mem = Memory.getInstance();

        Modules.log.debug("sceKernelGetThreadmanIdList type=" + type
            + " readbuf:0x" + Integer.toHexString(readbuf_addr)
            + " readbufsize:" + readbufsize
            + " idcount:0x" + Integer.toHexString(idcount_addr));

        // TODO type=SCE_KERNEL_TMID_Thread, don't show the idle threads!

        int count = 0;

        switch(type) {
            case SCE_KERNEL_TMID_Thread:
                for (Iterator<SceKernelThreadInfo> it = threadMap.values().iterator(); it.hasNext() /*&& count < readbufsize*/; ) {
                    SceKernelThreadInfo thread = it.next();

                    // Hide kernel mode threads when called from a user mode thread
                    if ((thread.attr & PSP_THREAD_ATTR_KERNEL) != PSP_THREAD_ATTR_KERNEL ||
                        (current_thread.attr & PSP_THREAD_ATTR_KERNEL) == PSP_THREAD_ATTR_KERNEL) {

                        if (count < readbufsize) {
                            Modules.log.debug("sceKernelGetThreadmanIdList adding thread '" + thread.name + "'");
                            mem.write32(readbuf_addr + count * 4, thread.uid);
                            count++;
                        } else {
                            Modules.log.warn("sceKernelGetThreadmanIdList NOT adding thread '" + thread.name + "'");
                        }
                    }
                }
                break;

            default:
                Modules.log.warn("UNIMPLEMENTED:sceKernelGetThreadmanIdList type=" + type);
                break;
        }

        // Fake success - 0 entries written
        if (mem.isAddressGood(idcount_addr)) {
            idcount_addr = count;
        }

        Emulator.getProcessor().cpu.gpr[2] = count; // TODO or idcount_addr?
    }

    public void ThreadMan_sceKernelChangeThreadPriority(int uid, int priority) {
        if (uid == 0) uid = current_thread.uid;
        SceUidManager.checkUidPurpose(uid, "ThreadMan-thread", true);
        SceKernelThreadInfo thread = threadMap.get(uid);
        if (thread == null) {
            Modules.log.warn("sceKernelChangeThreadPriority SceUID=" + Integer.toHexString(uid)
                    + " newPriority:0x" + Integer.toHexString(priority) + " unknown thread");
            Emulator.getProcessor().cpu.gpr[2] = ERROR_NOT_FOUND_THREAD;
        } else {
            Modules.log.debug("sceKernelChangeThreadPriority SceUID=" + Integer.toHexString(thread.uid)
                    + " newPriority:0x" + Integer.toHexString(priority) + " oldPriority:0x" + Integer.toHexString(thread.currentPriority));

            thread.currentPriority = priority;

            Emulator.getProcessor().cpu.gpr[2] = 0;
        }
    }

    public void ThreadMan_sceKernelChangeCurrentThreadAttr(int removeAttr, int addAttr) {
        Modules.log.debug("sceKernelChangeCurrentThreadAttr"
                + " removeAttr:0x" + Integer.toHexString(removeAttr)
                + " addAttr:0x" + Integer.toHexString(addAttr)
                + " oldAttr:0x" + Integer.toHexString(current_thread.attr));

        // Probably meant to be sceKernelChangeThreadAttr unknown=uid
        if (removeAttr != 0)
            Modules.log.warn("sceKernelChangeCurrentThreadAttr removeAttr:0x" + Integer.toHexString(removeAttr) + " non-zero");

        int newAttr = (current_thread.attr & ~removeAttr) | addAttr;
        // Don't allow switching into kernel mode!
        if ((current_thread.attr & PSP_THREAD_ATTR_USER) == PSP_THREAD_ATTR_USER &&
            (newAttr & PSP_THREAD_ATTR_USER) != PSP_THREAD_ATTR_USER) {
            Modules.log.debug("sceKernelChangeCurrentThreadAttr forcing user mode");
            newAttr |= PSP_THREAD_ATTR_USER;
        }

        current_thread.attr = newAttr;

        Emulator.getProcessor().cpu.gpr[2] = 0;
    }

    /** Registers a callback on the current thread.
     * @return true on success (the cbid was a valid callback uid) */
    public boolean setCallback(int callbackType, int cbid) {
        // Consistency check
        if (current_thread.callbackReady[callbackType])
            Modules.log.warn("setCallback(type=" + callbackType + ") ready=true");

        SceKernelCallbackInfo callback = callbackMap.get(cbid);
        if (callback == null) {
            Modules.log.warn("setCallback(type=" + callbackType + ") unknown uid " + Integer.toHexString(cbid));
            return false;
        } else {
            current_thread.callbackRegistered[callbackType] = true;
            current_thread.callbackInfo[callbackType] = callback;
            return true;
        }
    }

    /** Unregisters a callback by type and cbid. May not be on the current thread.
     * @param callbackType See SceKernelThreadInfo.
     * @param cbid The UID of the callback to unregister.
     * @return SceKernelCallbackInfo of the removed callback, or null if it
     * couldn't be found. */
    public SceKernelCallbackInfo clearCallback(int callbackType, int cbid) {
        SceKernelCallbackInfo oldInfo = null;

        for (Iterator<SceKernelThreadInfo> it = threadMap.values().iterator(); it.hasNext(); ) {
            SceKernelThreadInfo thread = it.next();

            if (thread.callbackRegistered[callbackType] &&
                thread.callbackInfo[callbackType].uid == cbid) {

                // Warn if we are removing a pending callback, this a callback
                // that has been pushed but not yet executed.
                if (thread.callbackReady[callbackType])
                    Modules.log.warn("clearCallback(type=" + callbackType + ") removing pending callback");

                oldInfo = thread.callbackInfo[callbackType];

                thread.callbackRegistered[callbackType] = false;
                thread.callbackReady[callbackType] = false;
                thread.callbackInfo[callbackType] = null;

                break;
            }
        }

        if (oldInfo == null) {
            Modules.log.warn("clearCallback(type=" + callbackType + ") cbid=" + Integer.toHexString(cbid)
                + " no matching callbacks found");
        }

        return oldInfo;
    }

    public boolean isInsideCallback() {
        return insideCallback;
    }

    /** push callback to all threads */
    public void pushCallback(int callbackType, int notifyArg) {
        pushCallback(callbackType, -1, 1, notifyArg);
    }

    /** @param cbid If cbid is -1, then push callback to all threads
     * if cbid is not -1 then only trigger that specific cbid provided it is
     * also of type callbackType.
     * ONLY call this from the main emulation thread, do not call from the GE thread! */
    public void pushCallback(int callbackType, int cbid, int notifyArg1, int notifyArg2) {
        boolean pushed = false;

        // GE callback is currently using Kernel callback implementation
        // To reduce log spam we'll use the VideoEngine logger on the GE callbacks
        org.apache.log4j.Logger log = Modules.log;
        if (callbackType == THREAD_CALLBACK_GE_SIGNAL ||
            callbackType == THREAD_CALLBACK_GE_FINISH)
            log = jpcsp.graphics.VideoEngine.log;

        for (Iterator<SceKernelThreadInfo> it = threadMap.values().iterator(); it.hasNext(); ) {
            SceKernelThreadInfo thread = it.next();

            if (thread.callbackRegistered[callbackType] &&
                (cbid == -1 || thread.callbackInfo[callbackType].uid == cbid)) {
                if (thread.callbackReady[callbackType]) {
                    // TODO behaviour may be undefined - example: terminate this thread, but continue other threads as normal
                    String msg = "pushCallback(type=" + callbackType
                        + ") thread:'" + thread.name
                        + "' already has callback pending (oldArg=0x" + Integer.toHexString(thread.callbackInfo[callbackType].notifyArg2)
                        + ",newArg=0x" + Integer.toHexString(notifyArg2) + ")";

                    if (thread.callbackInfo[callbackType].notifyArg1 == notifyArg1 &&
                        thread.callbackInfo[callbackType].notifyArg2 == notifyArg2) {
                        // args didn't change, probably not important so use debug instead of warn log level
                        log.debug(msg);
                    } else {
                        log.warn(msg);
                    }
                }

                thread.callbackReady[callbackType] = true;
                thread.callbackInfo[callbackType].notifyArg1 = notifyArg1;
                thread.callbackInfo[callbackType].notifyArg2 = notifyArg2;

                pushed = true;
            }
        }

        if (pushed) {
            // Enter callbacks immediately,
            // except those registered to the current thread. The app must explictly
            // call sceKernelCheckCallback or a waitCB function to do that.
            if (!insideCallback) {
                log.debug("pushCallback(type=" + callbackType + ") calling checkCallbacks");
                checkCallbacks();
            } else {
                log.error("pushCallback(type=" + callbackType + ") called while inside another callback!");
                Emulator.PauseEmu();
            }
        } else {
            log.warn("pushCallback(type=" + callbackType + ") no registered callbacks to push");
        }
    }
#endif
bool ThreadMan::checkThreadCallbacks(SceKernelThreadInfo* thread) {
	bool handled = false;

	for (int i = 0; i < THREAD_CALLBACK_SIZE && !handled; i++) {
		if (thread->callbackRegistered[i] && thread->callbackReady[i]) {
			Modules::log.debug("Entering callback type %u name: %s PC: %x thread %s $a0: %x $a1: %x $a2: %x ",i,thread->callbackInfo[i]->name
				,thread->callbackInfo[i]->callback_addr,thread->name,thread->callbackInfo[i]->notifyArg1,thread->callbackInfo[i]->notifyArg2,
				thread->callbackInfo[i]->callback_arg_addr);

			// Callbacks can pre-empt, save the current thread's context
			current_thread->saveContext();

			insideCallback = true;
			thread->callbackReady[i] = false;
			// Set the callback to run with the thread context it was registered from

			thread->callbackInfo[i]->startContext(thread->cpuContext/*, thread*/);
			thread->callbackInfo[i]->notifyCount++;
			handled = true;
		}
	}

	return handled;
}




    /**
     * Iterates waiting threads, making sure do_callbacks is set before
     * checking for pending callbacks.
     * Handles sceKernelCheckCallback when do_callbacks is set on current_thread.
     * Handles redirects to yieldCB (from fake waitCB) on the thread that called waitCB.
     *
     * We currently call checkCallbacks() at the end of each waitCB function
     * since this has less overhead than checking on every step.
     *
     * Some trickery is used in yieldCurrentThreadCB(). By the time we get
     * inside the checkCallbacks() function the thread that called yieldCB is
     * no longer the current thread. Also the thread that called yieldCB is
     * not in the wait state (it's in the ready state). so what we do is check
     * every thread, not just the waiting threads for the do_callbacks flag.
     * Also the waitingThreads list only contains waiting threads that have a
     * finite wait period, so we have to iterate on all threads anyway.
     *
     * It is probably unsafe to call contextSwitch() when insideCallback is true.
     * insideCallback may become true after a call to checkCallbacks().
     */
void ThreadMan::checkCallbacks() {
	//Modules.log.debug("checkCallbacks current thread is '" + current_thread.name + "' do_callbacks:" + current_thread.do_callbacks + " insideCallback:" + insideCallback + " caller:" + getCallingFunction());

	insideCallback = false;

	for(ThreadMap::iterator it = threadMap.begin(); it != threadMap.end(); ++it) {
		SceKernelThreadInfo *thread = it->second;
		// To work with our fake wait CB's (yieldCB()) we also need to check non-waiting threads.
		// because of this we are able to merge the separate check on current_thread into this loop (handle sceKernelCheckCallback).
		// Now we really need to make sure do_callbacks is consistent!
		//Modules.log.debug("checkCallbacks: candidate thread:'" + thread.name
		//    + "' state:0x" + Integer.toHexString(thread.status)
		//    + " do_callbacks:" + thread.do_callbacks);
		if (thread->do_callbacks && checkThreadCallbacks(thread)) {
			// callback was started
			break;
		}
		
	}

	if (!insideCallback) {
		//Modules.log.debug("checkCallbacks: no suitable thread/callback combinations remaining");

		// try and keep do_callbacks in a consistent state
		// usually when we come from sceKernelCheckCallback or yieldCurrentThreadCB
		for(ThreadMap::iterator it = threadMap.begin(); it != threadMap.end(); ++it) {
			SceKernelThreadInfo *thread = it->second;
			if (thread->status != PSP_THREAD_WAITING && thread->do_callbacks) {
				//Modules.log.debug("checkCallbacks: removing do_callbacks from non-waiting thread:'" + thread.name + "'");

				thread->do_callbacks = false;
			}
		}
	}
}
/** @return the bottom address or 0 on failure. */
int ThreadMan::mallocStack(int size) {
    if (size > 0) {
		int p = pspSysMem::malloc(2, PSP_SMEM_High, size, 0);
        if (p != 0) 
		{
          pspSysMem::addSysMemInfo(2, "ThreadMan-Stack",PSP_SMEM_High, size, p);
        }
        return p;
    } 
	else 
	{
      return 0;
    }
}
//TODO
#if 0
   public class Statistics {
        private ArrayList<ThreadStatistics> threads = new ArrayList<ThreadStatistics>();
        public long allCycles = 0;
        public long startTimeMillis;
        public long endTimeMillis;

        public Statistics() {
            startTimeMillis = System.currentTimeMillis();
        }

        public void exit() {
            endTimeMillis = System.currentTimeMillis();
        }

        public long getDurationMillis() {
            return endTimeMillis - startTimeMillis;
        }

        private void addThreadStatistics(SceKernelThreadInfo thread) {
            ThreadStatistics threadStatistics = new ThreadStatistics();
            threadStatistics.name = thread.name;
            threadStatistics.runClocks = thread.runClocks;
            threads.add(threadStatistics);

            allCycles += thread.runClocks;
        }

        private class ThreadStatistics {
            public String name;
            public long runClocks;
        }
    }
#endif