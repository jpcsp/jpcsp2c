/*
*
*
*   
*    29/5/2011 -  synced with 734
*/

#include "Common.h"
#include "Memory.h"
#include "pspSysMem.h"
#include "HLE/Modules.h"
#include "HLE/Kernel/Managers/SceUidManager.h"
#include "Allegrex/Allegrex.h"

static map<u32, SysMemInfo> blockList;
static u32 heapTop;
static u32 heapBottom;



SysMemInfo::SysMemInfo(
	u32 partitionid, const string &name, u32 type, u32 size, u32 addr)
	: partitionid(partitionid), name(name), type(type), size(size), addr(addr)
{
	uid = SceUidManager::getNewUid("SysMem");
	blockList[uid] = *this;
}

void pspSysMem::initialise() {
	blockList.clear();

	// the loader should do the first Malloc
	// which will set the heapBottom corectly
	heapBottom = 0x08400000;
	heapTop = 0x09FFFFFF;//Memory::MainMemoryEnd;
}
//Allocates to 64-bit alignment
// TODO: use the partitionid
u32 pspSysMem::malloc(u32 partitionid, u32 type, u32 size, u32 addr) {
	u32 allocatedAddress = 0;

	if (type == PSP_SMEM_Low) {
		allocatedAddress = heapBottom;
		allocatedAddress = (allocatedAddress + 63) & ~63;
		heapBottom = allocatedAddress + size;

		if (heapBottom > heapTop) {
			Modules::log.warn("Malloc overflowed (heapBottom = 0x%08X, heapTop = 0x%08X)",heapBottom, heapTop);
		}
	}
	else if (type == PSP_SMEM_Addr) {
		s32 highDiff = heapTop - addr;
		s32 lowDiff  = heapBottom - addr;

		if (highDiff < 0)
			highDiff = -highDiff;
		if (lowDiff  < 0)
			lowDiff  = -lowDiff;

		if (lowDiff <= highDiff) {
			// alloc near bottom
			allocatedAddress = heapBottom;
			if (allocatedAddress < addr)
				allocatedAddress = addr;
			allocatedAddress = (allocatedAddress + 63) & ~63;
			heapBottom = allocatedAddress + size;

			if (heapBottom > heapTop) {
               Modules::log.warn("Malloc overflowed (heapBottom = 0x%08X, heapTop = 0x%08X)",heapBottom, heapTop);
			}
		}
		else {
			// alloc near top
			allocatedAddress = heapTop - size;
			if (allocatedAddress > addr)
				allocatedAddress = addr;
			allocatedAddress = (allocatedAddress + 63) & ~63;
			heapTop = allocatedAddress;

			if (heapTop < heapBottom) {
				Modules::log.warn("Malloc underflowed (heapBottom = 0x%08X, heapTop = 0x%08X)",heapBottom, heapTop);
			}
		}
	}
	else if (type == PSP_SMEM_High) {
		allocatedAddress = heapTop - size + 1;
		allocatedAddress = allocatedAddress & ~63;
		heapTop = allocatedAddress - 1;

		if (heapTop < heapBottom) {
              Modules::log.warn("Malloc underflowed (heapBottom = 0x%08X, heapTop = 0x%08X)",heapBottom, heapTop);
		}
	}
	else if (type == PSP_SMEM_LowAligned) {
		allocatedAddress = heapBottom;
		allocatedAddress = (allocatedAddress + addr - 1) & ~(addr - 1);
		heapBottom = allocatedAddress + size;

		if (heapBottom > heapTop) {
             Modules::log.warn("Malloc overflowed (heapBottom = 0x%08X, heapTop = 0x%08X)",
				heapBottom, heapTop);
		}
	}
	else if (type == PSP_SMEM_HighAligned) {
		allocatedAddress = heapTop - size + 1;
		allocatedAddress = allocatedAddress & ~(addr - 1);
		heapTop = allocatedAddress - 1;

		if (heapTop < heapBottom) {
           Modules::log.warn("Malloc underflowed (heapBottom = 0x%08X, heapTop = 0x%08X)",
				heapBottom, heapTop);
		}
	}

	if (allocatedAddress != 0) {
        Modules::log.warn("Malloc(size = 0x%08X) new heapBottom = 0x%08X, new heapTop = 0x%08X",
			size, heapBottom, heapTop);
	}

	return allocatedAddress;
}
u32 pspSysMem::addSysMemInfo(
	u32 partitionid, const string &name, u32 type, u32 size, u32 addr)
{
	SysMemInfo info(partitionid, name, type, size, addr);
	return info.uid;
}
    // For internal use, example: ThreadMan allocating stack space
    // Also removes the associated SysMemInfo (if found) from blockList
void pspSysMem::free(u32 addr) {
	bool found = false;

	// reverse lookup on blockList, get SystemInfo and call free
	for (map<u32, SysMemInfo>::iterator it = blockList.begin();
		it != blockList.end(); ++it)
	{
		if (it->second.addr == addr) {
			found = true;
			free(it->second);
			blockList.erase(it);
			break;
		}
	}

	if (!found)
		Modules::log.warn("pspSysMem.Free(0x%08X) failed to find SysMemInfo", addr);
}


void pspSysMem::free(const SysMemInfo &info) {
	// TODO: implement
	Modules::log.error("UNIMPLEMENTED: pspSysMem.Free(info)");
}

