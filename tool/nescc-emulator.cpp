/**
 * Nescc
 * Copyright (C) 2017-2018 David Jolly
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

#include "./include/emulator.h"

int
main(
	__in int argc,
	__in const char *argv[]
	)
{
	int result = EXIT_SUCCESS;

	try {
		nescc::tool::emulator &instance = nescc::tool::emulator::acquire();
		instance.initialize();
		result = instance.invoke(std::vector<std::string>(argv, argv + argc));
		instance.uninitialize();
		instance.release();
	} catch(nescc::exception &exc) {
		std::cerr << "Error: " << exc.to_string(true) << std::endl;
		result = EXIT_FAILURE;
	} catch(std::exception &exc) {
		std::cerr << "Error: " << exc.what() << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}
