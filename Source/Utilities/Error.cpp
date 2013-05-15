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

#include "Common.h"

static const size_t errorMsgSize = 1024;
static bool errorSet = false;
static char errorMsg[errorMsgSize] = "";

void Error::set(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsprintf(errorMsg, fmt, args);
	va_end(args);
	errorSet = true;
}

const char *Error::get() {
	return errorMsg;
}

void Error::show(const char *fmt, ...) {
	if (fmt != NULL) {
		va_list args;
		va_start(args, fmt);
		vsprintf(errorMsg, fmt, args);
		va_end(args);
		errorSet = true;
	}
	if (!errorSet)
		strcpy(errorMsg, "Unknown error!");

	UINT uType = MB_OK | MB_ICONERROR;
	//uType |= MB_DEFAULT_DESKTOP_ONLY;
	MessageBox(
		NULL, errorMsg, "PCSP: Error", uType);
}

void Error::clear() {
	errorSet = false;
}

bool Error::isFine() {
	return !errorSet;
}
