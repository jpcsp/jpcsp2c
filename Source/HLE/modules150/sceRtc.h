

class sceRtc : public HLEModule
{
public:
	static void sceRtcGetTickResolution();
	static void sceRtcGetCurrentTick();
	static void sceRtcGetAccumulativeTime();
	static void sceRtc_029CA3B3();
	static void sceRtcGetCurrentClock();
	static void sceRtcGetCurrentClockLocalTime();
	static void sceRtcConvertUtcToLocalTime();
	static void sceRtcConvertLocalTimeToUTC();
	static void sceRtcIsLeapYear();
	static void sceRtcGetDaysInMonth();
	static void sceRtcGetDayOfWeek();
	static void sceRtcCheckValid();
	static void sceRtcSetTime_t();
	static void sceRtcGetTime_t();
	static void sceRtcSetDosTime();
	static void sceRtcGetDosTime();
	static void sceRtcSetWin32FileTime();
	static void sceRtcGetWin32FileTime();
	static void sceRtcSetTick();
	static void sceRtcGetTick();
	static void sceRtcCompareTick();
	static void sceRtcTickAddTicks();
	static void sceRtcTickAddMicroseconds();
	static void sceRtcTickAddSeconds();
	static void sceRtcTickAddMinutes();
	static void sceRtcTickAddHours();
	static void sceRtcTickAddDays();
	static void sceRtcTickAddWeeks();
	static void sceRtcTickAddMonths();
	static void sceRtcTickAddYears();
	static void sceRtcFormatRFC2822();
	static void sceRtcFormatRFC2822LocalTime();
	static void sceRtcFormatRFC3339();
	static void sceRtcFormatRFC3339LocalTime();
	static void sceRtcParseDateTime();
	static void sceRtcParseRFC3339();
	class sceRtcGetTickResolutionFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetTickResolutionFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetTickResolution();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetTickResolutionFunc;";
		}
	};
	class sceRtcGetCurrentTickFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetCurrentTickFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetCurrentTick();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetCurrentTickFunc;";
		}
	};
	class sceRtcGetAccumulativeTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetAccumulativeTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetAccumulativeTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetAccumulativeTimeFunc;";
		}
	};
	class sceRtc_029CA3B3Func : public HLEModuleFunction
	{
	public:
		sceRtc_029CA3B3Func(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtc_029CA3B3();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtc_029CA3B3Func;";
		}
	};
	class sceRtcGetCurrentClockFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetCurrentClockFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetCurrentClock();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetCurrentClockFunc;";
		}
	};
	class sceRtcGetCurrentClockLocalTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetCurrentClockLocalTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetCurrentClockLocalTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetCurrentClockLocalTimeFunc;";
		}
	};
	class sceRtcConvertUtcToLocalTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcConvertUtcToLocalTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcConvertUtcToLocalTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcConvertUtcToLocalTimeFunc;";
		}
	};
	class sceRtcConvertLocalTimeToUTCFunc : public HLEModuleFunction
	{
	public:
		sceRtcConvertLocalTimeToUTCFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcConvertLocalTimeToUTC();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcConvertLocalTimeToUTCFunc;";
		}
	};
	class sceRtcIsLeapYearFunc : public HLEModuleFunction
	{
	public:
		sceRtcIsLeapYearFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcIsLeapYear();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcIsLeapYearFunc;";
		}
	};

	class sceRtcGetDaysInMonthFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetDaysInMonthFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetDaysInMonth();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetDaysInMonthFunc;";
		}
	};
	class sceRtcGetDayOfWeekFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetDayOfWeekFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	      
	   }
   		virtual void execute()
		{
			 sceRtcGetDayOfWeek();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetDayOfWeekFunc;";
		}
	};
	class sceRtcCheckValidFunc : public HLEModuleFunction
	{
	public:
		sceRtcCheckValidFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcCheckValid();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcCheckValidFunc;";
		}
	};
	class sceRtcSetTime_tFunc : public HLEModuleFunction
	{
	public:
		sceRtcSetTime_tFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcSetTime_t();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcSetTime_tFunc;";
		}
	};
	class sceRtcGetTime_tFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetTime_tFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetTime_t();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetTime_tFunc;";
		}
	};
	class sceRtcSetDosTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcSetDosTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcSetDosTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcSetDosTimeFunc;";
		}
	};
	class sceRtcGetDosTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetDosTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetDosTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetDosTimeFunc;";
		}
	};
	class sceRtcSetWin32FileTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcSetWin32FileTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcSetWin32FileTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcSetWin32FileTimeFunc;";
		}
	};
	class sceRtcGetWin32FileTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetWin32FileTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetWin32FileTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetWin32FileTimeFunc;";
		}
	};
	class sceRtcSetTickFunc : public HLEModuleFunction
	{
	public:
		sceRtcSetTickFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcSetTick();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcSetTickFunc;";
		}
	};
	class sceRtcGetTickFunc : public HLEModuleFunction
	{
	public:
		sceRtcGetTickFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcGetTick();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcGetTickFunc;";
		}
	};

	class sceRtcCompareTickFunc : public HLEModuleFunction
	{
	public:
		sceRtcCompareTickFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcCompareTick();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcCompareTickFunc;";
		}
	};
	class sceRtcTickAddTicksFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddTicksFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddTicks();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddTicksFunc;";
		}
	};
	class sceRtcTickAddMicrosecondsFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddMicrosecondsFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddMicroseconds();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddMicrosecondsFunc;";
		}
	};
	class sceRtcTickAddSecondsFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddSecondsFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddSeconds();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddSecondsFunc;";
		}
	};
	class sceRtcTickAddMinutesFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddMinutesFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddMinutes();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddMinutesFunc;";
		}
	};
	class sceRtcTickAddHoursFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddHoursFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddHours();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddHoursFunc;";
		}
	};
	class sceRtcTickAddDaysFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddDaysFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddDays();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddDaysFunc;";
		}
	};
	class sceRtcTickAddWeeksFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddWeeksFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddWeeks();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddWeeksFunc;";
		}
	};
	class sceRtcTickAddMonthsFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddMonthsFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddMonths();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddMonthsFunc;";
		}
	};
	class sceRtcTickAddYearsFunc : public HLEModuleFunction
	{
	public:
		sceRtcTickAddYearsFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcTickAddYears();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcTickAddYearsFunc;";
		}
	};
	class sceRtcFormatRFC2822Func : public HLEModuleFunction
	{
	public:
		sceRtcFormatRFC2822Func(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcFormatRFC2822();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcFormatRFC2822Func;";
		}
	};
	class sceRtcFormatRFC2822LocalTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcFormatRFC2822LocalTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcFormatRFC2822LocalTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcFormatRFC2822LocalTimeFunc;";
		}
	};
	class sceRtcFormatRFC3339Func : public HLEModuleFunction
	{
	public:
		sceRtcFormatRFC3339Func(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcFormatRFC3339();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcFormatRFC3339Func;";
		}
	};
	class sceRtcFormatRFC3339LocalTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcFormatRFC3339LocalTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcFormatRFC3339LocalTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcFormatRFC3339LocalTimeFunc;";
		}
	};
	class sceRtcParseDateTimeFunc : public HLEModuleFunction
	{
	public:
		sceRtcParseDateTimeFunc(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcParseDateTime();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcParseDateTimeFunc;";
		}
	};
	class sceRtcParseRFC3339Func : public HLEModuleFunction
	{
	public:
		sceRtcParseRFC3339Func(string moduleName, string functionName): HLEModuleFunction(moduleName,functionName)
	   {
	       
	   }
   		virtual void execute()
		{
			sceRtcParseRFC3339();
		}
		virtual string compiledString() {
			return "jpcsp.HLE.Modules.sceRtc.sceRtcParseRFC3339Func;";
		}
	};
    virtual string getName();
    virtual void installModule(int version);
    virtual void uninstallModule(int version);

	


};
					
	
			
			
			
			
			
			
			
			
			