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

//#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

// C headers
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <direct.h>

// C++ headers
#include <map>
#include <list>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// pcsp headers
#include "Utilities/ShortTypes.h"
#include "Utilities/Error.h"
#include "Utilities/wxConfigEmu.h"
#include "Debugging/Log.h"
#include "Allegrex/Allegrex.h"
#include "util/Utilities.h"




//modules
#include "HLE/psputils.h"
//#include "HLE/ThreadMan.h"
#include "HLE/pspSysMem.h"