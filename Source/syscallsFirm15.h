

/*
 *  30/04/2011 - synced with 813 (createMutex is 0x30A7 not 33017(jpcsp)
 *
 */

struct HLEFunction {
	u32 syscall;
	u32 nid;
	const char *name;
};

static const HLEFunction calls[] = {
	{ 0x2000, 0xCA04A2B9, "sceKernelRegisterSubIntrHandler" },
	{ 0x2001, 0xD61E6961, "sceKernelReleaseSubIntrHandler" },
	{ 0x2002, 0xFB8E22EC, "sceKernelEnableSubIntr" },
	{ 0x2003, 0x8A389411, "sceKernelDisableSubIntr" },
	{ 0x2004, 0x5CB5A78B, "sceKernelSuspendSubIntr" },
	{ 0x2005, 0x7860E0DC, "sceKernelResumeSubIntr" },
	{ 0x2006, 0xFC4374B8, "sceKernelIsSubInterruptOccurred" },
	{ 0x2007, 0xD2E8363F, "QueryIntrHandlerInfo" },
	{ 0x2008, 0xEEE43F47, "sceKernelRegisterUserSpaceIntrStack" },
	{ 0x2009, 0x6E9EA350, "_sceKernelReturnFromCallback" },
	{ 0x200A, 0x0C106E53, "sceKernelRegisterThreadEventHandler" },
	{ 0x200B, 0x72F3C145, "sceKernelReleaseThreadEventHandler" },
	{ 0x200C, 0x369EEB6B, "sceKernelReferThreadEventHandlerStatus" },
	{ 0x200D, 0xE81CAF8F, "sceKernelCreateCallback" },
	{ 0x200E, 0xEDBA5844, "sceKernelDeleteCallback" },
	{ 0x200F, 0xC11BA8C4, "sceKernelNotifyCallback" },
	{ 0x2010, 0xBA4051D6, "sceKernelCancelCallback" },
	{ 0x2011, 0x2A3D44FF, "sceKernelGetCallbackCount" },
	{ 0x2012, 0x349D6D6C, "sceKernelCheckCallback" },
	{ 0x2013, 0x730ED8BC, "sceKernelReferCallbackStatus" },
	{ 0x2014, 0x9ACE131E, "sceKernelSleepThread" },
	{ 0x2015, 0x82826F70, "sceKernelSleepThreadCB" },
	{ 0x2016, 0xD59EAD2F, "sceKernelWakeupThread" },
	{ 0x2017, 0xFCCFAD26, "sceKernelCancelWakeupThread" },
	{ 0x2018, 0x9944F31F, "sceKernelSuspendThread" },
	{ 0x2019, 0x75156E8F, "sceKernelResumeThread" },
	{ 0x201A, 0x278C0DF5, "sceKernelWaitThreadEnd" },
	{ 0x201B, 0x840E8133, "sceKernelWaitThreadEndCB" },
	{ 0x201C, 0xCEADEB47, "sceKernelDelayThread" },
	{ 0x201D, 0x68DA9E36, "sceKernelDelayThreadCB" },
	{ 0x201E, 0xBD123D9E, "sceKernelDelaySysClockThread" },
	{ 0x201F, 0x1181E963, "sceKernelDelaySysClockThreadCB" },
	{ 0x2020, 0xD6DA4BA1, "sceKernelCreateSema" },
	{ 0x2021, 0x28B6489C, "sceKernelDeleteSema" },
	{ 0x2022, 0x3F53E640, "sceKernelSignalSema" },
	{ 0x2023, 0x4E3A1105, "sceKernelWaitSema" },
	{ 0x2024, 0x6D212BAC, "sceKernelWaitSemaCB" },
	{ 0x2025, 0x58B1F937, "sceKernelPollSema" },
	{ 0x2026, 0x8FFDF9A2, "sceKernelCancelSema" },
	{ 0x2027, 0xBC6FEBC5, "sceKernelReferSemaStatus" },
	{ 0x2028, 0x55C20A00, "sceKernelCreateEventFlag" },
	{ 0x2029, 0xEF9E4C70, "sceKernelDeleteEventFlag" },
	{ 0x202A, 0x1FB15A32, "sceKernelSetEventFlag" },
	{ 0x202B, 0x812346E4, "sceKernelClearEventFlag" },
	{ 0x202C, 0x402FCF22, "sceKernelWaitEventFlag" },
	{ 0x202D, 0x328C546A, "sceKernelWaitEventFlagCB" },
	{ 0x202E, 0x30FD48F0, "sceKernelPollEventFlag" },
	{ 0x202F, 0xCD203292, "sceKernelCancelEventFlag" },
	{ 0x2030, 0xA66B0120, "sceKernelReferEventFlagStatus" },
	{ 0x2031, 0x8125221D, "sceKernelCreateMbx" },
	{ 0x2032, 0x86255ADA, "sceKernelDeleteMbx" },
	{ 0x2033, 0xE9B3061E, "sceKernelSendMbx" },
	{ 0x2034, 0x18260574, "sceKernelReceiveMbx" },
	{ 0x2035, 0xF3986382, "sceKernelReceiveMbxCB" },
	{ 0x2036, 0x0D81716A, "sceKernelPollMbx" },
	{ 0x2037, 0x87D4DD36, "sceKernelCancelReceiveMbx" },
	{ 0x2038, 0xA8E8C846, "sceKernelReferMbxStatus" },
	{ 0x2039, 0x7C0DC2A0, "sceKernelCreateMsgPipe" },
	{ 0x203A, 0xF0B7DA1C, "sceKernelDeleteMsgPipe" },
	{ 0x203B, 0x876DBFAD, "sceKernelSendMsgPipe" },
	{ 0x203C, 0x7C41F2C2, "sceKernelSendMsgPipeCB" },
	{ 0x203D, 0x884C9F90, "sceKernelTrySendMsgPipe" },
	{ 0x203E, 0x74829B76, "sceKernelReceiveMsgPipe" },
	{ 0x203F, 0xFBFA697D, "sceKernelReceiveMsgPipeCB" },
	{ 0x2040, 0xDF52098F, "sceKernelTryReceiveMsgPipe" },
	{ 0x2041, 0x349B864D, "sceKernelCancelMsgPipe" },
	{ 0x2042, 0x33BE4024, "sceKernelReferMsgPipeStatus" },
	{ 0x2043, 0x56C039B5, "sceKernelCreateVpl" },
	{ 0x2044, 0x89B3D48C, "sceKernelDeleteVpl" },
	{ 0x2045, 0xBED27435, "sceKernelAllocateVpl" },
	{ 0x2046, 0xEC0A693F, "sceKernelAllocateVplCB" },
	{ 0x2047, 0xAF36D708, "sceKernelTryAllocateVpl" },
	{ 0x2048, 0xB736E9FF, "sceKernelFreeVpl" },
	{ 0x2049, 0x1D371B8A, "sceKernelCancelVpl" },
	{ 0x204A, 0x39810265, "sceKernelReferVplStatus" },
	{ 0x204B, 0xC07BB470, "sceKernelCreateFpl" },
	{ 0x204C, 0xED1410E0, "sceKernelDeleteFpl" },
	{ 0x204D, 0xD979E9BF, "sceKernelAllocateFpl" },
	{ 0x204E, 0xE7282CB6, "sceKernelAllocateFplCB" },
	{ 0x204F, 0x623AE665, "sceKernelTryAllocateFpl" },
	{ 0x2050, 0xF6414A71, "sceKernelFreeFpl" },
	{ 0x2051, 0xA8AA591F, "sceKernelCancelFpl" },
	{ 0x2052, 0xD8199E4C, "sceKernelReferFplStatus" },
	{ 0x2053, 0x0E927AED, "_sceKernelReturnFromTimerHandler" },
	{ 0x2054, 0x110DEC9A, "sceKernelUSec2SysClock" },
	{ 0x2055, 0xC8CD158C, "sceKernelUSec2SysClockWide" },
	{ 0x2056, 0xBA6B92E2, "sceKernelSysClock2USec" },
	{ 0x2057, 0xE1619D7C, "sceKernelSysClock2USecWide" },
	{ 0x2058, 0xDB738F35, "sceKernelGetSystemTime" },
	{ 0x2059, 0x82BC5777, "sceKernelGetSystemTimeWide" },
	{ 0x205A, 0x369ED59D, "sceKernelGetSystemTimeLow" },
	{ 0x205B, 0x6652B8CA, "sceKernelSetAlarm" },
	{ 0x205C, 0xB2C25152, "sceKernelSetSysClockAlarm" },
	{ 0x205D, 0x7E65B999, "sceKernelCancelAlarm" },
	{ 0x205E, 0xDAA3F564, "sceKernelReferAlarmStatus" },
	{ 0x205F, 0x20FFF560, "sceKernelCreateVTimer" },
	{ 0x2060, 0x328F9E52, "sceKernelDeleteVTimer" },
	{ 0x2061, 0xB3A59970, "sceKernelGetVTimerBase" },
	{ 0x2062, 0xB7C18B77, "sceKernelGetVTimerBaseWide" },
	{ 0x2063, 0x034A921F, "sceKernelGetVTimerTime" },
	{ 0x2064, 0xC0B3FFD2, "sceKernelGetVTimerTimeWide" },
	{ 0x2065, 0x542AD630, "sceKernelSetVTimerTime" },
	{ 0x2066, 0xFB6425C3, "sceKernelSetVTimerTimeWide" },
	{ 0x2067, 0xC68D9437, "sceKernelStartVTimer" },
	{ 0x2068, 0xD0AEEE87, "sceKernelStopVTimer" },
	{ 0x2069, 0xD8B299AE, "sceKernelSetVTimerHandler" },
	{ 0x206A, 0x53B00E9A, "sceKernelSetVTimerHandlerWide" },
	{ 0x206B, 0xD2D615EF, "sceKernelCancelVTimerHandler" },
	{ 0x206C, 0x5F32BEAA, "sceKernelReferVTimerStatus" },
	{ 0x206D, 0x446D8DE6, "sceKernelCreateThread" },
	{ 0x206E, 0x9FA03CD3, "sceKernelDeleteThread" },
	{ 0x206F, 0xF475845D, "sceKernelStartThread" },
	{ 0x2070, 0x532A522E, "_sceKernelExitThread" },
	{ 0x2071, 0xAA73C935, "sceKernelExitThread" },
	{ 0x2072, 0x809CE29B, "sceKernelExitDeleteThread" },
	{ 0x2073, 0x616403BA, "sceKernelTerminateThread" },
	{ 0x2074, 0x383F7BCC, "sceKernelTerminateDeleteThread" },
	{ 0x2075, 0x3AD58B8C, "sceKernelSuspendDispatchThread" },
	{ 0x2076, 0x27E22EC2, "sceKernelResumeDispatchThread" },
	{ 0x2077, 0xEA748E31, "sceKernelChangeCurrentThreadAttr" },
	{ 0x2078, 0x71BC9871, "sceKernelChangeThreadPriority" },
	{ 0x2079, 0x912354A7, "sceKernelRotateThreadReadyQueue" },
	{ 0x207A, 0x2C34E053, "sceKernelReleaseWaitThread" },
	{ 0x207B, 0x293B45B8, "sceKernelGetThreadId" },
	{ 0x207C, 0x94AA61EE, "sceKernelGetThreadCurrentPriority" },
	{ 0x207D, 0x3B183E26, "sceKernelGetThreadExitStatus" },
	{ 0x207E, 0xD13BDE95, "sceKernelCheckThreadStack" },
	{ 0x207F, 0x52089CA1, "sceKernelGetThreadStackFreeSize" },
	{ 0x2080, 0x17C1684E, "sceKernelReferThreadStatus" },
	{ 0x2081, 0xFFC36A14, "sceKernelReferThreadRunStatus" },
	{ 0x2082, 0x627E6F3A, "sceKernelReferSystemStatus" },
	{ 0x2083, 0x94416130, "sceKernelGetThreadmanIdList" },
	{ 0x2084, 0x57CF62DD, "sceKernelGetThreadmanIdType" },
	{ 0x2085, 0x64D4540E, "sceKernelReferThreadProfiler" },
	{ 0x2086, 0x8218B4DD, "sceKernelReferGlobalProfiler" },

	{ 0x2087, 0x3251EA56, "sceIoPollAsync" },
	{ 0x2088, 0xE23EEC33, "sceIoWaitAsync" },
	{ 0x2089, 0x35DBD746, "sceIoWaitAsyncCB" },
	{ 0x208A, 0xCB05F8D6, "sceIoGetAsyncStat" },
	{ 0x208B, 0xB293727F, "sceIoChangeAsyncPriority" },
	{ 0x208C, 0xA12A0514, "sceIoSetAsyncCallback" },
	{ 0x208D, 0x810C4BC3, "sceIoClose" },
	{ 0x208E, 0xFF5940B6, "sceIoCloseAsync" },
	{ 0x208F, 0x109F50BC, "sceIoOpen" },
	{ 0x2090, 0x89AA9906, "sceIoOpenAsync" },
	{ 0x2091, 0x6A638D83, "sceIoRead" },
	{ 0x2092, 0xA0B5A7C2, "sceIoReadAsync" },
	{ 0x2093, 0x42EC03AC, "sceIoWrite" },
	{ 0x2094, 0x0FACAB19, "sceIoWriteAsync" },
	{ 0x2095, 0x27EB27B8, "sceIoLseek" },
	{ 0x2096, 0x71B19E77, "sceIoLseekAsync" },
	{ 0x2097, 0x68963324, "sceIoLseek32" },
	{ 0x2098, 0x1B385D8F, "sceIoLseek32Async" },
	{ 0x2099, 0x63632449, "sceIoIoctl" },
	{ 0x209A, 0xE95A012B, "sceIoIoctlAsync" },
	{ 0x209B, 0xB29DDF9C, "sceIoDopen" },
	{ 0x209C, 0xE3EB004C, "sceIoDread" },
	{ 0x209D, 0xEB092469, "sceIoDclose" },
	{ 0x209E, 0xF27A9C51, "sceIoRemove" },
	{ 0x209F, 0x06A70004, "sceIoMkdir" },
	{ 0x20A0, 0x1117C65F, "sceIoRmdir" },
	{ 0x20A1, 0x55F4717D, "sceIoChdir" },
	{ 0x20A2, 0xAB96437F, "sceIoSync" },
	{ 0x20A3, 0xACE946E8, "sceIoGetstat" },
	{ 0x20A4, 0xB8A740F4, "sceIoChstat" },
	{ 0x20A5, 0x779103A0, "sceIoRename" },
	{ 0x20A6, 0x54F5FB11, "sceIoDevctl" },
	{ 0x20A7, 0x08BD7374, "sceIoGetDevType" },
	{ 0x20A8, 0xB2A628C1, "sceIoAssign" },
	{ 0x20A9, 0x6D08A871, "sceIoUnassign" },
	{ 0x20AA, 0xE8BC6571, "sceIoCancel" },
	{ 0x20AB, 0x5C2BE2CC, "sceIoGetFdList" },

	{ 0x20AC, 0x3054D478, "sceKernelStdioRead" },
	{ 0x20AD, 0x0CBB0571, "sceKernelStdioLseek" },
	{ 0x20AE, 0xA46785C9, "sceKernelStdioSendChar" },
	{ 0x20AF, 0xA3B931DB, "sceKernelStdioWrite" },
	{ 0x20B0, 0x9D061C19, "sceKernelStdioClose" },
	{ 0x20B1, 0x924ABA61, "sceKernelStdioOpen" },
	{ 0x20B2, 0x172D316E, "sceKernelStdin" },
	{ 0x20B3, 0xA6BAB2E9, "sceKernelStdout" },
	{ 0x20B4, 0xF78BA90A, "sceKernelStderr" },
	{ 0x20B5, 0xBFA98062, "sceKernelDcacheInvalidateRange" },
	{ 0x20B6, 0xC2DF770E, "sceKernelIcacheInvalidateRange" },
	{ 0x20B7, 0xC8186A58, "sceKernelUtilsMd5Digest" },
	{ 0x20B8, 0x9E5C5086, "sceKernelUtilsMd5BlockInit" },
	{ 0x20B9, 0x61E1E525, "sceKernelUtilsMd5BlockUpdate" },
	{ 0x20BA, 0xB8D24E78, "sceKernelUtilsMd5BlockResult" },
	{ 0x20BB, 0x840259F1, "sceKernelUtilsSha1Digest" },
	{ 0x20BC, 0xF8FCD5BA, "sceKernelUtilsSha1BlockInit" },
	{ 0x20BD, 0x346F6DA8, "sceKernelUtilsSha1BlockUpdate" },
	{ 0x20BE, 0x585F1C09, "sceKernelUtilsSha1BlockResult" },
	{ 0x20BF, 0xE860E75E, "sceKernelUtilsMt19937Init" },
	{ 0x20C0, 0x06FB8A63, "sceKernelUtilsMt19937UInt" },
	{ 0x20C1, 0x37FB5C42, "sceKernelGetGPI" },
	{ 0x20C2, 0x6AD345D7, "sceKernelSetGPO" },
	{ 0x20C3, 0x91E4F6A7, "sceKernelLibcClock" },
	{ 0x20C4, 0x27CC57F0, "sceKernelLibcTime" },
	{ 0x20C5, 0x71EC4271, "sceKernelLibcGettimeofday" },
	{ 0x20C6, 0x79D1C3FA, "sceKernelDcacheWritebackAll" },
	{ 0x20C7, 0xB435DEC5, "sceKernelDcacheWritebackInvalidateAll" },
	{ 0x20C8, 0x3EE30821, "sceKernelDcacheWritebackRange" },
	{ 0x20C9, 0x34B9FA9E, "sceKernelDcacheWritebackInvalidateRange" },
	{ 0x20CA, 0x80001C4C, "sceKernelDcacheProbe" },
	{ 0x20CB, 0x16641D70, "sceKernelDcacheReadTag" },
	{ 0x20CC, 0x920F104A, "sceKernelIcacheInvalidateAll" },
	{ 0x20CD, 0x4FD31C9D, "sceKernelIcacheProbe" },
	{ 0x20CE, 0xFB05FAD0, "sceKernelIcacheReadTag" },
	{ 0x20CF, 0x977DE386, "sceKernelLoadModule" },
	{ 0x20D0, 0xB7F46618, "sceKernelLoadModuleByID" },
	{ 0x20D1, 0x710F61B5, "sceKernelLoadModuleMs" },
	{ 0x20D2, 0xF9275D98, "sceKernelLoadModuleBufferUsbWlan" },
	{ 0x20D3, 0x50F0C1EC, "KernelStartModule" },
	{ 0x20D4, 0xD1FF982A, "KernelStopModule" },
	{ 0x20D5, 0x2E0911AA, "KernelUnloadModule" },
	{ 0x20D6, 0xD675EBB8, "KernelSelfStopUnloadModule" },
	{ 0x20D7, 0xCC1D3699, "KernelStopUnloadSelfModule" },
	{ 0x20D8, 0x644395E2, "KernelGetModuleIdList" },
	{ 0x20D9, 0x748CBED9, "KernelQueryModuleInfo" },
	{ 0x20DA, 0xF0A26395, "sceKernelGetModuleId" },
	{ 0x20DB, 0xD8B73127, "sceKernelGetModuleIdByAddress" },
	{ 0x20DC, 0xA291F107, "KernelMaxFreeMemSize" },
	{ 0x20DD, 0xF919F628, "KernelTotalFreeMemSize" },
	{ 0x20DE, 0x237DBD4F, "KernelAllocPartitionMemory" },
	{ 0x20DF, 0xB6D61D02, "KernelFreePartitionMemory" },
	{ 0x20E0, 0x9D9A5BA1, "KernelGetBlockHeadAddr" },
	{ 0x20E1, 0x13A5ABEF, "sceKernelPrintf" },
	{ 0x20E2, 0x3FC9AE6A, "KernelDevkitVersion" },
	{ 0x20E3, 0xEADB1BD7, "KernelPowerLock" },
	{ 0x20E4, 0x3AEE7261, "KernelPowerUnlock" },
	{ 0x20E5, 0x090CCB3F, "KernelPowerTick" },
	{ 0x20E6, 0x3E0271D3, "sceKernelVolatileMemLock" },
	{ 0x20E7, 0xA14F40B2, "sceKernelVolatileMemTryLock" },
	{ 0x20E8, 0xA569E425, "sceKernelVolatileMemUnlock" },
	{ 0x20E9, 0xBD2F1094, "KernelLoadExec" },
	{ 0x20EA, 0x2AC9954B, "KernelExitGameWithStatus" },
	{ 0x20EB, 0x05572A5F, "KernelExitGame" },
	{ 0x20EC, 0x4AC57943, "KernelRegisterExitCallback" },
	{ 0x20ED, 0x617F3FE6, "DmacMemcpy" },
	{ 0x20EE, 0xD97F94D8, "DmacTryMemcpy" },

	{ 0x20EF, 0x1F6752AD, "GeEdramGetSize" },
	{ 0x20F0, 0xE47E40E4, "GeEdramGetAddr" },
	{ 0x20F1, 0xB77905EA, "GeEdramSetAddrTranslation" },
	{ 0x20F2, 0xDC93CFEF, "GeGetCmd" },
	{ 0x20F3, 0x57C8945B, "GeGetMtx" },
	{ 0x20F4, 0x438A385A, "GeSaveContext" },
	{ 0x20F5, 0x0BF608FB, "GeRestoreContext" },
	{ 0x20F6, 0xAB49E76A, "GeListEnQueue" },
	{ 0x20F7, 0x1C0D95A6, "GeListEnQueueHead" },
	{ 0x20F8, 0x5FB86AB0, "GeListDeQueue" },
	{ 0x20F9, 0xE0D68148, "GeListUpdateStallAddr" },
	{ 0x20FA, 0x03444EB4, "GeListSync" },
	{ 0x20FB, 0xB287BD61, "GeDrawSync" },
	{ 0x20FC, 0xB448EC0D, "GeBreak" },
	{ 0x20FD, 0x4C06E472, "GeContinue" },
	{ 0x20FE, 0xA4FC06A4, "GeSetCallback" },
	{ 0x20FF, 0x05DB22CE, "GeUnsetCallback" },

	{ 0x2100, 0xC41C2853, "RtcGetTickResolution" },
	{ 0x2101, 0x3F7AD767, "RtcGetCurrentTick" },
	{ 0x2102, 0x011F03C1, "sceRtcGetAccumulativeTime" },
	{ 0x2103, 0x029CA3B3, "Rtc_029CA3B3" },
	{ 0x2104, 0x4CFA57B0, "RtcGetCurrentClock" },
	{ 0x2105, 0xE7C27D1B, "RtcGetCurrentClockLocalTime" },
	{ 0x2106, 0x34885E0D, "RtcConvertUtcToLocalTime" },
	{ 0x2107, 0x779242A2, "RtcConvertLocalTimeToUTC" },
	{ 0x2108, 0x42307A17, "RtcIsLeapYear" },
	{ 0x2109, 0x05EF322C, "RtcGetDaysInMonth" },
	{ 0x210A, 0x57726BC1, "RtcGetDayOfWeek" },
	{ 0x210B, 0x4B1B5E82, "RtcCheckValid" },
	{ 0x210C, 0x3A807CC8, "RtcSetTime_t" },
	{ 0x210D, 0x27C4594C, "RtcGetTime_t" },
	{ 0x210E, 0xF006F264, "RtcSetDosTime" },
	{ 0x210F, 0x36075567, "RtcGetDosTime" },
	{ 0x2110, 0x7ACE4C04, "RtcSetWin32FileTime" },
	{ 0x2111, 0xCF561893, "RtcGetWin32FileTime" },
	{ 0x2112, 0x7ED29E40, "RtcSetTick" },
	{ 0x2113, 0x6FF40ACC, "RtcGetTick" },
	{ 0x2114, 0x9ED0AE87, "RtcCompareTick" },
	{ 0x2115, 0x44F45E05, "RtcTickAddTicks" },
	{ 0x2116, 0x26D25A5D, "RtcTickAddMicroseconds" },
	{ 0x2117, 0xF2A4AFE5, "RtcTickAddSeconds" },
	{ 0x2118, 0xE6605BCA, "RtcTickAddMinutes" },
	{ 0x2119, 0x26D7A24A, "RtcTickAddHours" },
	{ 0x211A, 0xE51B4B7A, "RtcTickAddDays" },
	{ 0x211B, 0xCF3A2CA8, "RtcTickAddWeeks" },
	{ 0x211C, 0xDBF74F1B, "RtcTickAddMonths" },
	{ 0x211D, 0x42842C77, "RtcTickAddYears" },
	{ 0x211E, 0xC663B3B9, "RtcFormatRFC2822" },
	{ 0x211F, 0x7DE6711B, "RtcFormatRFC2822LocalTime" },
	{ 0x2120, 0x0498FB3C, "RtcFormatRFC3339" },
	{ 0x2121, 0x27F98543, "RtcFormatRFC3339LocalTime" },
	{ 0x2122, 0xDFBC5F16, "RtcParseDateTime" },
	{ 0x2123, 0x28E1E988, "RtcParseRFC3339" },

	{ 0x2124, 0x8C1009B2, "AudioOutput" },
	{ 0x2125, 0x136CAF51, "AudioOutputBlocking" },
	{ 0x2126, 0xE2D56B2D, "AudioOutputPanned" },
	{ 0x2127, 0x13F592BC, "AudioOutputPannedBlocking" },
	{ 0x2128, 0x5EC81C55, "AudioChReserve" },
	{ 0x2129, 0x41EFADE7, "AudioOneshotOutput" },
	{ 0x212A, 0x6FC46853, "AudioChRelease" },
	{ 0x212B, 0xB011922F, "sceAudioGetChannelRestLength" },
	{ 0x212C, 0xCB2E439E, "AudioSetChannelDataLen" },
	{ 0x212D, 0x95FD0C2D, "AudioChangeChannelConfig" },
	{ 0x212E, 0xB7E1D8E7, "AudioChangeChannelVolume" },
	{ 0x212F, 0x38553111, "sceAudioSRCChReserve" },
	{ 0x2130, 0x5C37C0AE, "sceAudioSRCChRelease" },
	{ 0x2131, 0xE0727056, "sceAudioSRCOutputBlocking" },
	{ 0x2132, 0x086E5895, "AudioInputBlocking" },
	{ 0x2133, 0x6D4BEC68, "AudioInput" },
	{ 0x2134, 0xA708C6A6, "AudioGetInputLength" },
	{ 0x2135, 0x87B2E651, "AudioWaitInputEnd" },
	{ 0x2136, 0x7DE61688, "AudioInputInit" },
	{ 0x2137, 0xE926D3FB, "sceAudioInputInitEx" },
	{ 0x2138, 0xA633048E, "sceAudioPollInputEnd" },
	{ 0x2139, 0xE9D97901, "AudioGetChannelRestLen" },

	{ 0x213A, 0x0E20F177, "sceDisplaySetMode" },
	{ 0x213B, 0xDEA197D4, "sceDisplayGetMode" },
	{ 0x213C, 0xDBA6C4C4, "sceDisplayGetFramePerSec" },
	{ 0x213D, 0x7ED59BC4, "sceDisplaySetHoldMode" },
	{ 0x213E, 0xA544C486, "sceDisplaySetResumeMode" },
	{ 0x213F, 0x289D82FE, "sceDisplaySetFrameBuf" },
	{ 0x2140, 0xEEDA2E54, "sceDisplayGetFrameBuf" },
	{ 0x2141, 0xB4F378FA, "sceDisplayIsForeground" },
	{ 0x2142, 0x31C4BAA8, "sceDisplayGetBrightness" },
	{ 0x2143, 0x9C6EAAD7, "sceDisplayGetVcount" },
	{ 0x2144, 0x4D4E10EC, "sceDisplayIsVblank" },
	{ 0x2145, 0x36CDFADE, "sceDisplayWaitVblank" },
	{ 0x2146, 0x8EB9EC49, "sceDisplayWaitVblankCB" },
	{ 0x2147, 0x984C27E7, "sceDisplayWaitVblankStart" },
	{ 0x2148, 0x46F186C3, "sceDisplayWaitVblankStartCB" },
	{ 0x2149, 0x773DD3A3, "sceDisplayGetCurrentHcount" },
	{ 0x214A, 0x210EAB3A, "sceDisplayGetAccumulatedHcount" },
	{ 0x214B, 0xA83EF139, "sceDisplayAdjustAccumulatedHcount" },

	{ 0x214C, 0x6A2774F3, "sceCtrlSetSamplingCycle" },
	{ 0x214D, 0x02BAAD91, "sceCtrlGetSamplingCycle" },
	{ 0x214E, 0x1F4011E6, "sceCtrlSetSamplingMode" },
	{ 0x214F, 0xDA6B76A1, "sceCtrlGetSamplingMode" },
	{ 0x2150, 0x3A622550, "sceCtrlPeekBufferPositive" },
	{ 0x2151, 0xC152080A, "sceCtrlPeekBufferNegative" },
	{ 0x2152, 0x1F803938, "sceCtrlReadBufferPositive" },
	{ 0x2153, 0x60B81F86, "sceCtrlReadBufferNegative" },
	{ 0x2154, 0xB1D0E5CD, "sceCtrlPeekLatch" },
	{ 0x2155, 0x0B588501, "sceCtrlReadLatch" },
	{ 0x2156, 0xA7144800, "sceCtrlSetIdleCancelThreshold" },
	{ 0x2157, 0x687660FA, "sceCtrlGetIdleCancelThreshold" },
	{ 0x2158, 0x348D99D4, "sceCtrl_348D99D4" },
	{ 0x2159, 0xAF5960F3, "sceCtrl_AF5960F3" },
	{ 0x215A, 0xA68FD260, "sceCtrlClearRapidFire" },
	{ 0x215B, 0x6841BE1A, "sceCtrlSetRapidFire" },

	{ 0x215C, 0xC7154136, "sceHprmRegisterCallback" },
	{ 0x215D, 0x444ED0B7, "sceHprmUnregisterCallback" },
	{ 0x215E, 0x71B5FB67, "sceHprm_71B5FB67" },
	{ 0x215F, 0x208DB1BD, "sceHprmIsRemoteExist" },
	{ 0x2160, 0x7E69EDA4, "HprmIsHeadphoneExist" },
	{ 0x2161, 0x219C58F1, "HprmIsMicrophoneExist" },
	{ 0x2162, 0x1910B327, "HprmPeekCurrentKey" },
	{ 0x2163, 0x2BCEC83E, "HprmPeekLatch" },
	{ 0x2164, 0x40D2F9F0, "HprmReadLatch" },

	{ 0x2165, 0x2B51FE2F, "Power_2B51FE2F" },
	{ 0x2166, 0x442BFBAC, "Power_442BFBAC" },
	{ 0x2167, 0xEFD3C963, "PowerTick" },
	{ 0x2168, 0xEDC13FE5, "PowerGetIdleTimer" },
	{ 0x2169, 0x7F30B3B1, "PowerIdleTimerEnable" },
	{ 0x216A, 0x972CE941, "PowerIdleTimerDisable" },
	{ 0x216B, 0x27F3292C, "PowerBatteryUpdateInfo" },
	{ 0x216C, 0xE8E4E204, "Power_E8E4E204" },
	{ 0x216D, 0xB999184C, "PowerGetLowBatteryCapacity" },
	{ 0x216E, 0x87440F5E, "PowerIsPowerOnline" },
	{ 0x216F, 0x0AFD0D8B, "PowerIsBatteryExist" },
	{ 0x2170, 0x1E490401, "PowerIsBatteryCharging" },
	{ 0x2171, 0xB4432BC8, "PowerGetBatteryChargingStatus" },
	{ 0x2172, 0xD3075926, "PowerIsLowBattery" },
	{ 0x2173, 0x78A1A796, "Power_78A1A796" },
	{ 0x2174, 0x94F5A53F, "PowerGetBatteryRemainCapacity" },
	{ 0x2175, 0xFD18A0FF, "Power_FD18A0FF" },
	{ 0x2176, 0x2085D15D, "PowerGetBatteryLifePercent" },
	{ 0x2177, 0x8EFB3FA2, "PowerGetBatteryLifeTime" },
	{ 0x2178, 0x28E12023, "PowerGetBatteryTemp" },
	{ 0x2179, 0x862AE1A6, "PowerGetBatteryElec" },
	{ 0x217A, 0x483CE86B, "PowerGetBatteryVolt" },
	{ 0x217B, 0x23436A4A, "Power_23436A4A" },
	{ 0x217C, 0x0CD21B1F, "Power_0CD21B1F" },
	{ 0x217D, 0x165CE085, "Power_165CE085" },
	{ 0x217E, 0x23C31FFE, "Power_23C31FFE" },
	{ 0x217F, 0xFA97A599, "Power_FA97A599" },
	{ 0x2180, 0xB3EDD801, "Power_B3EDD801" },
	{ 0x2181, 0xD6D016EF, "PowerLock" },
	{ 0x2182, 0xCA3D34C1, "PowerUnlock" },
	{ 0x2183, 0xDB62C9CF, "PowerCancelRequest" },
	{ 0x2184, 0x7FA406DD, "PowerIsRequest" },
	{ 0x2185, 0x2B7C7CF4, "PowerRequestStandby" },
	{ 0x2186, 0xAC32C9CC, "PowerRequestSuspend" },
	{ 0x2187, 0x2875994B, "Power_2875994B" },
	{ 0x2188, 0x3951AF53, "PowerEncodeUBattery" },
	{ 0x2189, 0x0074EF9B, "PowerGetResumeCount" },
	{ 0x218A, 0x04B7766E, "PowerRegisterCallback" },
	{ 0x218B, 0xDFA8BAF8, "PowerUnregisterCallback" },
	{ 0x218C, 0xDB9D28DD, "PowerUnregitserCallback" },
	{ 0x218D, 0x843FBF43, "PowerSetCpuClockFrequency" },
	{ 0x218E, 0xB8D7B3FB, "PowerSetBusClockFrequency" },
	{ 0x218F, 0xFEE03A2F, "PowerGetCpuClockFrequency" },
	{ 0x2190, 0x478FE6F5, "PowerGetBusClockFrequency" },
	{ 0x2191, 0xFDB5BFE9, "PowerGetCpuClockFrequencyInt" },
	{ 0x2192, 0xBD681969, "PowerGetBusClockFrequencyInt" },
	{ 0x2193, 0x34F9C463, "Power_34F9C463" },
	{ 0x2194, 0xB1A52C83, "PowerGetCpuClockFrequencyFloat" },
	{ 0x2195, 0x9BADB3EB, "PowerGetBusClockFrequencyFloat" },
	{ 0x2196, 0xEA382A27, "Power_EA382A27" },
	{ 0x2197, 0x737486F2, "PowerSetClockFrequency" },

	{ 0x2198, 0xAE5DE6AF, "UsbStart" },
	{ 0x2199, 0xC2464FA0, "UsbStop" },
	{ 0x219A, 0xC21645A4, "UsbGetState" },
	{ 0x219B, 0x4E537366, "UsbGetDrvList" },
	{ 0x219C, 0x112CC951, "UsbGetDrvState" },
	{ 0x219D, 0x586DB82C, "UsbActivate" },
	{ 0x219E, 0xC572A9C8, "UsbDeactivate" },
	{ 0x219F, 0x5BE0E002, "UsbWaitState" },
	{ 0x21A0, 0x1C360735, "UsbWaitCancel" },

	{ 0x21A1, 0xC69BEBCE, "OpenPSIDGetOpenPSID" },
	{ 0x21A2, 0x71EEF62D, "SircsSend" },

	{ 0x21A3, 0x46EBB729, "UmdCheckMedium" },
	{ 0x21A4, 0xC6183D47, "UmdActivate" },
	{ 0x21A5, 0xE83742BA, "UmdDeactivate" },
	{ 0x21A6, 0x8EF08FCE, "UmdWaitDriveStat" },
	{ 0x21A7, 0x56202973, "UmdWaitDriveStatWithTimer" },
	{ 0x21A8, 0x4A9E5E29, "UmdWaitDriveStatCB" },
	{ 0x21A9, 0x6AF9B50A, "UmdCancelWaitDriveStat" },
	{ 0x21AA, 0x6B4A146C, "UmdGetDriveStat" },
	{ 0x21AB, 0x20628E6F, "UmdGetErrorStat" },
	{ 0x21AC, 0x340B7686, "UmdGetDiscInfo" },
	{ 0x21AD, 0xAEE7404D, "UmdRegisterUMDCallBack" },
	{ 0x21AE, 0xBD2BDE07, "UmdUnRegisterUMDCallBack" },

	{ 0x21AF, 0x93440B11, "WlanDevIsPowerOn" },
	{ 0x21B0, 0xD7763699, "WlanGetSwitchState" },
	{ 0x21B1, 0x0C622081, "WlanGetEtherAddr" },
	{ 0x21B2, 0x482CAE9A, "WlanDevAttach" },
	{ 0x21B3, 0xC9A8CAB7, "WlanDevDetach" },
	{ 0x21B4, 0x19E51F54, "WlanDrv_lib_19E51F54" },
	{ 0x21B5, 0x5E7C8D94, "WlanDevIsGameMode" },
	{ 0x21B6, 0x5ED4049A, "WlanGPPrevEstablishActive" },
	{ 0x21B7, 0xB4D7CB74, "WlanGPSend" },
	{ 0x21B8, 0xA447103A, "WlanGPRecv" },
	{ 0x21B9, 0x9658C9F7, "WlanGPRegisterCallback" },
	{ 0x21BA, 0x4C7F62E0, "WlanGPUnRegisterCallback" },
	{ 0x21BB, 0x81579D36, "WlanDrv_lib_81579D36" },
	{ 0x21BC, 0x5BAA1FE5, "WlanDrv_lib_5BAA1FE5" },
	{ 0x21BD, 0x4C14BACA, "WlanDrv_lib_4C14BACA" },
	{ 0x21BE, 0x2D0FAE4E, "WlanDrv_lib_2D0FAE4E" },
	{ 0x21BF, 0x56F467CA, "WlanDrv_lib_56F467CA" },
	{ 0x21C0, 0xFE8A0B46, "WlanDrv_lib_FE8A0B46" },
	{ 0x21C1, 0x40B0AA4A, "WlanDrv_lib_40B0AA4A" },
	{ 0x21C2, 0x7FF54BD2, "WlanDevSetGPIO" },
	{ 0x21C3, 0x05FE320C, "WlanDevGetStateGPIO" },
	{ 0x21C4, 0x8D5F551B, "WlanDrv_lib_8D5F551B" },

	{ 0x21C5, 0x8986295E, "VaudioOutputBlocking" },
	{ 0x21C6, 0x03B6807D, "VaudioChReserve" },
	{ 0x21C7, 0x67585DFD, "VaudioChRelease" },
	{ 0x21C8, 0x346FBE94, "Vaudio_346FBE94" },

	{ 0x21C9, 0x9B25EDF1, "RegExit" },
	{ 0x21CA, 0x92E41280, "RegOpenRegistry" },
	{ 0x21CB, 0xFA8A5739, "RegCloseRegistry" },
	{ 0x21CC, 0xDEDA92BF, "RegRemoveRegistry" },
	{ 0x21CD, 0x1D8A762E, "Reg_1D8A762E" },
	{ 0x21CE, 0x0CAE832B, "Reg_0CAE832B" },
	{ 0x21CF, 0x39461B4D, "RegFlushRegistry" },
	{ 0x21D0, 0x0D69BF40, "sceReg_0D69BF40" },
	{ 0x21D1, 0x57641A81, "sceRegCreateKey" },
	{ 0x21D2, 0x17768E14, "sceRegSetKeyValue" },
	{ 0x21D3, 0xD4475AA8, "sceRegGetKeyInfo" },
	{ 0x21D4, 0x28A8E98A, "sceRegGetKeyValue" },
	{ 0x21D5, 0x2C0DB9DD, "sceRegGetKeysNum" },
	{ 0x21D6, 0x2D211135, "sceRegGetKeys" },
	{ 0x21D7, 0x4CA16893, "sceReg_4CA16893" },
	{ 0x21D8, 0x3615BC87, "sceRegRemoveKey" },
	{ 0x21D9, 0xC5768D02, "sceRegKickBackDiscover" },
	{ 0x21DA, 0x30BE0259, "sceRegGetKeyValueByName" },

	{ 0x21DB, 0xC492F751, "sceUtilityGameSharingInitStart" },
	{ 0x21DC, 0xEFC6F80F, "sceUtilityGameSharingShutdownStart" },
	{ 0x21DD, 0x7853182D, "sceUtilityGameSharingUpdate" },
	{ 0x21DE, 0x946963F3, "sceUtilityGameSharingGetStatus" },

	{ 0x21DF, 0x3AD50AE7, "sceNetplayDialogInitStart" },
	{ 0x21E0, 0xBC6B6296, "sceNetplayDialogShutdownStart" },
	{ 0x21E1, 0x417BED54, "sceNetplayDialogUpdate" },
	{ 0x21E2, 0xB6CEE597, "sceNetplayDialogGetStatus" },

	{ 0x21E3, 0x4DB1E739, "sceUtilityNetconfInitStart" },
	{ 0x21E4, 0xF88155F6, "sceUtilityNetconfShutdownStart" },
	{ 0x21E5, 0x91E70E35, "sceUtilityNetconfUpdate" },
	{ 0x21E6, 0x6332AA39, "sceUtilityNetconfGetStatus" },
	{ 0x21E7, 0x50C4CD57, "sceUtilitySavedataInitStart" },
	{ 0x21E8, 0x9790B33C, "sceUtilitySavedataShutdownStart" },
	{ 0x21E9, 0xD4B95FFB, "sceUtilitySavedataUpdate" },
	{ 0x21EA, 0x8874DBE0, "sceUtilitySavedataGetStatus" },
	{ 0x21EB, 0x2995D020, "sceUtility_2995D020" },
	{ 0x21EC, 0xB62A4061, "sceUtility_B62A4061" },
	{ 0x21ED, 0xED0FAD38, "sceUtility_ED0FAD38" },
	{ 0x21EE, 0x88BC7406, "sceUtility_88BC7406" },
	{ 0x21EF, 0x2AD8E239, "sceUtilityMsgDialogInitStart" },
	{ 0x21F0, 0x67AF3428, "sceUtilityMsgDialogShutdownStart" },
	{ 0x21F1, 0x95FC253B, "sceUtilityMsgDialogUpdate" },
	{ 0x21F2, 0x9A1C91D7, "sceUtilityMsgDialogGetStatus" },
	{ 0x21F3, 0xF6269B82, "sceUtilityOskInitStart" },
	{ 0x21F4, 0x3DFAEBA9, "sceUtilityOskShutdownStart" },
	{ 0x21F5, 0x4B85C861, "sceUtilityOskUpdate" },
	{ 0x21F6, 0xF3F76017, "sceUtilityOskGetStatus" },
	{ 0x21F7, 0x45C18506, "sceUtilitySetSystemParamInt" },
	{ 0x21F8, 0x41E30674, "sceUtilitySetSystemParamString" },
	{ 0x21F9, 0xA5DA2406, "sceUtilityGetSystemParamInt" },
	{ 0x21FA, 0x34B78343, "sceUtilityGetSystemParamString" },
	{ 0x21FB, 0x5EEE6548, "sceUtilityCheckNetParam" },
	{ 0x21FC, 0x434D4B3A, "sceUtilityGetNetParam" },
	{ 0x21FD, 0x17CB4D96, "sceUtility_private_17CB4D96" },
	{ 0x21FE, 0xEE7AC503, "sceUtility_private_EE7AC503" },
	{ 0x21FF, 0x5FF96ED3, "sceUtility_private_5FF96ED3" },
	{ 0x2200, 0x9C9DD5BC, "sceUtility_private_9C9DD5BC" },
	{ 0x2201, 0x4405BA38, "sceUtility_private_4405BA38" },
	{ 0x2202, 0x1DFA62EF, "sceUtility_private_1DFA62EF" },
	{ 0x2203, 0x680C0EA8, "sceUtilityDialogSetStatus" },
	{ 0x2204, 0xB222E887, "sceUtilityDialogGetType" },
	{ 0x2205, 0x4F2206BC, "sceUtilityDialogGetParam" },
	{ 0x2206, 0xEF5BC2D1, "sceUtility_private_EF5BC2D1" },
	{ 0x2207, 0xE01FE32A, "sceUtilityDialogGetSpeed" },
	{ 0x2208, 0x19461966, "sceUtility_private_19461966" },
	{ 0x2209, 0x6F923BD3, "sceUtilityDialogSetThreadId" },
	{ 0x220A, 0xA5168A5D, "sceUtilityDialogLoadModule" },
	{ 0x220B, 0x3CEAE1DF, "sceUtilityDialogPowerLock" },
	{ 0x220C, 0x56BEDCA4, "sceUtilityDialogPowerUnlock" },
	{ 0x220D, 0x072DEBF2, "sceUtilityCreateNetParam" },
	{ 0x220E, 0x9CE50172, "sceUtilityDeleteNetParam" },
	{ 0x220F, 0xFB0C4840, "sceUtilityCopyNetParam" },
	{ 0x2210, 0xFC4516F3, "sceUtilitySetNetParam" },

	// Added random syscalls numbers.
	// sceAtrac3plus found in puzzle booble
	{ 0x2211, 0xD1F59FDB, "sceAtracStartEntry" },
	{ 0x2212, 0xD5C28CC0, "sceAtracEndEntry" },
	{ 0x2213, 0x780F88D1, "sceAtracGetAtracID" },
	{ 0x2214, 0x61EB33F5, "sceAtracReleaseAtracID" },
	{ 0x2215, 0x0E2A73AB, "sceAtracSetData" },
	{ 0x2216, 0x3F6E26B5, "sceAtracSetHalfwayBuffer" },
	{ 0x2217, 0x7A20E7AF, "sceAtracSetDataAndGetID" },
	{ 0x2218, 0x0FAE370E, "sceAtracSetHalfwayBufferAndGetID" },
	{ 0x2219, 0x6A8C3CD5, "sceAtracDecodeData" },
	{ 0x221A, 0x9AE849A7, "sceAtracGetRemainFrame" },
	{ 0x221B, 0x5D268707, "sceAtracGetStreamDataInfo" },
	{ 0x221C, 0x7DB31251, "sceAtracAddStreamData" },
	{ 0x221D, 0x83E85EA0, "sceAtracGetSecondBufferInfo" },
	{ 0x221E, 0x83BF7AFD, "sceAtracSetSecondBuffer" },
	{ 0x221F, 0xE23E3A35, "sceAtracGetNextDecodePosition" },
	{ 0x2220, 0xA2BBA8BE, "sceAtracGetSoundSample" },
	{ 0x2221, 0x31668BAA, "sceAtracGetChannel" },
	{ 0x2222, 0xD6A5F2F7, "sceAtracGetMaxSample" },
	{ 0x2223, 0x36FAABFB, "sceAtracGetNextSample" },
	{ 0x2224, 0xA554A158, "sceAtracGetBitrate" },
	{ 0x2225, 0xFAA4F89B, "sceAtracGetLoopStatus" },
	{ 0x2226, 0x868120B5, "sceAtracSetLoopNum" },
	{ 0x2227, 0xCA3CA3D2, "sceAtracGetBufferInfoForReseting" },
	{ 0x2228, 0x644E5607, "sceAtracResetPlayPosition" },
	{ 0x2229, 0xE88F759B, "sceAtracGetInternalErrorInfo" },

	{ 0x222A, 0x39AF39A6, "sceNetInit" },
	{ 0x222B, 0x281928A9, "sceNetTerm" },
	{ 0x222C, 0x50647530, "sceNetFreeThreadinfo" },
	{ 0x222D, 0xAD6844C6, "sceNetThreadAbort" },
	{ 0x222E, 0x89360950, "sceNetEtherNtostr" },
	{ 0x222F, 0xD27961C9, "sceNetEtherStrton" },
	{ 0x2230, 0x0BF0A3AE, "sceNetGetLocalEtherAddr" },
	{ 0x2231, 0xCC393E48, "sceNetGetMallocStat" },

	// InterruptManagerForKernel
	{ 0x2232, 0x092968F4, "sceKernelCpuSuspendIntr" },
	{ 0x2233, 0x5F10D406, "sceKernelCpuResumeIntr" },
	{ 0x2234, 0x3B84732D, "sceKernelCpuResumeIntrWithSync" },
	{ 0x2235, 0xFE28C6D9, "sceKernelIsIntrContext" },

	{ 0x2236, 0x53991063, "InterruptManagerForKernel_53991063" },
	{ 0x2237, 0x468BC716, "sceKernelGetInterruptExitCount" },
	{ 0x2238, 0x43CD40EF, "ReturnToThread" },
	{ 0x2239, 0x85F7766D, "SaveThreadContext" },
	{ 0x223A, 0x02314986, "sceKernelCpuEnableIntr" },
	{ 0x223B, 0x00B6B0F3, "QueryInterruptManCB" },
	{ 0x223C, 0x58DD8978, "sceKernelRegisterIntrHandler" },
	{ 0x223D, 0x15894D0B, "InterruptManagerForKernel_15894D0B" },
	{ 0x223E, 0xF987B1F0, "sceKernelReleaseIntrHandler" },
	{ 0x223F, 0xB5A15B30, "sceKernelSetIntrLevel" },
	{ 0x2240, 0x43A7BBDC, "InterruptManagerForKernel_43A7BBDC" },
	{ 0x2241, 0x02475AAF, "sceKernelIsInterruptOccurred" },
	{ 0x2242, 0x4D6E7305, "sceKernelEnableIntr" },
	{ 0x2243, 0x750E2507, "sceKernelSuspendIntr" },
	{ 0x2244, 0xD774BA45, "sceKernelDisableIntr" },
	{ 0x2245, 0x494D6D2B, "sceKernelResumeIntr" },
	{ 0x2246, 0x2CD783A1, "RegisterContextHooks" },
	{ 0x2247, 0x55242A8B, "ReleaseContextHooks" },
	{ 0x2248, 0x27BC9A45, "UnSupportIntr" },
	{ 0x2249, 0x0E224D66, "SupportIntr" },
	{ 0x224A, 0x272766F8, "sceKernelRegisterDebuggerIntrHandler" },
	{ 0x224B, 0xB386A459, "sceKernelReleaseDebuggerIntrHandler" },
	{ 0x224C, 0xCDC86B64, "sceKernelCallSubIntrHandler" },
	{ 0x224D, 0xD6878EB6, "sceKernelGetUserIntrStack" },
	{ 0x224E, 0xF4454E44, "sceKernelCallUserIntrHandler" },
	{ 0x224F, 0x30C08374, "sceKernelGetCpuClockCounter" },
	{ 0x2250, 0x35634A64, "sceKernelGetCpuClockCounterWide" },
	{ 0x2251, 0x2DC9709B, "_sceKernelGetCpuClockCounterLow" },
	{ 0x2252, 0xE9E652A9, "_sceKernelGetCpuClockCounterHigh" },
	{ 0x2253, 0x0FC68A56, "sceKernelSetPrimarySyscallHandler" },
	{ 0x2254, 0xF4D443F3, "sceKernelRegisterSystemCallTable" },
	{ 0x2255, 0x8B61808B, "sceKernelQuerySystemCall" },

	{ 0x2256, 0x21FF80E4, "sceMpegQueryStreamOffset" },
	{ 0x2257, 0x611E9E11, "sceMpegQueryStreamSize" },
	{ 0x2258, 0x682A619B, "sceMpegInit" },
	{ 0x2259, 0x874624D6, "sceMpegFinish" },
	{ 0x225A, 0xC132E22F, "sceMpegQueryMemSize" },
	{ 0x225B, 0xD8C5F121, "sceMpegCreate" },
	{ 0x225C, 0x606A4649, "sceMpegDelete" },
	{ 0x225D, 0x42560F23, "sceMpegRegistStream" },
	{ 0x225E, 0x591A4AA2, "sceMpegUnRegistStream" },
	{ 0x225F, 0xA780CF7E, "sceMpegMallocAvcEsBuf" },
	{ 0x2260, 0xCEB870B1, "sceMpegFreeAvcEsBuf" },
	{ 0x2261, 0xF8DCB679, "sceMpegQueryAtracEsSize" },
	{ 0x2262, 0xC02CF6B5, "sceMpegQueryPcmEsSize" },
	{ 0x2263, 0x167AFD9E, "sceMpegInitAu" },
	{ 0x2264, 0x234586AE, "sceMpegChangeGetAvcAuMode" },
	{ 0x2265, 0x9DCFB7EA, "sceMpegChangeGetAuMode" },
	{ 0x2266, 0xFE246728, "sceMpegGetAvcAu" },
	{ 0x2267, 0x8C1E027D, "sceMpegGetPcmAu" },
	{ 0x2268, 0xE1CE83A7, "sceMpegGetAtracAu" },
	{ 0x2269, 0x500F0429, "sceMpegFlushStream" },
	{ 0x226A, 0x707B7629, "sceMpegFlushAllStream" },
	{ 0x226B, 0x0E3C2E9D, "sceMpegAvcDecode" },
	{ 0x226C, 0x0F6C18D7, "sceMpegAvcDecodeDetail" },
	{ 0x226D, 0xA11C7026, "sceMpegAvcDecodeMode" },
	{ 0x226E, 0x740FCCD1, "sceMpegAvcDecodeStop" },
	{ 0x226F, 0x4571CC64, "sceMpegAvcDecodeFlush" },
	{ 0x2270, 0x211A057C, "sceMpegAvcQueryYCbCrSize" },
	{ 0x2271, 0x67179B1B, "sceMpegAvcInitYCbCr" },
	{ 0x2272, 0xF0EB1125, "sceMpegAvcDecodeYCbCr" },
	{ 0x2273, 0xF2930C9C, "sceMpegAvcDecodeStopYCbCr" },
	{ 0x2274, 0x31BD0272, "sceMpegAvcCsc" },
	{ 0x2275, 0x800C44DF, "sceMpegAtracDecode" },
	{ 0x2276, 0xD7A29F46, "sceMpegRingbufferQueryMemSize" },
	{ 0x2277, 0x37295ED8, "sceMpegRingbufferConstruct" },
	{ 0x2278, 0x13407F13, "sceMpegRingbufferDestruct" },
	{ 0x2279, 0xB240A59E, "sceMpegRingbufferPut" },
	{ 0x227A, 0xB5F6DC87, "sceMpegRingbufferAvailableSize" },
	{ 0x227B, 0x11CAB459, "sceMpeg_11CAB459" },
	{ 0x227C, 0x3C37A7A6, "sceMpegNextAvcRpAu" },
	{ 0x227D, 0xB27711A8, "sceMpeg_B27711A8" },
	{ 0x227E, 0xD4DD6E75, "sceMpeg_D4DD6E75" },
	{ 0x227F, 0xC345DED2, "sceMpeg_C345DED2" },
	{ 0x2280, 0xAB0E9556, "sceMpeg_AB0E9556" },
	{ 0x2281, 0xCF3547A2, "sceMpegAvcDecodeDetail2" },
	{ 0x2282, 0x988E9E12, "sceMpeg_988E9E12" },

	{ 0x2283, 0x17943399, "sceNetInetInit" },
	{ 0x2284, 0xA9ED66B9, "sceNetInetTerm" },
	{ 0x2285, 0xDB094E1B, "sceNetInetAccept" },
	{ 0x2286, 0x1A33F9AE, "sceNetInetBind" },
	{ 0x2287, 0x8D7284EA, "sceNetInetClose" },
	{ 0x2288, 0x805502DD, "sceNetInetCloseWithRST" },
	{ 0x2289, 0x410B34AA, "sceNetInetConnect" },
	{ 0x228A, 0xE247B6D6, "sceNetInetGetpeername" },
	{ 0x228B, 0x162E6FD5, "sceNetInetGetsockname" },
	{ 0x228C, 0x4A114C7C, "sceNetInetGetsockopt" },
	{ 0x228D, 0xD10A1A7A, "sceNetInetListen" },
	{ 0x228E, 0xFAABB1DD, "sceNetInetPoll" },
	{ 0x228F, 0xCDA85C99, "sceNetInetRecv" },
	{ 0x2290, 0xC91142E4, "sceNetInetRecvfrom" },
	{ 0x2291, 0xEECE61D2, "sceNetInetRecvmsg" },
	{ 0x2292, 0x5BE8D595, "sceNetInetSelect" },
	{ 0x2293, 0x7AA671BC, "sceNetInetSend" },
	{ 0x2294, 0x05038FC7, "sceNetInetSendto" },
	{ 0x2295, 0x774E36F4, "sceNetInetSendmsg" },
	{ 0x2296, 0x2FE71FE7, "sceNetInetSetsockopt" },
	{ 0x2297, 0x4CFE4E56, "sceNetInetShutdown" },
	{ 0x2298, 0x8B7B220F, "sceNetInetSocket" },
	{ 0x2299, 0x80A21ABD, "sceNetInetSocketAbort" },
	{ 0x229A, 0xFBABE411, "sceNetInetGetErrno" },
	{ 0x229B, 0xB3888AD4, "sceNetInetGetTcpcbstat" },
	{ 0x229C, 0x39B0C7D3, "sceNetInetGetUdpcbstat" },
	{ 0x229D, 0xB75D5B0A, "sceNetInetInetAddr" },
	{ 0x229E, 0x1BDF5D13, "sceNetInetInetAton" },
	{ 0x229F, 0xD0792666, "sceNetInetInetNtop" },
	{ 0x22A0, 0xE30B8C19, "sceNetInetInetPton" },

	{ 0x22A1, 0x381BD9E7, "sceImposeHomeButton" },
	{ 0x22A2, 0x5595A71A, "sceImposeSetHomePopup" },
	{ 0x22A3, 0x0F341BE4, "sceImposeGetHomePopup" },
	{ 0x22A4, 0x72189C48, "sceImposeSetUMDPopup" },
	{ 0x22A5, 0xE0887BC8, "sceImposeGetUMDPopup" },
	{ 0x22A6, 0x36AA6E91, "sceImposeSetLanguageMode" },
	{ 0x22A7, 0x24FD7BCF, "sceImposeGetLanguageMode" },
	{ 0x22A8, 0x8C943191, "sceImposeGetBatteryIconStatus" },

	{ 0x22A9, 0xE1D621D7, "sceNetAdhocInit" },
	{ 0x22AA, 0xA62C6F57, "sceNetAdhocTerm" },
	{ 0x22AB, 0x7A662D6B, "sceNetAdhocPollSocket" },
	{ 0x22AC, 0x73BFD52D, "sceNetAdhocSetSocketAlert" },
	{ 0x22AD, 0x4D2CE199, "sceNetAdhocGetSocketAlert" },
	{ 0x22AE, 0x6F92741B, "sceNetAdhocPdpCreate" },
	{ 0x22AF, 0xABED3790, "sceNetAdhocPdpSend" },
	{ 0x22B0, 0xDFE53E03, "sceNetAdhocPdpRecv" },
	{ 0x22B1, 0x7F27BB5E, "sceNetAdhocPdpDelete" },
	{ 0x22B2, 0xC7C1FC57, "sceNetAdhocGetPdpStat" },
	{ 0x22B3, 0x877F6D66, "sceNetAdhocPtpOpen" },
	{ 0x22B4, 0xFC6FC07B, "sceNetAdhocPtpConnect" },
	{ 0x22B5, 0xE08BDAC1, "sceNetAdhocPtpListen" },
	{ 0x22B6, 0x9DF81198, "sceNetAdhocPtpAccept" },
	{ 0x22B7, 0x4DA4C788, "sceNetAdhocPtpSend" },
	{ 0x22B8, 0x8BEA2B3E, "sceNetAdhocPtpRecv" },
	{ 0x22B9, 0x9AC2EEAC, "sceNetAdhocPtpFlush" },
	{ 0x22BA, 0x157E6225, "sceNetAdhocPtpClose" },
	{ 0x22BB, 0xB9685118, "sceNetAdhocGetPtpStat" },
	{ 0x22BC, 0x7F75C338, "sceNetAdhocGameModeCreateMaster" },
	{ 0x22BD, 0x3278AB0C, "sceNetAdhocGameModeCreateReplica" },
	{ 0x22BE, 0x98C204C8, "sceNetAdhocGameModeUpdateMaster" },
	{ 0x22BF, 0xFA324B4E, "sceNetAdhocGameModeUpdateReplica" },
	{ 0x22C0, 0xA0229362, "sceNetAdhocGameModeDeleteMaster" },
	{ 0x22C1, 0x0B2228E9, "sceNetAdhocGameModeDeleteReplica" },
	{ 0x22C2, 0xE26F226E, "sceNetAdhocctlInit" },
	{ 0x22C3, 0x9D689E13, "sceNetAdhocctlTerm" },
	{ 0x22C4, 0x0AD043ED, "sceNetAdhocctlConnect" },
	{ 0x22C5, 0xEC0635C1, "sceNetAdhocctlCreate" },
	{ 0x22C6, 0x5E7F79C9, "sceNetAdhocctlJoin" },
	{ 0x22C7, 0x08FFF7A0, "sceNetAdhocctlScan" },
	{ 0x22C8, 0x34401D65, "sceNetAdhocctlDisconnect" },
	{ 0x22C9, 0x20B317A0, "sceNetAdhocctlAddHandler" },
	{ 0x22CA, 0x6402490B, "sceNetAdhocctlDelHandler" },
	{ 0x22CB, 0x75ECD386, "sceNetAdhocctlGetState" },
	{ 0x22CC, 0x362CBE8F, "sceNetAdhocctlGetAdhocId" },
	{ 0x22CD, 0xE162CB14, "sceNetAdhocctlGetPeerList" },
	{ 0x22CE, 0x99560ABE, "sceNetAdhocctlGetAddrByName" },
	{ 0x22CF, 0x8916C003, "sceNetAdhocctlGetNameByAddr" },
	{ 0x22D0, 0xDED9D28E, "sceNetAdhocctlGetParameter" },
	{ 0x22D1, 0x81AEE1BE, "sceNetAdhocctlGetScanInfo" },
	{ 0x22D2, 0xA5C055CE, "sceNetAdhocctlCreateEnterGameMode" },
	{ 0x22D3, 0x1FF89745, "sceNetAdhocctlJoinEnterGameMode" },
	{ 0x22D4, 0xCF8E084D, "sceNetAdhocctlExitGameMode" },
	{ 0x22D5, 0x5A014CE0, "sceNetAdhocctlGetGameModeInfo" },
	{ 0x22D6, 0x2A2A1E07, "sceNetAdhocMatchingInit" },
	{ 0x22D7, 0x7945ECDA, "sceNetAdhocMatchingTerm" },
	{ 0x22D8, 0xCA5EDA6F, "sceNetAdhocMatchingCreate" },
	{ 0x22D9, 0x93EF3843, "sceNetAdhocMatchingStart" },
	{ 0x22DA, 0x32B156B3, "sceNetAdhocMatchingStop" },
	{ 0x22DB, 0xF16EAF4F, "sceNetAdhocMatchingDelete" },
	{ 0x22DC, 0x5E3D4B79, "sceNetAdhocMatchingSelectTarget" },
	{ 0x22DE, 0xEA3C6108, "sceNetAdhocMatchingCancelTarget" },
	{ 0x22DF, 0xB58E61B7, "sceNetAdhocMatchingSetHelloOpt" },
	{ 0x22E0, 0xB5D96C2A, "sceNetAdhocMatchingGetHelloOpt" },
	{ 0x22E1, 0xC58BCD9E, "sceNetAdhocMatchingGetMembers" },
	{ 0x22E2, 0x40F8F435, "sceNetAdhocMatchingGetPoolMaxAlloc" },

	// sceSasCore doesn't exist in firmware as default 2.70
	// and highter has that but many games ask for that so we load anyway..
	{ 0x22E3, 0xA3589D81, "sceSasCore_A3589D81" },
	{ 0x22E4, 0x50A14DFC, "sceSasCore_50A14DFC" },
	{ 0x22E5, 0x68A46B95, "sceSasCore_68A46B95" },
	{ 0x22E6, 0x440CA7D8, "sceSasCore_440CA7D8" },
	{ 0x22E7, 0xAD84D37F, "sceSasCore_AD84D37F" },
	{ 0x22E8, 0x99944089, "sceSasCore_99944089" },
	{ 0x22E9, 0xB7660A23, "sceSasCore_B7660A23" },
	{ 0x22EA, 0x019B25EB, "sceSasCore_019B25EB" },
	{ 0x22EB, 0x9EC3676A, "sceSasCore_9EC3676A" },
	{ 0x22EC, 0x5F9529F6, "sceSasCore_5F9529F6" },
	{ 0x22ED, 0x74AE582A, "sceSasCore_74AE582A" },
	{ 0x22EE, 0xCBCD4F79, "sceSasCore_CBCD4F79" },
	{ 0x22EF, 0x42778A9F, "sceSasCore_42778A9F" },
	{ 0x22F1, 0xA0CF2FA4, "sceSasCore_A0CF2FA4" },
	{ 0x22F2, 0x76F01ACA, "sceSasCore_76F01ACA" },
	{ 0x22F3, 0xF983B186, "sceSasCore_F983B186" },
	{ 0x22F4, 0xD5A229C9, "sceSasCore_D5A229C9" },
	{ 0x22F5, 0x33D4AB37, "sceSasCore_33D4AB37" },
	{ 0x22F6, 0x267A6DD2, "sceSasCore_267A6DD2" },
	{ 0x22F7, 0x2C8E6AB3, "sceSasCore_2c8e6ab3" },
	{ 0x22F8, 0x787D04D5, "sceSasCore_787d04d5" },
	{ 0x22F9, 0xA232CBE6, "sceSasCore_a232cbe6" },
	{ 0x22FA, 0xD5EBBBCD, "sceSasCore_d5ebbbcd" },

	// sceSasCore the following 4 appears to be on
	// Firmware 3.00 and after but add it just in case
	{ 0x22FB, 0xBD11B7C2, "sceSasCore_bd11b7c2" },
	{ 0x22FC, 0xD1E0A01E, "sceSasCore_d1e0a01e" },
	{ 0x22FD, 0xE175EF66, "sceSasCore_e175ef66" },
	{ 0x22FE, 0xE855BF76, "sceSasCore_e855bf76" },

	{ 0x22FF, 0xFD8585E1, "sceNetSetDropRate" },
	{ 0x2300, 0xE4D21302, "sceHttpsInit" }, // 1.00+
	{ 0x2301, 0xCFB957C6, "sceNetApctlConnect" }, // 1.00+
	{ 0x2302, 0x629E2FB7, "sceNetResolverStartAtoN" }, // 1.00+
	{ 0x2303, 0x808F6063, "sceNetResolverStop" }, // 1.00+

	{ 0x2304, 0xE58818A8, "sceUsbstorBootSetCapacity" }, // 1.00+
	{ 0x2305, 0x7CEB2C09, "sceKernelRegisterKprintfHandler" }, // 1.00+
	{ 0x2306, 0x84F370BC, "Kprintf" }, // 1.00+
	{ 0x2307, 0xC80181A2, "sceNetGetDropRate"},// 1.00+
	{ 0x2308, 0xB4D1CBBF, "sceCccSetTable"}, // 1.00+
	{ 0x2309, 0xBB73FF67, "sceNetApDialogDummyInit"}, // 1.00+
	{ 0x230a, 0x3811281E, "sceNetApDialogDummyConnect"}, // 1.00+
	{ 0x230b, 0xCA9BE5BF,"sceNetApDialogDummyGetState"}, // 1.00+
	{ 0x230c, 0xF213BE65,"sceNetApDialogDummyTerm"}, // 1.00+

	// FAKE MAPPING!! for Final Fantasy checks
	{ 0x3000, 0x8F2DF740, "ModuleMgrForUser_8F2DF740" }, // fw 3.52 or less?
	{ 0x3001, 0x7591C7DB, "sceKernelSetCompiledSdkVersion" }, // fw 2.5
	{ 0x3002, 0xF77D77CB, "sceKernelSetCompilerVersion" }, // fw 2.5
	{ 0x3003, 0xECA32A99, "sceAtracIsSecondBufferNeeded" }, // fw 2.5
	{ 0x3004, 0x2A2B3DE0, "sceUtilityLoadModule" }, // fw 3.52 or less?
	{ 0x3005, 0x4928BD96, "sceUtilityMsgDialogAbort" }, // fw 3.52 or less?
	{ 0x3006, 0xE49BFE92, "sceUtilityUnloadModule" }, // fw 3.52 or less?

	// More fake mapping.
	{ 0x3007, 0x87533940, "sceUmdReplaceProhibit" }, // umd function 2.00+
	{ 0x3008, 0x8F58BEDF, "sceNetAdhocMatching_8f58bedf" }, // 2.50+
	{ 0x3009, 0x099EF33C, "sceFontFindOptimumFont" }, // 2.00+
	{ 0x300A, 0x0DA7535E, "sceFontGetFontInfo" }, // 2.00+
	{ 0x300B, 0x3AEA8CB6, "sceFontClose" }, // 2.00+
	{ 0x300C, 0x574B6FBC, "sceFontDoneLib" }, // 2.00+
	{ 0x300D, 0x67F17ED7, "sceFontNewLib" }, // 2.00+
	{ 0x300E, 0xA834319D, "sceFontOpen" }, // 2.00+
	{ 0x300F, 0xCA1E6945, "sceFontGetCharGlyphImage_Clip" }, // 2.00+
	{ 0x3010, 0xDCC80C2F, "sceFontGetCharInfo" }, // 2.00+
	{ 0x3011, 0x1579A159, "sceUtilityLoadNetModule" }, // 2.00+
	{ 0x3012, 0x64D50C56, "sceUtilityUnloadNetModule" }, // 2.00+
	{ 0x3013, 0xC629AF26, "sceUtilityLoadAvModule" }, // 2.70+
	{ 0x3014, 0x980F4895, "sceFontGetCharGlyphImage" }, // 2.00, 2.01 and 3.00+
	{ 0x3015, 0xF8170FBE, "sceKernelDeleteMutex" }, // 2.70+
	{ 0x3016, 0x6B30100F, "sceKernelUnlockMutex" }, // 2.70+
	{ 0x3017, 0xB011B11F, "sceKernelLockMutex" }, // 2.70+
	

	// Found in Wipeout Pure.
	{ 0x3018, 0xB55249D2, "sceKernelIsCpuIntrEnable" }, // 1.00+
	{ 0x3019, 0xF3370E61, "sceNetResolverInit" }, // 1.00+
	{ 0x301A, 0x6138194A, "sceNetResolverTerm" }, // 1.00+
	{ 0x301B, 0xE2F91F9B, "sceNetApctlInit" }, // 1.00+
	{ 0x301C, 0xB3EDD0EC, "sceNetApctlTerm" }, // 1.00+
	{ 0x301D, 0x2BEFDF23, "sceNetApctlGetInfo" }, // 1.00+
	{ 0x301E, 0x8ABADD51, "sceNetApctlAddHandler" }, // 1.00+
	{ 0x301F, 0x5963991B, "sceNetApctlDelHandler" }, // 1.00+
	{ 0x3020, 0x24FE91A1, "sceNetApctlDisconnect" }, // 1.00+
	{ 0x3021, 0x5DEAC81B, "sceNetApctlGetState" }, // 1.00+
	{ 0x3022, 0xAB1ABE07, "sceHttpInit" }, // 1.00+
	{ 0x3023, 0xD1C8945E, "sceHttpEnd" }, // 1.00+
	{ 0x3024, 0x9B1F1F36, "sceHttpCreateTemplate" }, // 1.00+
	{ 0x3025, 0xFCF8C055, "sceHttpDeleteTemplate" }, // 1.00+
	{ 0x3026, 0xCDF8ECB9, "sceHttpCreateConnectionWithURL" }, // 1.00+
	{ 0x3027, 0x5152773B, "sceHttpDeleteConnection" }, // 1.00+
	{ 0x3028, 0xB509B09E, "sceHttpCreateRequestWithURL" }, // 1.00+
	{ 0x3028, 0xA5512E01, "sceHttpDeleteRequest" }, // 1.00+
	{ 0x3029, 0x78A0D3EC, "sceHttpEnableKeepAlive" }, // 1.00+
	{ 0x302A, 0x1A0EBB69, "sceHttpDisableRedirect" }, // 1.00+
	{ 0x302B, 0xAE948FEE, "sceHttpDisableAuth" }, // 1.00+
	{ 0x302C, 0x0B12ABFB, "sceHttpDisableCookie" }, // 1.00+
	{ 0x302D, 0xA6800C34, "sceHttpInitCache" }, // 1.00+
	{ 0x302E, 0x78B54C09, "sceHttpEndCache" }, // 1.00+
	{ 0x302F, 0xCCBD167A, "sceHttpDisableCache" }, // 1.00+
	{ 0x3030, 0xBB70706F, "sceHttpSendRequest" }, // 1.00+
	{ 0x3031, 0xC10B6BD9, "sceHttpAbortRequest" }, // 1.00+
	{ 0x3032, 0x4CC7D78F, "sceHttpGetStatusCode" }, // 1.00+
	{ 0x3033, 0xDB266CCF, "sceHttpGetAllHeader" }, // 1.00+
	{ 0x3034, 0xEDEEB999, "sceHttpReadData" }, // 1.00+
	{ 0x3035, 0x2A6C3296, "sceHttpSetAuthInfoCB" }, // 1.00+
	{ 0x3036, 0x15540184, "sceHttpDeleteHeader" }, // 1.00+
	{ 0x3037, 0x3EABA285, "sceHttpAddExtraHeader" }, // 1.00+
	{ 0x3038, 0xD081EC8F, "sceHttpGetNetworkErrno" }, // 1.00+
	{ 0x3039, 0x68AB0F86, "sceHttpsInitWithPath" }, // 1.00+
	{ 0x303A, 0x87797BDD, "sceHttpsLoadDefaultCert" }, // 1.00+
	{ 0x303B, 0xF9D8EB63, "sceHttpsEnd" }, // 1.00+
	{ 0x303C, 0x8077A433, "sceParseHttpStatusLine" }, // 1.00+
	{ 0x303D, 0x568518C9, "sceUriParse" }, // 1.00+
	{ 0x303E, 0x957ECBE2, "sceSslInit" }, // 1.00+
	{ 0x303F, 0x191CDEFF, "sceSslEnd" }, // 1.00+

	// World Championship Poker 2.
	{ 0x3040, 0x244172AF, "sceNetResolverCreate" }, // 1.00+
	{ 0x3041, 0x94523E09, "sceNetResolverDelete" }, // 1.00+
	{ 0x3042, 0x224C5F44, "sceNetResolverStartNtoA" }, // 1.00+

	// Vampire Chronicles.
	{ 0x3043, 0x27F6E642, "sceFontGetNumFontList" }, // 1.00+
	{ 0x3044, 0xBC75D85B, "sceFontGetFontList" }, // 1.00+
	{ 0x3045, 0xACE23476, "sceKernelCheckPspConfigFunction" },
	{ 0x3046, 0x7BE1421C, "sceKernelCheckExecFileFunction" },
	{ 0x3047, 0xBF983EF2, "sceKernelProbeExecutableObjectFunction" },
	{ 0x3048, 0x7068E6BA, "sceKernelLoadExecutableObjectFunction" },
	{ 0x3049, 0xB4D6FECC, "sceKernelApplyElfRelSectionFunction" },
	{ 0x3050, 0x54AB2675, "sceKernelApplyPspRelSectionFunction" },
	{ 0x3051, 0x2952F5AC, "sceKernelDcacheWBinvAllFunction" },
	{ 0x3052, 0xD8779AC6, "sceKernelIcacheClearAllFunction" },
	{ 0x3053, 0x99A695F0, "sceKernelRegisterLibraryFunction" },
	{ 0x3054, 0x5873A31F, "sceKernelRegisterLibraryForUserFunction" },
	{ 0x3055, 0x0B464512, "sceKernelReleaseLibraryFunction" },
	{ 0x3056, 0x9BAF90F6, "sceKernelCanReleaseLibraryFunction" },
	{ 0x3057, 0x0E760DBA, "sceKernelLinkLibraryEntriesFunction" },
	{ 0x3058, 0x0DE1F600, "sceKernelLinkLibraryEntriesForUserFunction" },
	{ 0x3059, 0xDA1B09AA, "sceKernelUnLinkLibraryEntriesFunction" },
	{ 0x3060, 0xC99DD47A, "sceKernelQueryLoadCoreCBFunction" },
	{ 0x3061, 0x616FCCCD, "sceKernelSetBootCallbackLevelFunction" },
	{ 0x3062, 0x52A86C21, "sceKernelGetModuleFromUIDFunction" },
	{ 0x3063, 0xCD0F3BAC, "sceKernelCreateModuleFunction" },
	{ 0x3064, 0x6B2371C2, "sceKernelDeleteModuleFunction" },
	{ 0x3065, 0x8D8A8ACE, "sceKernelAssignModuleFunction" },
	{ 0x3066, 0xAFF947D4, "sceKernelCreateAssignModuleFunction" },
	{ 0x3067, 0xAE7C6E76, "sceKernelRegisterModuleFunction" },
	{ 0x3068, 0x74CF001A, "sceKernelReleaseModuleFunction" },
	{ 0x3069, 0xCF8A41B1, "sceKernelFindModuleByNameFunction" },
	{ 0x306A, 0xFB8AE27D, "sceKernelFindModuleByAddressFunction" },
	{ 0x306B, 0xCCE4A157, "sceKernelFindModuleByUIDFunction" },
	{ 0x306C, 0x929B5C69, "sceKernelGetModuleListWithAllocFunction" },
	{ 0x306D, 0x05D915DB, "sceKernelGetModuleIdListForKernelFunction" },

	// 3.90+ or lower
	{ 0x306E, 0x315AD3A0, "sceKernelSetCompiledSdkVersion380_390" },

	// TODO remove when we support exports, MPEG.PRX
	{ 0x306F, 0x2D31F5B1, "sceVideocodecGetEDRAM" }, // 1.50+
	{ 0x3070, 0x4F160BF4, "sceVideocodecReleaseEDRAM" }, // 1.50+
	{ 0x3071, 0xC01EC829, "sceVideocodecOpen" }, // 1.50+
	{ 0x3072, 0x17099F0A, "sceVideocodecInit" }, // 1.50+
	{ 0x3073, 0xDBA273FA, "sceVideocodecDecode" }, // 1.50+
	{ 0x3074, 0xA2F0564E, "sceVideocodecStop" }, // 1.50+
	{ 0x3075, 0x307E6E1C, "sceVideocodecDelete" }, // 1.50+
	{ 0x3076, 0x745A7B7A, "sceVideocodecSetMemory" }, // 1.50+
	{ 0x3077, 0x2F385E7F, "sceVideocodecScanHeader" }, // 1.50+
	{ 0x3078, 0x26927D19, "sceVideocodecGetVersion" }, // 1.50+

	// TODO remove when we support exports, MPEG.PRX
	{ 0x3079, 0x9D3F790C, "sceAudiocodeCheckNeedMem" }, // 1.50+
	{ 0x307A, 0x5B37EB1D, "sceAudiocodecInit" }, // 1.50+
	{ 0x307B, 0x70A703F8, "sceAudiocodecDecode" }, // 1.50+
	{ 0x307C, 0x8ACA11D5, "sceAudiocodecGetInfo" }, // 1.50+
	{ 0x307D, 0x6CD2A861, "sceAudiocodec_6CD2A861" }, // 1.00 - 2.50
	{ 0x307E, 0x59176A0F, "sceAudiocodec_59176A0F" }, // 1.50+
	{ 0x307F, 0x3A20A200, "sceAudiocodecGetEDRAM" }, // 1.50+
	{ 0x3080, 0x29681260, "sceAudiocodecReleaseEDRAM" }, // 1.50+

	// Serial.
	{ 0x3081, 0x588845DA, "sceHprmEnd" }, // 1.50+
	{ 0x3082, 0x44439604, "sceSysconCtrlHRPower" }, // 1.50+
	{ 0x3083, 0x7FD7A631, "sceSysregUartIoEnable" }, // 1.50+
	{ 0x3084, 0xE146606D, "sceKernelRegisterDebugPutchar" }, // 1.50+
	{ 0x3085, 0xD636B827, "sceKernelDipswAll" }, // 1.50+

	{ 0x3086, 0x01562BA3, "sceAudioOutput2Reserve" }, // 2.50+ or lower
	{ 0x3087, 0x43196845, "sceAudioOutput2Release" }, // 2.50+ or lower
	{ 0x3088, 0x2D53F36E, "sceAudioOutput2OutputBlocking" }, // 2.50+ or lower
	{ 0x3089, 0x63F2889C, "sceAudioOutput2ChangeLength" }, // 2.50+ or lower
	{ 0x308A, 0x647CEF33, "sceAudioOutput2GetRestSample" }, // 2.50+ or lower

	{ 0x308B, 0xEBD177D6, "scePower_EBD177D6" }, // 3.52+ or lower
	{ 0x308C, 0x342061E5, "sceKernelSetCompiledSdkVersion370" }, // 3.72+ or lower
	{ 0x308D, 0x07F58C24, "sceSasCore_07F58C24" }, // 3.72+ or lower

	{ 0x308E, 0xEBD5C3E6, "sceKernelSetCompiledSdkVersion395" }, // 3.95+ or lower
	{ 0x308F, 0x132F1ECA, "sceAtracReinit" }, // 2.50+ or lower
	{ 0x3090, 0x2DD3E298, "sceAtrac3plus_2DD3E298" }, // 2.50+ or lower
	{ 0x3091, 0xB3B5D042, "sceAtracGetOutputChannel" }, // 2.50+ or lower

	{ 0x3092, 0xCDC3AA41, "sceUtilityHtmlViewerInitStart" }, // 2.00+
	{ 0x3093, 0x05AFB9E4, "sceUtilityHtmlViewerUpdate" }, // 2.00+
	{ 0x3094, 0xBDA7D894, "sceUtilityHtmlViewerGetStatus" }, // 2.00+
	{ 0x3095, 0xF5CE1134, "sceUtilityHtmlViewerShutdownStart" }, // 2.00+
	{ 0x3096, 0x76D1363B, "sceHttpSaveSystemCookie" }, // 2.00+
	{ 0x3097, 0xF1657B22, "sceHttpLoadSystemCookie" }, // 2.00+

	{ 0x3095, 0xBB8E7FE6, "sceFontOpenUserMemory" }, // 2.00+
	{ 0x3096, 0xF7D8D092, "sceUtilityUnloadAvModule" }, // 2.71+
	{ 0x3097, 0xEE232411, "sceFontSetAltCharacterCode" }, // 2.00+
	{ 0x3098, 0x5C3E4A9E, "sceFontGetCharImageRect" }, // 2.00+ or lower
	{ 0x3099, 0x8CA3A97E, "sceNetInetGetPspError" }, // 2.00+
	{ 0x309a, 0x8DB83FDC, "sceNetAdhocctlGetPeerInfo" }, // 2.00+
	{ 0x309b, 0x0D5BC6D2, "sceUtilityLoadUsbModule" }, // 2.71+ or lower
	{ 0x309c, 0xF64910F0, "sceUtilityUnloadUsbModule" }, // 2.71+ or lower
	{ 0x309d, 0x472694CD, "sceFontPointToPixelH" }, // 2.00+ or lower
	{ 0x309e, 0x5333322D, "sceFontGetFontInfoByIndexNumber" }, // 2.00+ or lower
	{ 0x309f, 0xEC19337D, "sceNetAdhocMatchingAbortSendData" }, // 3.52+ or lower
	{ 0x30a0, 0xF79472D7, "sceNetAdhocMatchingSendData" }, // 3.52+ or lower
	{ 0x30a1, 0x0DDCD2C9, "sceKernelTryLockMutex" }, // 2.71+ or lower
	{ 0x30a2, 0x5BF4DD27, "sceKernelLockMutexCB" }, // 2.71+ or lower
	{ 0x30a3, 0x87D9223C, "sceKernelCancelMutex" }, // 2.71+ or lower
	{ 0x30a4, 0xA9C2CB9A, "sceKernelReferMutexStatus" }, // 2.71+ or lower
	{ 0x30a5, 0x5CF9D852, "sceAtracSetMOutHalfwayBuffer"}, // 2.50+ or lower
	{ 0x30a6, 0x48293280, "sceFontSetResolution"}, // 2.00+

    { 0x30A7, 0xB7D098C6, "sceKernelCreateMutex" }, // 2.70+  //in jpcsp is different!
	
	{ 0x30a8, 0x07EC321A,"sceMp3ReserveMp3Handle" }, // 3.95+
	{ 0x30a9, 0x0DB149F4,"sceMp3NotifyAddStreamData" }, // 3.95+
	{ 0x30aa, 0x2A368661,"sceMp3ResetPlayPosition" },// 3.95+
	{ 0x30ab, 0x35750070,"sceMp3InitResource" }, // 3.95+
	{ 0x30ac, 0x3C2FA058,"sceMp3TermResource" }, // 3.95+
	{ 0x30ad, 0x3CEF484F,"sceMp3SetLoopNum" }, // 3.95+
	{ 0x30ae, 0x44E07129,"sceMp3Init" }, // 3.95+
	{ 0x30af, 0x7F696782,"sceMp3GetMp3ChannelNum" }, // 3.95+
	{ 0x30b0, 0x8F450998,"sceMp3GetSamplingRate" }, // 3.95+
	{ 0x30b1, 0xA703FE0F,"sceMp3GetInfoToAddStreamData" }, // 3.95+
	{ 0x30b2, 0xD021C0FB,"sceMp3Decode" }, // 3.95+
	{ 0x30b3, 0xD0A56296,"sceMp3CheckStreamDataNeeded" }, // 3.95+
	{ 0x30b4, 0xF5478233,"sceMp3ReleaseMp3Handle" }, // 3.95+

	// CFW, we choose to start at 0x5000.
	{ 0x5000, 0x5C25EA72, "kubridge_4C25EA72" },
	{ 0x5001, 0x577AF198, "systemctrl_user_577AF198" },
	{ 0x5002, 0x75643FCA, "systemctrl_user_75643FCA" },
	{ 0x5003, 0xABA7F1B0, " systemctrl_user_ABA7F1B0" },
	{ 0x5004, 0x16C3B7EE, "systemctrl_user_16C3B7EE" },
	{ 0x5005, 0x85B520C6, "systemctrl_user_85B520C6" },

		// Module magic, we choose to start at 0x4000.
	{ 0x6000, 0xD3744BE0, "module_bootstart" },
	{ 0x6001, 0x2F064FA6, "module_reboot_before" },
	{ 0x6002, 0xADF12745, "module_reboot_phase" },
	{ 0x6003, 0xD632ACDB, "module_start" },
	{ 0x6004, 0xCEE8593C, "module_stop_1" },
	{ 0x6005, 0xF01D73A7, "module_stop_2" },
	{ 0x6006, 0x0F7C276C, "module_0F7C276C" },

};
