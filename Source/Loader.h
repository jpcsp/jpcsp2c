/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "HLE/Kernel/Types/SceModule.h"

enum Format {
	FormatUnknown = 0x00,
	FormatELF     = 0x01,
	FormatPRX     = 0x02,
	FormatPBP     = 0x04,
	FormatSCE     = 0x08,
	FormatPSP     = 0x10
};

namespace Loader {
	void reset();

	// pspfilename   Example:
	//               ms0:/PSP/GAME/xxx/EBOOT.PBP
	//               disc0:/PSP_GAME/SYSDIR/BOOT.BIN
	//               disc0:/PSP_GAME/SYSDIR/EBOOT.BIN
	//               xxx:/yyy/zzz.prx
	// baseAddress   should be at least 64-byte aligned,
	//               or how ever much is the default alignment in pspSysMem.
	// returns       Always a SceModule object, you should check the
	//               fileFormat member against the Format* bits.
	//               Example: (fileFormat & FormatELF)
	SceModule *loadModule(
		const char *pspFileName, ifstream &f, u32 baseAddress);
};
