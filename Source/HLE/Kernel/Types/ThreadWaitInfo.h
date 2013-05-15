/*
 * 
 *  30/04/2011 - synced with 913
 *               (actual change was in r826)
 */


#pragma once

struct ThreadWaitInfo {
    bool forever;
    u64 microTimeTimeout; // when Clock.microTime() reaches microTimeTimeout the wait has expired

    // TODO change waitingOnThreadEnd, waitingOnEventFlag, etc to waitType,
    // since we can only wait on one type of event at a time.

    // Thread End
    bool waitingOnThreadEnd;
    u32 ThreadEnd_id;

    // Event Flag
    bool waitingOnEventFlag;
    u32 EventFlag_id;
    u32 EventFlag_bits;
    u32 EventFlag_wait;
    u32 EventFlag_outBits_addr;

    // Semaphore
    bool waitingOnSemaphore;
    u32 Semaphore_id;
    u32 Semaphore_signal;

    // Mutex
    bool waitingOnMutex;
    u32 Mutex_id;

    // IO
    bool waitingOnIo;
    u32 Io_id;

	ThreadWaitInfo() : forever(false), waitingOnThreadEnd(false), waitingOnEventFlag(false),
		waitingOnSemaphore(false), waitingOnMutex(false), waitingOnIo(false) {}
};