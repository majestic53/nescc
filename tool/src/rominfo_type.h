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

#ifndef NESCC_TOOL_ROMINFO_TYPE_H_
#define NESCC_TOOL_ROMINFO_TYPE_H_

#include <climits>
#include <map>
#include "../../include/exception.h"

namespace nescc {

	namespace tool {

		#define NESCC_TOOL_ROMINFO_HEADER "[NESCC::TOOL::ROMINFO]"
#ifndef NDEBUG
		#define NESCC_TOOL_ROMINFO_EXCEPTION_HEADER NESCC_TOOL_ROMINFO_HEADER " "
#else
		#define NESCC_TOOL_ROMINFO_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_TOOL_ROMINFO_EXCEPTION_ARGUMENT_MALFORMED = 0,
			NESCC_TOOL_ROMINFO_EXCEPTION_ARGUMENT_MISSING,
			NESCC_TOOL_ROMINFO_EXCEPTION_ARGUMENT_UNSUPPORTED,
			NESCC_TOOL_ROMINFO_EXCEPTION_FILE_MALFORMED,
			NESCC_TOOL_ROMINFO_EXCEPTION_FILE_NOT_CREATED,
			NESCC_TOOL_ROMINFO_EXCEPTION_FILE_NOT_FOUND,
			NESCC_TOOL_ROMINFO_EXCEPTION_PATH_REASSIGNED,
			NESCC_TOOL_ROMINFO_EXCEPTION_PATH_UNASSIGNED,
			NESCC_TOOL_ROMINFO_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_TOOL_ROMINFO_EXCEPTION_MAX NESCC_TOOL_ROMINFO_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_TOOL_ROMINFO_EXCEPTION_STR[] = {
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Malformed argument",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Missing argument",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Unsupported argument",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Malformed file",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Failed to create file",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "File does not exist",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Path reassigned",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Path unassigned",
			NESCC_TOOL_ROMINFO_EXCEPTION_HEADER "Rominfo is uninitialized",
			};

		#define NESCC_TOOL_ROMINFO_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_TOOL_ROMINFO_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_TOOL_ROMINFO_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_TOOL_ROMINFO_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_TOOL_ROMINFO_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_TOOL_ROMINFO_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_TOOL_ROMINFO_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define NESCC_ROMINFO "Nescc-rominfo"

		#define ARGUMENT_COLUMN_WIDTH 24
		#define ARGUMENT_DELIMITER '-'

		#define DIRECTORY_DELIMITER "/"

		#define EXTENSION_DELIMITER "."

		#define EXTRACT_DIRECTORY_DEFAULT "./"
		#define EXTRACT_DIRECTORY_DELIMITER "/\\"
		#define EXTRACT_EXTENSION_DELIMITER "."
		#define EXTRACT_FILE_DEFAULT "rom"
		#define EXTRACT_ROM_CHR_EXTENSION "chr"
		#define EXTRACT_ROM_CHR_DECODE_EXTENSION "ppm"
		#define EXTRACT_ROM_PRG_EXTENSION "prg"

		#define ROM_CHR_DECODE_DEPTH 255
		#define ROM_CHR_DECODE_DIMENSION 128
		#define ROM_CHR_DECODE_TYPE "P3"

		enum {
			ARGUMENT_DECODE_CHR = 0,
			ARGUMENT_EXTRACT_CHR,
			ARGUMENT_EXTRACT_PRG,
			ARGUMENT_HELP,
			ARGUMENT_VERBOSE,
			ARGUMENT_VERSION,
		};

		#define ARGUMENT_MAX ARGUMENT_VERSION

		static const std::string ARGUMENT_STR[] = {
			"d", "c", "p", "h", "a", "v",
			};

		#define ARGUMENT_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR[_TYPE_]))

		static const std::string ARGUMENT_STR_LONG[] = {
			"decode-chr", "extract-chr", "extract-pgr", "help", "verbose", "version",
			};

		#define ARGUMENT_STRING_LONG(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR_LONG[_TYPE_]))

		static const std::string ARGUMENT_STRING_DESC[] = {
			"Decode character rom banks",
			"Extract character rom banks",
			"Extract program rom banks",
			"Display help information",
			"Display verbose information",
			"Display version information",
			};

		#define ARGUMENT_STRING_DESCRIPTION(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STRING_DESC[_TYPE_]))

		static const std::map<std::string, uint32_t> ARGUMENT_MAP = {
			std::make_pair(ARGUMENT_STRING(ARGUMENT_DECODE_CHR), ARGUMENT_DECODE_CHR),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_DECODE_CHR), ARGUMENT_DECODE_CHR),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_EXTRACT_CHR), ARGUMENT_EXTRACT_CHR),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_EXTRACT_CHR), ARGUMENT_EXTRACT_CHR),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_EXTRACT_PRG), ARGUMENT_EXTRACT_PRG),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_EXTRACT_PRG), ARGUMENT_EXTRACT_PRG),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_VERBOSE), ARGUMENT_VERBOSE),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERBOSE), ARGUMENT_VERBOSE),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERSION), ARGUMENT_VERSION),
			};

		enum {
			TILE_PALETTE_0 = 0,
			TILE_PALETTE_1,
			TILE_PALETTE_2,
			TILE_PALETTE_3,
		};

		#define TILE_PALETTE_MAX TILE_PALETTE_3

		union tile_palette_t {
			struct {
				uint32_t blue : 8;
				uint32_t green : 8;
				uint32_t red : 8;
				uint32_t unused : 8;
			};
			uint32_t raw;
		};

		static const tile_palette_t TILE_PALETTE_COL[] = {
			{ .red = 0x00, .green = 0x00, .blue = 0x00 }, // 0x000000
			{ .red = 0x9c, .green = 0x17, .blue = 0x46 }, // 0x9C1746
			{ .red = 0xe2, .green = 0x7a, .blue = 0x53 }, // 0xE27A53
			{ .red = 0x8a, .green = 0x5e, .blue = 0x1d }, // 0x8A5E1D
			};

		#define TILE_PALETTE_COLOR(_TYPE_) \
			(((_TYPE_) > TILE_PALETTE_MAX) ? TILE_PALETTE_COL[0] : \
				TILE_PALETTE_COL[_TYPE_])

		struct tile_plain_t {
			uint8_t first[CHAR_BIT];
			uint8_t second[CHAR_BIT];
		};
	}
}

#endif // NESCC_TOOL_ROMINFO_TYPE_H_
