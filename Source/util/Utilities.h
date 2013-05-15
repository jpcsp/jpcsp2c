

#pragma once

#include "Common.h"

namespace Utilities
{
	string readString(u32 address, u32 size);
	void writeString(u32 address, const char *str);
	__int64 currentTimeMillis();
	 __int64 currentTimeMicroSeconds();
}