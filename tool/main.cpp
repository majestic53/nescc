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

#include <map>
#include <vector>
#include "../include/runtime.h"

#define ARGUMENT_COLUMN_WIDTH 12
#define ARGUMENT_DELIMITER '-'

enum {
	ARGUMENT_DEBUG,
	ARGUMENT_HELP,
	ARGUMENT_VERSION,
};

#define ARGUMENT_MAX ARGUMENT_VERSION
#define ARGUMENT_MIN 2

static const std::string ARGUMENT_STR[] = {
	"d", "h", "v",
	};

#define ARGUMENT_STRING(_TYPE_) \
	(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
		STRING_CHECK(ARGUMENT_STR[_TYPE_]))

static const std::string ARGUMENT_STR_LONG[] = {
	"debug", "help", "version",
	};

#define ARGUMENT_STRING_LONG(_TYPE_) \
	(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
		STRING_CHECK(ARGUMENT_STR_LONG[_TYPE_]))

static const std::string ARGUMENT_STR_DESC[] = {
	"Display debug information",
	"Display help information",
	"Display version information",
	};

#define ARGUMENT_STRING_DESCRIPTION(_TYPE_) \
	(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
		STRING_CHECK(ARGUMENT_STR_DESC[_TYPE_]))

static const std::map<std::string, uint32_t> ARGUMENT_MAP = {
	std::make_pair(ARGUMENT_STRING(ARGUMENT_DEBUG), ARGUMENT_DEBUG),
	std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_DEBUG), ARGUMENT_DEBUG),
	std::make_pair(ARGUMENT_STRING(ARGUMENT_HELP), ARGUMENT_HELP),
	std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_HELP), ARGUMENT_HELP),
	std::make_pair(ARGUMENT_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION),
	std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERSION), ARGUMENT_VERSION),
	};

std::string
usage_string(void)
{
	int iter = 0;
	std::stringstream result;

	result << NESCC;

	for(; iter <= ARGUMENT_MAX; ++iter) {
		result << " [" << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter) << "|"
			<< ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << ARGUMENT_STRING_LONG(iter) << "]";
	}

	result << " path";

	return result.str();
}

std::string
version_string(
	__in_opt bool verbose = false
	)
{
	std::stringstream result;

	if(verbose) {
		result << NESCC << " ";
	}

	result << nescc::runtime::version(true);

	if(verbose) {
		result << std::endl << NESCC_COPYRIGHT;
	}

	return result.str();
}

std::string
help_string(void)
{
	int iter = 0;
	std::stringstream result;

	result << version_string(true) << std::endl << std::endl << usage_string();

	for(; iter <= ARGUMENT_MAX; ++iter) {

		if(!iter) {
			result << std::endl;
		}

		result << std::endl << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter)
			<< " | " << ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << std::left << std::setw(ARGUMENT_COLUMN_WIDTH)
				<< ARGUMENT_STRING_LONG(iter)
			<< ARGUMENT_STRING_DESCRIPTION(iter);
	}

	return result.str();
}

int
run(
	__in const std::string &path,
	__in_opt bool debug = false
	)
{
	int result = EXIT_SUCCESS;

	try {
		nescc::runtime &instance = nescc::runtime::acquire();
		instance.initialize();
		instance.run(path, debug);
		instance.uninitialize();
		instance.release();
	} catch(nescc::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = EXIT_FAILURE;
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}

int
run_input(
	__in const std::vector<std::string> &arguments
	)
{
	std::string path;
	int result = EXIT_SUCCESS;
	bool debug = false, help = false, version = false;

	try {
		size_t index = 0;
		std::vector<std::string>::const_iterator iter;

		if(arguments.size() < ARGUMENT_MIN) {
			THROW_EXCEPTION_FORMAT("Missing arguments", "%s", STRING_CHECK(usage_string()));
		}

		for(iter = arguments.begin() + 1; iter != arguments.end(); ++index, ++iter) {
			std::string str = *iter;

			if(str.front() == ARGUMENT_DELIMITER) {
				std::map<std::string, uint32_t>::const_iterator entry;

				str = str.substr(1, str.size());
				if(str.front() == ARGUMENT_DELIMITER) {
					str = str.substr(1, str.size());
				}

				entry = ARGUMENT_MAP.find(str);
				if(entry == ARGUMENT_MAP.end()) {
					THROW_EXCEPTION_FORMAT("Malformed argument", "Arg[%u]=%s", index, (*iter).c_str());
				}

				switch(entry->second) {
					case ARGUMENT_DEBUG:
						debug = true;
						break;
					case ARGUMENT_HELP:
						help = true;
						break;
					case ARGUMENT_VERSION:
						version = true;
						break;
					default:
						THROW_EXCEPTION_FORMAT("Unsupported argument", "Arg[%u]=%s", index,
							(*iter).c_str());
				}
			} else {

				if(!path.empty()) {
					THROW_EXCEPTION_FORMAT("Path reassigned!", "%s", STRING_CHECK(usage_string()));
				}

				path = str;
			}
		}

		if(help) {
			std::cout << help_string() << std::endl;
		} else if(version) {
			std::cout << version_string() << std::endl;
		} else if(path.empty()) {
			THROW_EXCEPTION_FORMAT("Unassigned path!", "%s", STRING_CHECK(usage_string()));
		} else {
			result = run(path, debug);
		}
	} catch(nescc::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = EXIT_FAILURE;
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}

int
main(
	__in int argc,
	__in const char *argv[]
	)
{
	return run_input(std::vector<std::string>(argv, argv + argc));
}
