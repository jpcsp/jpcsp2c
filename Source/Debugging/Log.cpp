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

#ifdef _WINDOWS
#include <io.h>
#include <fcntl.h>
HANDLE hStdOut;
#endif

typedef map<string, FILE *> logmap;
typedef map<FILE *, u32>    refmap;

static logmap logMap;
static refmap refMap;
static bool useCon = false;

static FILE *getLogFile(const string &logName) {
	string fileName;
	logmap::iterator it;

	/*if (logName == "SomeLog") {
		fileName = "somelog.log";
		it = logMap.find(fileName);
	}
	else */{
		fileName = "pcsp.log";
		it = logMap.find(fileName);
	}
	
	if (it == logMap.end()) {
		FILE *file = fopen(fileName.c_str(), "w");
		logMap[fileName] = file;
		refMap[file] = 1;
		return file;
	}
	else {
		FILE *file = it->second;
		assert(refMap[file] != 0);
		++refMap[file];
		return file;
	}
}

Log::Log()
	: file(NULL)
{
}

Log::Log(const string &name)
	: name(name)
{
	file = getLogFile(name);

	u32 lvl;
	if(!wxConfigEmu::get("Logging", name, &lvl, LogLevelOff))
	{
	   lvl=0;
       wxConfigEmu::set("Logging", name, lvl);
	}

	level = (LogLevel)lvl;
}

Log::Log(const Log &other)
	: level(other.level), file(other.file), name(other.name)
{
	if (file != NULL)
		++refMap[file];
}

Log::~Log()
{
	if (file == NULL)
		return;

	refmap::iterator it = refMap.find(file);
	if (it == refMap.end())
		return;

	--it->second;
	if (it->second == 0) {
		fclose(file);
		refMap.erase(it);

		setLevel(level);
	}
}

void Log::operator =(const Log &other) {
	level = other.level;
	name = other.name;
	file = other.file;
	if (file != NULL)
		++refMap[file];
}

LogLevel Log::getLevel() const {
	return level;
}

void Log::setLevel(LogLevel level) {
	u32 lvl = level;
//	wxConfigEmu::set("Logging", name, lvl);
	this->level = level;
}

bool Log::checkLevel(LogLevel level) const {
#ifdef NO_LOGGING
	return false;
#else
	return this->level <= level;
#endif
}

void Log::fatal(const char *fmt, ...) {
	if (checkLevel(LogLevelFatal) && file == NULL && !useCon)
		return;

	va_list args;
	va_start(args, fmt);
	if (file != NULL) {
		fprintf(file, "[Fatal][%s] ", name.c_str());
		vfprintf(file, fmt, args);
		fprintf(file, "\n");
	}
	if (useCon) {
#ifdef _WINDOWS
			SetConsoleTextAttribute(hStdOut, 0x0074);
#endif
		printf("[Fatal][%s] ", name.c_str());
		vprintf(fmt, args);
		printf("\n");
	}
	va_end(args);
}

void Log::error(const char *fmt, ...) {
	if (checkLevel(LogLevelError) && file == NULL && !useCon)
		return;

	va_list args;
	va_start(args, fmt);
	if (file != NULL) {
		fprintf(file, "[Error][%s] ", name.c_str());
		vfprintf(file, fmt, args);
		fprintf(file, "\n");
	}
	if (useCon) {
#ifdef _WINDOWS
		SetConsoleTextAttribute(hStdOut, 0x000C);
#endif
		printf("[Error][%s] ", name.c_str());
		vprintf(fmt, args);
		printf("\n");
	}
	va_end(args);
}

void Log::warn(const char *fmt, ...) {
	if (checkLevel(LogLevelWarn) && file == NULL && !useCon)
		return;

	va_list args;
	va_start(args, fmt);
	if (file != NULL) {
		fprintf(file, "[Warn][%s] ", name.c_str());
		vfprintf(file, fmt, args);
		fprintf(file, "\n");
	}
	if (useCon) {
#ifdef _WINDOWS
		SetConsoleTextAttribute(hStdOut, 0x000D);
#endif
		printf("[Warn][%s] ", name.c_str());
		vprintf(fmt, args);
		printf("\n");
	}
	va_end(args);
}

void Log::info(const char *fmt, ...) {
	if (checkLevel(LogLevelInfo) && file == NULL && !useCon)
		return;

	va_list args;
	va_start(args, fmt);
	if (file != NULL) {
		fprintf(file, "[Info][%s] ", name.c_str());
		vfprintf(file, fmt, args);
		fprintf(file, "\n");
	}
	if (useCon) {
#ifdef _WINDOWS
		SetConsoleTextAttribute(hStdOut, 0x0007);
#endif
		printf("[Info][%s] ", name.c_str());
		vprintf(fmt, args);
		printf("\n");
	}
	va_end(args);
}

void Log::debug(const char *fmt, ...) {
	if (checkLevel(LogLevelDebug) && file == NULL && !useCon)
		return;

	va_list args;
	va_start(args, fmt);
	if (file != NULL) {
		fprintf(file, "[Debug][%s] ", name.c_str());
		vfprintf(file, fmt, args);
		fprintf(file, "\n");
	}
	if (useCon) {
#ifdef _WINDOWS
		SetConsoleTextAttribute(hStdOut, 0x000A);
#endif
		printf("[Debug][%s] ", name.c_str());
		vprintf(fmt, args);
		printf("\n");
	}
	va_end(args);
}

void Log::trace(const char *fmt, ...) {
	if (checkLevel(LogLevelTrace) && file == NULL && !useCon)
		return;

	va_list args;
	va_start(args, fmt);
	if (file != NULL) {
		fprintf(file, "[Trace][%s] ", name.c_str());
		vfprintf(file, fmt, args);
		fprintf(file, "\n");
	}
	if (useCon) {
#ifdef _WINDOWS
		SetConsoleTextAttribute(hStdOut, 0x000F);
#endif
		printf("[Trace][%s ", name.c_str());
		vprintf(fmt, args);
		printf("\n");
	}
	va_end(args);
}
#ifdef _WINDOWS



bool ShowConsole(bool show) {
	if (show == useCon) return true;
	if (show) {
		if (!AllocConsole()) {
			//Log.Fatal("Failed to allocate console!");
			return false;
		}
		SetConsoleTitle("PCSP Output");

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) {
			FreeConsole();
			//Log.Fatal("Failed to allocate console!");
			return false;
		}

		int crt = _open_osfhandle((long)hStdOut, _O_TEXT);
		*stdout = *_fdopen(crt, "w");
		setvbuf(stdout, NULL, _IONBF, 0);
		useCon = true;
	}
	else {
		FreeConsole();
		useCon = false;
	}
	return true;
}
#endif

