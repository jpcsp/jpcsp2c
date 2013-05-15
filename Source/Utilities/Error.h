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

namespace Error {
	// set formatted error msg
	void set(const char *fmt, ...);

	// get last error msg
	const char *get();

	// show error box with formatted error msg
	// if fmt is NULL, last set error msg is used
	void show(const char *fmt = NULL, ...);

	// clear error status
	void clear();

	// returns true if there's no error
	bool isFine();
};
