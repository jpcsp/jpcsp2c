

#include "Common.h"
#include "Memory.h"
#include "Utilities.h"

string Utilities::readString(u32 address, u32 size)
{
	char *start = (char *)Memory::getPointer(address);
	char *p = start;
	while(*p && (u32)(p - start) < size)
		++p;
	return string(start, p - start);
}

void Utilities::writeString(u32 address, const char *str)
{
	char *start = (char *)Memory::getPointer(address);
	strcpy(start, str);
}

__int64 Utilities::currentTimeMillis()
{
static const __int64 magic = 116444736000000000; // 1970/1/1
  SYSTEMTIME st;
  GetSystemTime(&st);
  FILETIME   ft;
  SystemTimeToFileTime(&st,&ft); // in 100-nanosecs...
  __int64 t;
  memcpy(&t,&ft,sizeof t);
  return (t - magic)/10000; // scale to millis.
}
 __int64 Utilities::currentTimeMicroSeconds() //or we can use that...
{
  SYSTEMTIME st;
  FILETIME ft;
  ULARGE_INTEGER ulift;
  GetSystemTime(&st);
  SystemTimeToFileTime(&st,&ft);
  ulift.LowPart = ft.dwLowDateTime;
  ulift.HighPart = ft.dwHighDateTime;
  return (ulift.QuadPart/10) + 50522659200000000i64;
 }
