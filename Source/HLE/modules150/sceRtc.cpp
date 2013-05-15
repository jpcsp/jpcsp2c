/*
*
*      8/05/2011
*      WIP!!! !
*      based on r913
*/
#include "Common.h"
#include "HLE/modules/HLEModuleFunction.h"
#include "HLE/modules/HLEThread.h"
#include "HLE/modules/HLEModuleManager.h"
#include "HLE/modules/HLEModule.h"
#include "util/Utilities.h"
#include "Memory.h"
#include "sceRtc.h"
#include "HLE/Modules.h"
#include "dateutil.h"


__int64 hleGetCurrentTick() 
{
	return Utilities::currentTimeMillis() * 1000L;
}

void sceRtc::sceRtcGetTickResolution()
{
   pspRegs.GPR[2] = 1000000;
}
void sceRtc::sceRtcGetCurrentTick()
{
	u32 addr = pspRegs.GPR[4];
    __int64 tick = hleGetCurrentTick(); 
	*((__int64*)Memory::getPointer(addr)) = tick;
	pspRegs.GPR[2]=0;
}
void sceRtc::sceRtcGetAccumulativeTime()
{
	    Modules::log.warn("Unimplemented NID function sceRtcGetAccumulativeTime [0x011F03C1]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtc_029CA3B3()
{
		Modules::log.warn("Unimplemented NID function sceRtc_029CA3B3 [0x029CA3B3]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcGetCurrentClock()
{	
		Modules::log.warn("Unimplemented NID function sceRtcGetCurrentClock [0x4CFA57B0]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcGetCurrentClockLocalTime()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcGetCurrentClockLocalTime");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcConvertUtcToLocalTime()//TODO TEST it
{
		Modules::log.debug("UNTESTED PORTED FUNCTION sceRtcConvertUtcToLocalTime");
		u32 utc_addr = pspRegs.GPR[4];
        u32 local_addr = pspRegs.GPR[5];

        Modules::log.debug("PARTIAL:sceRtcConvertUtcToLocalTime");
        unsigned __int64 utc= *((unsigned __int64*)Memory::getPointer(utc_addr));
        unsigned __int64 local = utc; // TODO
		*((unsigned __int64*)Memory::getPointer(local_addr)) = local;
		pspRegs.GPR[2] = 0;

}
void sceRtc::sceRtcConvertLocalTimeToUTC()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcConvertLocalTimeToUTC");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcIsLeapYear()
{
	    Modules::log.debug("sceRtcIsLeapYear");
		u32 year = pspRegs.GPR[4];
		if((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
			pspRegs.GPR[2] = 1;
		else
			pspRegs.GPR[2] = 0;
}
void sceRtc::sceRtcGetDaysInMonth()
{
   u32 year = pspRegs.GPR[4];
   u32 month = pspRegs.GPR[5];
    Modules::log.debug("sceRtcGetDaysInMonth year : %u month : %u",year,month);
   pspRegs.GPR[2] = daysformonth(month,year);
}
void sceRtc::sceRtcGetDayOfWeek()
{
   u32 year = pspRegs.GPR[4];
   u32 month = pspRegs.GPR[5];
   u32 day = pspRegs.GPR[6];
   Modules::log.debug("sceRtcGetDayofWeek year : %u month : %u day : %u",year,month,day);
   pspRegs.GPR[2] =((getDayNumber(year,month,day)+1)) %7;
}

void sceRtc::sceRtcCheckValid()
{
		Modules::log.warn("Unimplemented NID function sceRtcCheckValid [0x4B1B5E82]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcSetTime_t()
{
		Modules::log.warn("Unimplemented NID function sceRtcSetTime_t [0x3A807CC8]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcGetTime_t()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcGetTime_t");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcSetDosTime()
{
		Modules::log.warn("Unimplemented NID function sceRtcSetDosTime [0xF006F264]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcGetDosTime()
{
		Modules::log.warn("Unimplemented NID function sceRtcGetDosTime [0x36075567]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcSetWin32FileTime()
{
		Modules::log.warn("Unimplemented NID function sceRtcSetWin32FileTime [0x7ACE4C04]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcGetWin32FileTime()
{

		Modules::log.warn("Unimplemented NID function sceRtcGetWin32FileTime [0xCF561893]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}
void sceRtc::sceRtcSetTick()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP)sceRtcSetTick");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcGetTick()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcGetTick");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcCompareTick()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcCompareTick");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddTicks()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP)sceRtcTickAddTicks");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddMicroseconds()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddMicroseconds");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddSeconds()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddSeconds");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddMinutes()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP)sceRtcTickAddMinutes");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddHours()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddHours");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddDays()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddDays");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddWeeks()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddWeeks");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddMonths()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddMonths");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcTickAddYears()
{
		Modules::log.error("UNIMPLEMENTED (EXISTS IN JPCSP) sceRtcTickAddYears");
		pspRegs.GPR[2] = 0xDEADC0DE;
}//TODO
void sceRtc::sceRtcFormatRFC2822() 
{

		Modules::log.warn("Unimplemented NID function sceRtcFormatRFC2822 [0xC663B3B9]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}

void sceRtc::sceRtcFormatRFC2822LocalTime()
{
		Modules::log.warn("Unimplemented NID function sceRtcFormatRFC2822LocalTime [0x7DE6711B]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}

void sceRtc::sceRtcFormatRFC3339() 
{
		Modules::log.warn("Unimplemented NID function sceRtcFormatRFC3339 [0x0498FB3C]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}

void sceRtc::sceRtcFormatRFC3339LocalTime() 
{
		Modules::log.warn("Unimplemented NID function sceRtcFormatRFC3339LocalTime [0x27F98543]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}

void sceRtc::sceRtcParseDateTime() 
{
		Modules::log.warn("Unimplemented NID function sceRtcParseDateTime [0xDFBC5F16]");
		pspRegs.GPR[2] = 0xDEADC0DE;
}

void sceRtc::sceRtcParseRFC3339() 
{
	Modules::log.warn("Unimplemented NID function sceRtcParseRFC3339 [0x28E1E988]");
	pspRegs.GPR[2] = 0xDEADC0DE;
}

#if 0

void sceRtcGetCurrentClockLocalTime() {
		
		

        int addr = pspRegs.GPR[4];
        ScePspDateTime pspTime = new ScePspDateTime();
        pspTime.write(mem, addr);

		pspRegs.GPR[2] = 0;
	}

void sceRtcConvertUtcToLocalTime() {
		
		
		


	}

void sceRtcConvertLocalTimeToUTC() {
		
		
		

        int local_addr = pspRegs.GPR[4];
        int utc_addr = pspRegs.GPR[5];

        Modules.log.debug("PARTIAL:sceRtcConvertLocalTimeToUTC");

        long local = mem.read64(local_addr);
        long utc = local; // TODO
        mem.write64(utc_addr, utc);

		pspRegs.GPR[2] = 0;
	}


void sceRtcGetTime_t() {
        
        
        

        

        int date_addr = pspRegs.GPR[4];
        int time_addr = pspRegs.GPR[5];

        if (mem.isAddressGood(date_addr) && mem.isAddressGood(time_addr)) {
            ScePspDateTime dateTime = new ScePspDateTime();
            dateTime.read(mem, date_addr);
            Calendar cal = Calendar.getInstance();
            cal.set(dateTime.year, dateTime.month - 1, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
            int unixtime = (int)(cal.getTime().getTime() / 1000L);
            Modules.log.debug("sceRtcGetTime_t psptime:" + dateTime + " unixtime:" + unixtime);
            mem.write32(time_addr, unixtime);
            pspRegs.GPR[2] = 0;
        } else {
            Modules::log.warn("sceRtcGetTime_t bad address " + String.format("0x%08X 0x%08X", date_addr, time_addr));
            pspRegs.GPR[2] = -1;
        }
    }

    /** Set a pspTime struct based on ticks. */
void sceRtcSetTick() {
        
        
        

        int time_addr = pspRegs.GPR[4];
        int ticks_addr = pspRegs.GPR[5];

        Modules.log.debug("sceRtcSetTick");

        if (mem.isAddressGood(time_addr) && mem.isAddressGood(ticks_addr)) {
            long ticks = mem.read64(ticks_addr);
            ScePspDateTime time = ScePspDateTime.fromMicros(ticks);
            time.write(mem, time_addr);
            pspRegs.GPR[2] = 0;
        } else {
            Modules::log.warn("sceRtcSetTick bad address "
                + String.format("0x%08X 0x%08X", time_addr, ticks_addr));
            pspRegs.GPR[2] = -1;
        }
    }

    /** Set ticks based on a pspTime struct. */
void sceRtcGetTick() {
        
        
        

        int time_addr = pspRegs.GPR[4];
        int ticks_addr = pspRegs.GPR[5];

        Modules::log.warn("UNIMPLEMENTED:sceRtcGetTick");

        if (mem.isAddressGood(time_addr) && mem.isAddressGood(ticks_addr)) {
            /* TODO use java library to convert a date to seconds, then just multiply it by the tick resolution
            ScePspDateTime time = new ScePspDateTime();
            time.read(mem, time_addr);
            long ticks = ...
            mem.write64(ticks_addr, ticks);
            pspRegs.GPR[2] = 0;
            */
            pspRegs.GPR[2] = -1;
        } else {
            Modules::log.warn("sceRtcGetTick bad address "
                + String.format("0x%08X 0x%08X", time_addr, ticks_addr));
            pspRegs.GPR[2] = -1;
        }
    }

void sceRtcCompareTick() {
        
        
        

        int first = pspRegs.GPR[4];
        int second = pspRegs.GPR[5];

        Modules.log.debug("sceRtcCompareTick");

        if (mem.isAddressGood(first) && mem.isAddressGood(second)) {
            long tick1 = mem.read64(first);
            long tick2 = mem.read64(second);

            if (tick1 == tick2)
                pspRegs.GPR[2] = 0;
            else if (tick1 < tick2)
                pspRegs.GPR[2] = -1;
            else if (tick1 > tick2)
                pspRegs.GPR[2] = 1;
        } else {
            Modules::log.warn("sceRtcCompareTick bad address "
                + String.format("0x%08X 0x%08X", first, second));
            pspRegs.GPR[2] = -1;
        }
    }

    /** 64 bit addend */
    protected void hleRtcTickAdd64(, long multiplier) {
        
        
        

        int dest_addr = pspRegs.GPR[4];
        int src_addr = pspRegs.GPR[5];
        long value = ((((long)pspRegs.GPR[6]) & 0xFFFFFFFFL) | (((long)pspRegs.GPR[7])<<32));

        Modules.log.debug("hleRtcTickAdd64 " + multiplier + " * " + value);

        if (mem.isAddressGood(src_addr) && mem.isAddressGood(dest_addr)) {
            long src = mem.read64(src_addr);
            mem.write64(dest_addr, src + multiplier * value);
            pspRegs.GPR[2] = 0;
        } else {
            Modules::log.warn("hleRtcTickAdd64 bad address "
                + String.format("0x%08X 0x%08X", src_addr, dest_addr));
            pspRegs.GPR[2] = -1;
        }
    }

    /** 32 bit addend */
    protected void hleRtcTickAdd32(, long multiplier) {
        
        
        

        int dest_addr = pspRegs.GPR[4];
        int src_addr = pspRegs.GPR[5];
        int value = pspRegs.GPR[6];

        Modules.log.debug("hleRtcTickAdd32 " + multiplier + " * " + value);

        if (mem.isAddressGood(src_addr) && mem.isAddressGood(dest_addr)) {
            long src = mem.read64(src_addr);
            mem.write64(dest_addr, src + multiplier * value);
            pspRegs.GPR[2] = 0;
        } else {
            Modules::log.warn("hleRtcTickAdd32 bad address "
                + String.format("0x%08X 0x%08X", src_addr, dest_addr));
            pspRegs.GPR[2] = -1;
        }
    }

void sceRtcTickAddTicks() {
        Modules.log.debug("sceRtcTickAddTicks redirecting to hleRtcTickAdd64(1)");
        hleRtcTickAdd64(processor, 1);
    }

void sceRtcTickAddMicroseconds() {
        Modules.log.debug("sceRtcTickAddMicroseconds redirecting to hleRtcTickAdd64(1)");
        hleRtcTickAdd64(processor, 1);
    }

void sceRtcTickAddSeconds() {
        Modules.log.debug("sceRtcTickAddSeconds redirecting to hleRtcTickAdd64(1000000)");
        hleRtcTickAdd64(processor, 1000000L);
    }

void sceRtcTickAddMinutes() {
        Modules.log.debug("sceRtcTickAddMinutes redirecting to hleRtcTickAdd64(60*1000000)");
        hleRtcTickAdd64(processor, 60*1000000L);
    }

void sceRtcTickAddHours() {
        Modules.log.debug("sceRtcTickAddHours redirecting to hleRtcTickAdd32(60*60*1000000)");
        hleRtcTickAdd32(processor, 60*60*1000000L);
    }

void sceRtcTickAddDays() {
        Modules.log.debug("sceRtcTickAddDays redirecting to hleRtcTickAdd32(24*60*60*1000000)");
        hleRtcTickAdd32(processor, 24*60*60*1000000L);
    }

void sceRtcTickAddWeeks() {
        Modules.log.debug("sceRtcTickAddWeeks redirecting to hleRtcTickAdd32(7*24*60*60*1000000)");
        hleRtcTickAdd32(processor, 7*24*60*60*1000000L);
    }

    /** TODO check on real psp.
     * maybe take account of different number of days in each month,
     * decompose the source ticks into parts add months, re-assemble.
     * setting 1 month as 30 days */
void sceRtcTickAddMonths() {
        Modules::log.warn("PARTIAL:sceRtcTickAddMonths redirecting to hleRtcTickAdd32(30*24*60*60*1000000)");
        hleRtcTickAdd32(processor, 30*24*60*60*1000000L);
    }

    /** TODO check on real psp.
     * maybe take account of different number of days in each year.
     * setting 1 year as 365 days */
void sceRtcTickAddYears() {
        Modules.log.debug("PARTIAL:sceRtcTickAddYears redirecting to hleRtcTickAdd32(365*24*60*60*1000000)");
        hleRtcTickAdd32(processor, 365*24*60*60*1000000L);
    }


#endif

sceRtc::sceRtcGetTickResolutionFunc			*sceRtcGetTickResolutionFunction= new sceRtc::sceRtcGetTickResolutionFunc("sceRtc", "sceRtcGetTickResolution");
sceRtc::sceRtcGetCurrentTickFunc			*sceRtcGetCurrentTickFunction= new sceRtc::sceRtcGetCurrentTickFunc("sceRtc", "sceRtcGetCurrentTick");
sceRtc::sceRtcGetAccumulativeTimeFunc		*sceRtcGetAccumulativeTimeFunction = new sceRtc::sceRtcGetAccumulativeTimeFunc("sceRtc", "sceRtcGetAccumulativeTime");
sceRtc::sceRtc_029CA3B3Func			        *sceRtc_029CA3B3Function= new sceRtc::sceRtc_029CA3B3Func	("sceRtc", "sceRtc_029CA3B3");
sceRtc::sceRtcGetCurrentClockFunc			*sceRtcGetCurrentClockFunction= new sceRtc::sceRtcGetCurrentClockFunc("sceRtc", "sceRtcGetCurrentClock");
sceRtc::sceRtcGetCurrentClockLocalTimeFunc	*sceRtcGetCurrentClockLocalTimeFunction=new sceRtc::sceRtcGetCurrentClockLocalTimeFunc("sceRtc", "sceRtcGetCurrentClockLocalTime");
sceRtc::sceRtcConvertUtcToLocalTimeFunc		*sceRtcConvertUtcToLocalTimeFunction= new sceRtc::sceRtcConvertUtcToLocalTimeFunc("sceRtc", "sceRtcConvertUtcToLocalTime");
sceRtc::sceRtcConvertLocalTimeToUTCFunc		*sceRtcConvertLocalTimeToUTCFunction= new sceRtc::sceRtcConvertLocalTimeToUTCFunc("sceRtc", "sceRtcConvertLocalTimeToUTC");
sceRtc::sceRtcIsLeapYearFunc			    *sceRtcIsLeapYearFunction = new sceRtc::sceRtcIsLeapYearFunc("sceRtc", "sceRtcIsLeapYear");
sceRtc::sceRtcGetDaysInMonthFunc			*sceRtcGetDaysInMonthFunction = new sceRtc::sceRtcGetDaysInMonthFunc("sceRtc", "sceRtcGetDaysInMonth");
sceRtc::sceRtcGetDayOfWeekFunc			    *sceRtcGetDayOfWeekFunction = new sceRtc::sceRtcGetDayOfWeekFunc("sceRtc", "sceRtcGetDayOfWeek");
sceRtc::sceRtcCheckValidFunc		        *sceRtcCheckValidFunction = new sceRtc::sceRtcCheckValidFunc("sceRtc", "sceRtcCheckValid");
sceRtc::sceRtcSetTime_tFunc		            *sceRtcSetTime_tFunction = new sceRtc::sceRtcSetTime_tFunc("sceRtc", "sceRtcSetTime_t");
sceRtc::sceRtcGetTime_tFunc			        *sceRtcGetTime_tFunction = new sceRtc::sceRtcGetTime_tFunc("sceRtc", "sceRtcGetTime_t");
sceRtc::sceRtcSetDosTimeFunc		        *sceRtcSetDosTimeFunction = new sceRtc::sceRtcSetDosTimeFunc("sceRtc", "sceRtcSetDosTime");
sceRtc::sceRtcGetDosTimeFunc			    *sceRtcGetDosTimeFunction = new sceRtc::sceRtcGetDosTimeFunc("sceRtc", "sceRtcGetDosTime");
sceRtc::sceRtcSetWin32FileTimeFunc			*sceRtcSetWin32FileTimeFunction = new sceRtc::sceRtcSetWin32FileTimeFunc("sceRtc", "sceRtcSetWin32FileTime");
sceRtc::sceRtcGetWin32FileTimeFunc			*sceRtcGetWin32FileTimeFunction = new sceRtc::sceRtcGetWin32FileTimeFunc	("sceRtc", "sceRtcGetWin32FileTime");
sceRtc::sceRtcSetTickFunc			        *sceRtcSetTickFunction = new sceRtc::sceRtcSetTickFunc("sceRtc", "sceRtcSetTick");
sceRtc::sceRtcGetTickFunc			        *sceRtcGetTickFunction = new sceRtc::sceRtcGetTickFunc("sceRtc", "sceRtcGetTick");
sceRtc::sceRtcCompareTickFunc			    *sceRtcCompareTickFunction = new sceRtc::sceRtcCompareTickFunc("sceRtc", "sceRtcCompareTick");
sceRtc::sceRtcTickAddTicksFunc			    *sceRtcTickAddTicksFunction = new sceRtc::sceRtcTickAddTicksFunc("sceRtc", "sceRtcTickAddTicks");
sceRtc::sceRtcTickAddMicrosecondsFunc		*sceRtcTickAddMicrosecondsFunction = new sceRtc::sceRtcTickAddMicrosecondsFunc("sceRtc", "sceRtcTickAddMicroseconds");
sceRtc::sceRtcTickAddSecondsFunc			*sceRtcTickAddSecondsFunction = new sceRtc::sceRtcTickAddSecondsFunc("sceRtc", "sceRtcTickAddSeconds");
sceRtc::sceRtcTickAddMinutesFunc			*sceRtcTickAddMinutesFunction = new sceRtc::sceRtcTickAddMinutesFunc("sceRtc", "sceRtcTickAddMinutes");
sceRtc::sceRtcTickAddHoursFunc			    *sceRtcTickAddHoursFunction = new sceRtc::sceRtcTickAddHoursFunc("sceRtc", "sceRtcTickAddHours");
sceRtc::sceRtcTickAddDaysFunc			    *sceRtcTickAddDaysFunction = new sceRtc::sceRtcTickAddDaysFunc("sceRtc", "sceRtcTickAddDays");
sceRtc::sceRtcTickAddWeeksFunc			    *sceRtcTickAddWeeksFunction= new sceRtc::sceRtcTickAddWeeksFunc("sceRtc", "sceRtcTickAddWeeks");
sceRtc::sceRtcTickAddMonthsFunc			    *sceRtcTickAddMonthsFunction = new sceRtc::sceRtcTickAddMonthsFunc("sceRtc", "sceRtcTickAddMonths");
sceRtc::sceRtcTickAddYearsFunc			    *sceRtcTickAddYearsFunction = new sceRtc::sceRtcTickAddYearsFunc("sceRtc", "sceRtcTickAddYears");
sceRtc::sceRtcFormatRFC2822Func			    *sceRtcFormatRFC2822Function = new sceRtc::sceRtcFormatRFC2822Func("sceRtc", "sceRtcFormatRFC2822");
sceRtc::sceRtcFormatRFC2822LocalTimeFunc	*sceRtcFormatRFC2822LocalTimeFunction = new sceRtc::sceRtcFormatRFC2822LocalTimeFunc("sceRtc", "sceRtcFormatRFC2822LocalTime");
sceRtc::sceRtcFormatRFC3339Func			    *sceRtcFormatRFC3339Function = new sceRtc::sceRtcFormatRFC3339Func("sceRtc", "sceRtcFormatRFC3339");
sceRtc::sceRtcFormatRFC3339LocalTimeFunc	*sceRtcFormatRFC3339LocalTimeFunction= new sceRtc::sceRtcFormatRFC3339LocalTimeFunc("sceRtc", "sceRtcFormatRFC3339LocalTime");
sceRtc::sceRtcParseDateTimeFunc			    *sceRtcParseDateTimeFunction = new sceRtc::sceRtcParseDateTimeFunc("sceRtc", "sceRtcParseDateTime");
sceRtc::sceRtcParseRFC3339Func			    *sceRtcParseRFC3339Function= new sceRtc::sceRtcParseRFC3339Func("sceRtc", "sceRtcParseRFC3339") ;

string sceRtc::getName()
{
   return "sceRtc";
}

void sceRtc::installModule(int version)
{
	if (version >= 150) {

			HLEModuleManager::addFunction(sceRtcGetTickResolutionFunction, 0xC41C2853);
			HLEModuleManager::addFunction(sceRtcGetCurrentTickFunction, 0x3F7AD767);
			HLEModuleManager::addFunction(sceRtcGetAccumulativeTimeFunction, 0x011F03C1);
			HLEModuleManager::addFunction(sceRtc_029CA3B3Function, 0x029CA3B3);
			HLEModuleManager::addFunction(sceRtcGetCurrentClockFunction, 0x4CFA57B0);
			HLEModuleManager::addFunction(sceRtcGetCurrentClockLocalTimeFunction, 0xE7C27D1B);
			HLEModuleManager::addFunction(sceRtcConvertUtcToLocalTimeFunction, 0x34885E0D);
			HLEModuleManager::addFunction(sceRtcConvertLocalTimeToUTCFunction, 0x779242A2);
			HLEModuleManager::addFunction(sceRtcIsLeapYearFunction, 0x42307A17);
			HLEModuleManager::addFunction(sceRtcGetDaysInMonthFunction, 0x05EF322C);
			HLEModuleManager::addFunction(sceRtcGetDayOfWeekFunction, 0x57726BC1);
			HLEModuleManager::addFunction(sceRtcCheckValidFunction, 0x4B1B5E82);
			HLEModuleManager::addFunction(sceRtcSetTime_tFunction, 0x3A807CC8);
			HLEModuleManager::addFunction(sceRtcGetTime_tFunction, 0x27C4594C);
			HLEModuleManager::addFunction(sceRtcSetDosTimeFunction, 0xF006F264);
			HLEModuleManager::addFunction(sceRtcGetDosTimeFunction, 0x36075567);
			HLEModuleManager::addFunction(sceRtcSetWin32FileTimeFunction, 0x7ACE4C04);
			HLEModuleManager::addFunction(sceRtcGetWin32FileTimeFunction, 0xCF561893);
			HLEModuleManager::addFunction(sceRtcSetTickFunction, 0x7ED29E40);
			HLEModuleManager::addFunction(sceRtcGetTickFunction, 0x6FF40ACC);
			HLEModuleManager::addFunction(sceRtcCompareTickFunction, 0x9ED0AE87);
			HLEModuleManager::addFunction(sceRtcTickAddTicksFunction, 0x44F45E05);
			HLEModuleManager::addFunction(sceRtcTickAddMicrosecondsFunction, 0x26D25A5D);
			HLEModuleManager::addFunction(sceRtcTickAddSecondsFunction, 0xF2A4AFE5);
			HLEModuleManager::addFunction(sceRtcTickAddMinutesFunction, 0xE6605BCA);
			HLEModuleManager::addFunction(sceRtcTickAddHoursFunction, 0x26D7A24A);
			HLEModuleManager::addFunction(sceRtcTickAddDaysFunction, 0xE51B4B7A);
			HLEModuleManager::addFunction(sceRtcTickAddWeeksFunction, 0xCF3A2CA8);
			HLEModuleManager::addFunction(sceRtcTickAddMonthsFunction, 0xDBF74F1B);
			HLEModuleManager::addFunction(sceRtcTickAddYearsFunction, 0x42842C77);
			HLEModuleManager::addFunction(sceRtcFormatRFC2822Function, 0xC663B3B9);
			HLEModuleManager::addFunction(sceRtcFormatRFC2822LocalTimeFunction, 0x7DE6711B);
			HLEModuleManager::addFunction(sceRtcFormatRFC3339Function, 0x0498FB3C);
			HLEModuleManager::addFunction(sceRtcFormatRFC3339LocalTimeFunction, 0x27F98543);
			HLEModuleManager::addFunction(sceRtcParseDateTimeFunction, 0xDFBC5F16);
			HLEModuleManager::addFunction(sceRtcParseRFC3339Function, 0x28E1E988);

		}
}
void sceRtc::uninstallModule(int version)
{
	if (version >= 150) {

			HLEModuleManager::removeFunction(sceRtcGetTickResolutionFunction);
			HLEModuleManager::removeFunction(sceRtcGetCurrentTickFunction);
			HLEModuleManager::removeFunction(sceRtcGetAccumulativeTimeFunction);
			HLEModuleManager::removeFunction(sceRtc_029CA3B3Function);
			HLEModuleManager::removeFunction(sceRtcGetCurrentClockFunction);
			HLEModuleManager::removeFunction(sceRtcGetCurrentClockLocalTimeFunction);
			HLEModuleManager::removeFunction(sceRtcConvertUtcToLocalTimeFunction);
			HLEModuleManager::removeFunction(sceRtcConvertLocalTimeToUTCFunction);
			HLEModuleManager::removeFunction(sceRtcIsLeapYearFunction);
			HLEModuleManager::removeFunction(sceRtcGetDaysInMonthFunction);
			HLEModuleManager::removeFunction(sceRtcGetDayOfWeekFunction);
			HLEModuleManager::removeFunction(sceRtcCheckValidFunction);
			HLEModuleManager::removeFunction(sceRtcSetTime_tFunction);
			HLEModuleManager::removeFunction(sceRtcGetTime_tFunction);
			HLEModuleManager::removeFunction(sceRtcSetDosTimeFunction);
			HLEModuleManager::removeFunction(sceRtcGetDosTimeFunction);
			HLEModuleManager::removeFunction(sceRtcSetWin32FileTimeFunction);
			HLEModuleManager::removeFunction(sceRtcGetWin32FileTimeFunction);
			HLEModuleManager::removeFunction(sceRtcSetTickFunction);
			HLEModuleManager::removeFunction(sceRtcGetTickFunction);
			HLEModuleManager::removeFunction(sceRtcCompareTickFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddTicksFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddMicrosecondsFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddSecondsFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddMinutesFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddHoursFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddDaysFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddWeeksFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddMonthsFunction);
			HLEModuleManager::removeFunction(sceRtcTickAddYearsFunction);
			HLEModuleManager::removeFunction(sceRtcFormatRFC2822Function);
			HLEModuleManager::removeFunction(sceRtcFormatRFC2822LocalTimeFunction);
			HLEModuleManager::removeFunction(sceRtcFormatRFC3339Function);
			HLEModuleManager::removeFunction(sceRtcFormatRFC3339LocalTimeFunction);
			HLEModuleManager::removeFunction(sceRtcParseDateTimeFunction);
			HLEModuleManager::removeFunction(sceRtcParseRFC3339Function);
		}
}
 
	