u32 pspSysMem::maxFreeMemSize() {
	// Since some apps try and allocate the value of sceKernelMaxFreeMemSize,
	// which will leave no space for stacks we're going to reserve 0x09f00000
	// to 0x09ffffff for stacks, but stacks are allowed to go below that
	// (if there's free space of course).

	u32 heapTopGuard = heapTop;
	if (heapTopGuard > 0x09F00000)
		heapTopGuard = 0x09F00000;

	// don't forget our alignment padding!
	s32 maxFree = heapTopGuard - heapBottom - 64;// don't forget our alignment padding!

	// TODO: something not quite right here...
	if (maxFree < 0)
		maxFree = 0;

	return maxFree;
}


/** @return the size of the largest allocatable block */
void pspSysMem::sceKernelMaxFreeMemSize()
{
   int maxFree = maxFreeMemSize();
   Modules::log.debug("sceKernelMaxFreeMemSize %u (hex= %08X)",maxFree,maxFree);
   pspRegs.GPR[2]= maxFree;
}

void pspSysMem::sceKernelTotalFreeMemSize()
{
   int totalFree = heapTop - heapBottom;
   Modules::log.debug("sceKernelTotalFreeMemSize %u (hex= %08X)",totalFree,totalFree);
   pspRegs.GPR[2] = totalFree;
}

 /**
* @param partitionid TODO probably user, kernel etc
* 1 = kernel, 2 = user, 3 = me, 4 = kernel mirror (from potemkin/dash)
* @param type If type is PSP_SMEM_Addr, then addr specifies the lowest
* address to allocate the block from.
*/
void pspSysMem::sceKernelAllocPartitionMemory(u32 partitionid, u32 pname, u32 type, u32 size, u32 addr)
{
        pname &= 0x3fffffff;
        addr &= 0x3fffffff;
        string name = Utilities::readString(pname, 32);

        // print debug info
        string typeStr;
        switch(type) {
            case PSP_SMEM_Low: typeStr = "PSP_SMEM_Low"; break;
            case PSP_SMEM_High: typeStr = "PSP_SMEM_High"; break;
            case PSP_SMEM_Addr: typeStr = "PSP_SMEM_Addr"; break;
            case PSP_SMEM_LowAligned: typeStr = "PSP_SMEM_LowAligned"; break;
            case PSP_SMEM_HighAligned: typeStr = "PSP_SMEM_HighAligned"; break;
            default: typeStr = "UNHANDLED " + type; break;
        }

		Modules::log.debug("sceKernelAllocPartitionMemory(partitionid=%u name: '%s' type=%s size=%u addr=0x%x)",partitionid,name.c_str(),typeStr.c_str(),size,addr);
        if (type < PSP_SMEM_Low || type > PSP_SMEM_HighAligned)
        {
            pspRegs.GPR[2] = PSP_ERROR_ILLEGAL_MEMORY_BLOCK_ALLOCATION_TYPE;
        }
        else
        {
            addr = malloc(partitionid, type, size, addr);
            if (addr != 0)
            {
                SysMemInfo info (partitionid, name, type, size, addr);
                pspRegs.GPR[2] = info.uid;
            }
            else
            {
               pspRegs.GPR[2] = PSP_ERROR_FAILED_TO_ALLOCATE_MEMORY_BLOCK;
            }
        }
}

void pspSysMem::sceKernelFreePartitionMemory(u32 uid) 
{
	     SceUidManager::checkUidPurpose(uid, "SysMem", true);
	     map<u32, SysMemInfo>::iterator it = blockList.find(uid);
	     if (it == blockList.end())
	     {
			 Modules::log.warn("sceKernelFreePartitionMemory unknown SceUID= 0x%08X" + uid);
			 pspRegs.GPR[2]= PSP_ERROR_ILLEGAL_CHUNK_ID;
			 return;
		 }
		 else
		 {		
             free(it->second);	
			 Modules::log.warn("UNIMPLEMENT:sceKernelFreePartitionMemory SceUID= 0x%08X name : '%s'",uid,it->second.name.c_str());
			 pspRegs.GPR[2] = 0;
		 }
		 blockList.erase(uid);
		  
}

void pspSysMem::sceKernelGetBlockHeadAddr(u32 uid) 
{
	 SceUidManager::checkUidPurpose(uid, "SysMem", true);
	 map<u32, SysMemInfo>::iterator it = blockList.find(uid);
	 if (it == blockList.end())
	 {
		 Modules::log.warn("sceKernelGetBlockHeadAddr unknown SceUID= %08X" + uid);
         pspRegs.GPR[2] = PSP_ERROR_ILLEGAL_CHUNK_ID;
	 }
	else
	{
		Modules::log.debug("sceKernelGetBlockHeadAddr SceUID= %08X name: '%s' headAddr: %08X ", it->second.uid , it->second.name.c_str() , it->second.addr);
		pspRegs.GPR[2] = it->second.addr;
	}
}
void pspSysMem::sceKernelDevkitVersion()
{
        // Return 1.5 for now
        int version = PSP_FIRMWARE_150;
        Modules::log.debug("sceKernelDevkitVersion return: 0x%x",version);
		pspRegs.GPR[2] = version;
}
