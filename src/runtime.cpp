/**
 * Nescc
 * Copyright (C) 2017 David Jolly
 *
 * Nescc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nescc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/runtime.h"
#include "./runtime_type.h"

namespace nescc {

	runtime::runtime(void)
	{
		// TODO
	}

	runtime::~runtime(void)
	{
		// TODO
	}

	bool
	runtime::on_create(void)
	{
		// TODO

		return true;
	}

	void
	runtime::on_destroy(void)
	{
		// TODO
	}

	bool
	runtime::on_initialize(
		__in const std::vector<uint8_t> &input
		)
	{
		// TODO

		return true;
	}

	void
	runtime::on_uninitialize(void)
	{
		// TODO
	}

	std::string
	runtime::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		result << NESCC_RUNTIME_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Base=" << nescc::interface::singleton<nescc::runtime>::to_string(verbose);
		}

		return result.str();
	}

	std::string
	runtime::version(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << NESCC_VERSION_MAJOR << "." << NESCC_VERSION_MINOR << "." << NESCC_VERSION_WEEK
			<< "." << NESCC_VERSION_REVISION;

		if(verbose) {
			result << "-" << NESCC_VERSION_RELEASE;
		}

		return result.str();
	}
}
