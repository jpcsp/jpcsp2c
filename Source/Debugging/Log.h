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

// #define NO_LOGGING
bool ShowConsole(bool show);
enum LogLevel {
	LogLevelOff,
	LogLevelFatal,
	LogLevelError,
	LogLevelWarn,
	LogLevelInfo,
	LogLevelDebug,
	LogLevelTrace
};

class Log {
public:
	Log();
	Log(const string &name);
	Log(const Log &other);
	~Log();

	void operator =(const Log &other);

	static void useConsole(bool use);

	LogLevel getLevel() const;
	void     setLevel(LogLevel level);
	bool   checkLevel(LogLevel level) const;

	void fatal(const char *fmt, ...);
	void error(const char *fmt, ...);
	void warn (const char *fmt, ...);
	void info (const char *fmt, ...);
	void debug(const char *fmt, ...);
	void trace(const char *fmt, ...);

private:
	LogLevel level;
	FILE    *file;
	string   name;
};
