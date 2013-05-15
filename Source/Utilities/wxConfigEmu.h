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
#include "Common.h"
namespace wxConfigEmu {
	bool load();
	bool save();

	//void set(const string &sect, const string &name, bool value);
	//void set(const string &sect, const string &name, s32 value);
	void set(const string &sect, const string &name, u32 value);
	//void set(const string &sect, const string &name, const string &value);

	bool get(const string &sect, const string &name, bool *value, bool def = false);
	//bool get(const string &sect, const string &name, s32 *value, s32 def = 0);
	bool get(const string &sect, const string &name, u32 * value, u32 def =0);
	//bool get(const string &sect, const string &name, string &value, const string &def = string());
};