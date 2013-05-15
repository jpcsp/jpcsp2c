/*
*
*
*   
*    29/5/2011 -  synced with 734
*/

#pragma once

enum pspSysMemBlockType {
	PSP_SMEM_Low         = 0,
	PSP_SMEM_High        = 1,
	PSP_SMEM_Addr        = 2,
	PSP_SMEM_LowAligned  = 3,
	PSP_SMEM_HighAligned = 4
};

enum PspFirmwareVersion {
	PSP_FIRMWARE_100 = 0x01000300,
	PSP_FIRMWARE_150 = 0x01050001,
	PSP_FIRMWARE_151 = 0x01050100,
	PSP_FIRMWARE_152 = 0x01050200,
	PSP_FIRMWARE_200 = 0x02000010,
	PSP_FIRMWARE_201 = 0x02000010, // Same as 2.00
	PSP_FIRMWARE_250 = 0x02050010,
	PSP_FIRMWARE_260 = 0x02060010,
	PSP_FIRMWARE_270 = 0x02070010,
	PSP_FIRMWARE_271 = 0x02070110
};

enum pspSysMemError {
	PSP_ERROR_ILLEGAL_PARTITION_ID                 = 0x800200d6,
	PSP_ERROR_PARTITION_IN_USE                     = 0x800200d7,
	PSP_ERROR_ILLEGAL_MEMORY_BLOCK_ALLOCATION_TYPE = 0x800200d8,
	PSP_ERROR_FAILED_TO_ALLOCATE_MEMORY_BLOCK      = 0x800200d9,	
	PSP_ERROR_ILLEGAL_CHUNK_ID                     = 0x800200de// may not be for pspSysMem...
};

class SysMemInfo {
public:
	SysMemInfo() { }
	SysMemInfo(
		u32 partitionid, const string &name, u32 type, u32 size, u32 addr);

	u32 uid;
	u32 partitionid;
	string name;
	u32 type;
	u32 size;
	u32 addr;
};

namespace pspSysMem {
	void initialise();
	
	u32 addSysMemInfo(
		u32 partitionid, const string &name, u32 type, u32 size, u32 addr);
	u32 maxFreeMemSize();

	// Allocates to 64-byte alignment
	u32 malloc(u32 partitionid, u32 type, u32 size, u32 addr);

	// For internal use, example: ThreadMan allocating stack space.
    // Also removes the associated SysMemInfo (if found) from blockList.
	void free(u32 addr);

	void free(const SysMemInfo &info);


	// returns the size of the largest allocatable block
	void sceKernelMaxFreeMemSize();
	void sceKernelTotalFreeMemSize();

	// partitionid: TODO: probably user, kernel etc
	// 1 = kernel, 2 = user, 3 = me, 4 = kernel mirror (from potemkin/dash)
	// type: If type is PSP_SMEM_Addr, then addr specifies the lowest
	// address to allocate the block from.
	void sceKernelAllocPartitionMemory(u32 partitionid, u32 pname, u32 type, u32 size, u32 addr);

	void sceKernelFreePartitionMemory(u32 uid);
	void sceKernelGetBlockHeadAddr(u32 uid);
	void sceKernelDevkitVersion();

	
};