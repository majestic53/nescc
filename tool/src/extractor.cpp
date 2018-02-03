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

#include <cstring>
#include <fstream>
#include "../../include/console/cpu.h"
#include "../include/extractor.h"
#include "./extractor_type.h"

namespace nescc {

	namespace tool {

		extractor::extractor(void) :
			m_character_rom_base(0),
			m_character_rom_count(0),
			m_character_rom_size(0),
			m_character_rom_width(0),
			m_header({}),
			m_length(0),
			m_mapper(0),
			m_mirroring(0),
			m_program_ram_count(0),
			m_program_ram_size(0),
			m_program_rom_base(0),
			m_program_rom_count(0),
			m_program_rom_width(0),
			m_program_rom_size(0),
			m_trace(nescc::trace::acquire()),
			m_tv_system(0)
		{
			m_trace.initialize();

			TRACE_ENTRY();
			TRACE_EXIT();
		}

		extractor::~extractor(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();

			m_trace.uninitialize();
			m_trace.release();
		}

		std::string
		extractor::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "Rom: "
					<< STRING_CHECK(m_path)
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Size: "
					<< FLOAT_PRECISION(1, m_length / KILOBYTE) << " KB (" << m_length << " bytes)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Version: "
					<< ((m_header.version == CARTRIDGE_VERSION_2) ? CARTRIDGE_VERSION_2 : CARTRIDGE_VERSION_1)
				<< std::endl << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- PRG RAM: "
					<< m_program_ram_count << " (" << FLOAT_PRECISION(1, m_program_ram_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- PRG ROM: "
					<< m_program_rom_count << " (" << FLOAT_PRECISION(1, m_program_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Base: "
					<< m_program_rom_base << " (" << FLOAT_PRECISION(1, m_program_rom_base / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Length: "
					<< m_program_rom_size << " (" << FLOAT_PRECISION(1, m_program_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Width: "
					<< m_program_rom_width << " (" << FLOAT_PRECISION(1, m_program_rom_width / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- CHR ROM: "
					<< m_character_rom_count << " (" << FLOAT_PRECISION(1, m_character_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Base: "
					<< m_character_rom_base << " (" << FLOAT_PRECISION(1, m_character_rom_base / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Length: "
					<< m_character_rom_size << " (" << FLOAT_PRECISION(1, m_character_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Width: "
					<< m_character_rom_width << " (" << FLOAT_PRECISION(1, m_character_rom_width / KILOBYTE) << " KB)"
				<< std::endl << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Mapper: "
					<< m_mapper << " (" << CARTRIDGE_MAPPER_STRING(m_mapper) << ")"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Mirroring: "
					<< m_mirroring << " (" << CARTRIDGE_MIRRORING_STRING(m_mirroring) << ")"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- TV system: "
					<< m_tv_system << " (" << CARTRIDGE_TV_SYSTEM_STRING(m_tv_system) << ")"
				<< std::endl << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- VRAM: "
					<< (m_header.vram ? "Present" : "NOT present")
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Trainer: "
					<< (m_header.trainer ? "Present" : "NOT present")
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Battery: "
					<< (m_header.battery ? "Present" : "NOT present")
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- PlayChoice-10: "
					<< (m_header.play_choice_10 ? "True" : "False")
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- VS. Unisystem: "
					<< (m_header.vs_unisystem ? "True" : "False");

			if(verbose) {

				if(!result.str().empty()) {
					result << std::endl << std::endl;
				}

				result << m_raw.as_string(0, m_raw.size(), 0, true);
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		extractor::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION(NESCC_TOOL_EXTRACTOR_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Extractor clearing...");

			m_character_rom_base = 0;
			m_character_rom_count = 0;
			m_character_rom_size = 0;
			m_character_rom_width = 0;
			std::memset(&m_header, 0, sizeof(m_header));
			m_mapper = 0;
			m_mirroring = 0;
			m_length = 0;
			m_path.clear();
			m_program_ram_count = 0;
			m_program_ram_size = 0;
			m_program_rom_base = 0;
			m_program_rom_count = 0;
			m_program_rom_width = 0;
			m_program_rom_size = 0;
			m_raw.clear();
			m_tv_system = 0;

			TRACE_MESSAGE(TRACE_INFORMATION, "Extractor cleared.");

			TRACE_EXIT();
		}

		void
		extractor::decode_character_bank(
			__in const std::string &path,
			__in std::vector<nescc::core::memory<uint8_t>>::iterator &bank
			)
		{
			nescc::core::bitmap image;
			std::vector<uint8_t> pixel;
			uint32_t image_x, image_y, tile_x, tile_y;

			TRACE_ENTRY_FORMAT("Path[%u]=%s, Bank=%p", path.size(), STRING_CHECK(path), &bank);

			pixel.resize(ROM_CHR_DECODE_WIDTH * ROM_CHR_DECODE_HEIGHT, 0);

			for(tile_y = 0; tile_y < (ROM_CHR_DECODE_HEIGHT / CHAR_BIT); ++tile_y) {

				for(tile_x = 0; tile_x < (ROM_CHR_DECODE_WIDTH / CHAR_BIT); ++tile_x) {
					uint32_t index;
					tile_plain_t *plain = nullptr;

					index = ((tile_y * sizeof(tile_plain_t) * (ROM_CHR_DECODE_HEIGHT / CHAR_BIT))
							+ ((tile_x % (ROM_CHR_DECODE_HEIGHT / CHAR_BIT)) * sizeof(tile_plain_t)));
					if(tile_x >= (ROM_CHR_DECODE_HEIGHT / CHAR_BIT)) {
						index += ROM_CHR_DECODE_TABLE_LENGTH;
					}

					plain = (tile_plain_t *) (bank->raw() + index);
					if(plain) {
						uint8_t pixel_y = 0;
						uint16_t sprite_x = (tile_x * CHAR_BIT), sprite_y = (tile_y * CHAR_BIT);

						for(; pixel_y < CHAR_BIT; ++pixel_y) {
							int8_t pixel_x = (CHAR_BIT - 1);

							for(; pixel_x >= 0; --pixel_x) {
								index = (((sprite_y + pixel_y) * ROM_CHR_DECODE_WIDTH)
										+ (sprite_x + ((CHAR_BIT - 1) - pixel_x)));
								pixel.at(index) = (((plain->first[pixel_y] >> pixel_x) & 1)
										| (((plain->second[pixel_y] >> pixel_x) & 1) << 1));
							}
						}
					}
				}
			}

			image.allocate(ROM_CHR_DECODE_WIDTH, ROM_CHR_DECODE_HEIGHT);

			for(image_y = 0; image_y < ROM_CHR_DECODE_HEIGHT; ++image_y) {

				for(image_x = 0; image_x < ROM_CHR_DECODE_WIDTH; ++image_x) {
					image.set(TILE_PALETTE_COLOR(pixel.at((image_y * ROM_CHR_DECODE_WIDTH) + image_x)),
						image_x, image_y);
				}
			}

			image.save(path);

			TRACE_EXIT();
		}

		void
		extractor::decode_program_bank(
			__in const std::string &path,
			__in std::vector<nescc::core::memory<uint8_t>>::iterator &bank
			)
		{
			std::ofstream file;
			std::string result;

			TRACE_ENTRY_FORMAT("Path[%u]=%s, Bank=%p", path.size(), STRING_CHECK(path), &bank);

			result = nescc::console::cpu::bank_as_string(bank, true);

			file = std::ofstream(path.c_str(), std::ios::out | std::ios::trunc);
			if(!file) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_NOT_CREATED,
					"Path[%u]=%s", path.size(), STRING_CHECK(path));
			}

			file.write((char *) result.c_str(), result.size());
			file.close();

			TRACE_EXIT();
		}

		std::string
		extractor::display_help(
			__in_opt bool verbose
			) const
		{
			int iter = 0;
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_EXTRACTOR << display_version(verbose)
				<< std::endl << NESCC_COPYRIGHT
				<< std::endl << std::endl << display_usage();

			if(verbose) {

				for(; iter <= ARGUMENT_MAX; ++iter) {

					if(!iter) {
						result << std::endl;
					}

					result << std::endl << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter)
						<< " | " << ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << std::left
							<< std::setw(ARGUMENT_COLUMN_WIDTH) << ARGUMENT_STRING_LONG(iter)
						<< ARGUMENT_STRING_DESCRIPTION(iter);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		std::string
		extractor::display_usage(void) const
		{
			int iter = 0;
			std::stringstream result;

			TRACE_ENTRY();

			result << NESCC_EXTRACTOR;

			for(; iter <= ARGUMENT_MAX; ++iter) {
				result << " [" << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter)
					<< "|" << ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << ARGUMENT_STRING_LONG(iter) << "]";
			}

			result << " path";

			TRACE_EXIT();
			return result.str();
		}

		std::string
		extractor::display_version(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_VERSION_MAJOR << "." << NESCC_VERSION_MINOR << "." << NESCC_VERSION_WEEK
				<< "." << NESCC_VERSION_REVISION;

			if(verbose) {
				result << "-" << NESCC_VERSION_RELEASE;
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		extractor::extract_header(void)
		{
			uint32_t length;

			TRACE_ENTRY();

			if(std::memcmp(m_header.magic, CARTRIDGE_MAGIC, CARTRIDGE_MAGIC_LENGTH)) {
				uint8_t *expected = (uint8_t *) CARTRIDGE_MAGIC;

				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_MALFORMED,
					"Header mismatch: %02x%02x%02x%02x (expecting %02x%02x%02x%02x)",
					m_header.magic[0], m_header.magic[1], m_header.magic[2], m_header.magic[3],
					expected[0], expected[1], expected[2], expected[3]);
			}

			m_mapper = ((m_header.mapper_high << NIBBLE) | m_header.mapper_low);
			m_character_rom_count = m_header.rom_character;
			m_character_rom_size = (m_character_rom_count * CARTRIDGE_ROM_CHARACTER_WIDTH(m_mapper));
			m_character_rom_count ? m_character_rom_width = (m_character_rom_size / m_character_rom_count)
				: m_character_rom_width = 0;
			m_program_ram_count = m_header.ram_program;
			m_program_ram_size = (m_program_ram_count * CARTRIDGE_RAM_PROGRAM_WIDTH(m_mapper));
			m_program_rom_count = m_header.rom_program;
			m_program_rom_size = (m_program_rom_count * CARTRIDGE_ROM_PROGRAM_WIDTH(m_mapper));
			m_program_rom_count ? m_program_rom_width = (m_program_rom_size / m_program_rom_count)
				: m_program_rom_width = 0;
			m_program_rom_base = ((m_header.trainer ? CARTRIDGE_TRAINER_LENGTH : 0) + sizeof(m_header));
			m_character_rom_base = (m_program_rom_size + m_program_rom_base);
			m_mirroring = m_header.mirroring;
			m_tv_system = m_header.tv_system;

			length = (m_program_rom_base + m_program_rom_size);
			if(length != m_character_rom_base) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_MALFORMED,
					"PRG ROM offset does not match expected: %.01f KB (%u bytes) (expecting: %.01f KB (%u bytes))",
					length / KILOBYTE, length, m_character_rom_base / KILOBYTE, m_character_rom_base);
			}

			length = (m_character_rom_base + m_character_rom_size);
			if(length != m_length) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_MALFORMED,
					"CHR ROM offset does not match expected: %.01f KB (%u bytes) (expecting: %.01f KB (%u bytes))",
					length / KILOBYTE, length, m_length / KILOBYTE, m_length);
			}

			TRACE_EXIT();
		}

		bool
		extractor::extract_path(
			__in const std::string &path,
			__inout std::string &directory,
			__inout std::string &file,
			__inout std::string &extension
			)
		{
			size_t position;
			bool result = false;
			std::string input_path = path;

			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			directory.clear();
			file.clear();
			extension.clear();

			if(!input_path.empty()) {

				position = input_path.find_last_of(EXTRACT_DIRECTORY_DELIMITER);
				if(position != input_path.npos) {
					directory = input_path.substr(0, position);
					input_path = input_path.substr(position + 1, input_path.size());

					position = input_path.find_last_of(EXTRACT_EXTENSION_DELIMITER);
					if(position != input_path.npos) {
						file = input_path.substr(0, position);
						extension = input_path.substr(position + 1, input_path.size());
						result = true;
					}
				}
			}

			TRACE_EXIT_FORMAT("Result=%x, Directory[%u]=%s, File[%u]=%s, Extension[%u]=%s", result, directory.size(),
				STRING_CHECK(directory), file.size(), STRING_CHECK(file), extension.size(), STRING_CHECK(extension));
			return result;
		}

		std::string
		extractor::extract_rom_character(
			__in_opt bool decode
			)
		{
			uint32_t offset;
			std::ifstream input;
			std::stringstream result;
			std::vector<nescc::core::memory<uint8_t>> bank;
			std::string directory, extension, file;

			TRACE_ENTRY_FORMAT("Decode=%x", decode);

			result << "Extracting character banks from " << m_path
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- CHR ROM: "
					<< m_character_rom_count << " (" << FLOAT_PRECISION(1, m_character_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Base: "
					<< m_character_rom_base << " (" << FLOAT_PRECISION(1, m_character_rom_base / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Length: "
					<< m_character_rom_size << " (" << FLOAT_PRECISION(1, m_character_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Width: "
					<< m_character_rom_width << " (" << FLOAT_PRECISION(1, m_character_rom_width / KILOBYTE) << " KB)"
				<< std::endl;

			input = std::ifstream(m_path.c_str(), std::ios::in | std::ios::binary);
			if(!input) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_NOT_FOUND, "Path[%u]=%s",
					m_path.size(), STRING_CHECK(m_path));
			}

			input.seekg(m_character_rom_base, std::ios::beg);

			for(offset = m_character_rom_base; offset < (m_character_rom_base + m_character_rom_size);
					offset += m_character_rom_width) {
				bank.push_back(nescc::core::memory<uint8_t>());

				if(m_character_rom_width) {
					bank.back().set_size(m_character_rom_width);
					input.read((char *) bank.back().raw(), bank.back().size());
				}
			}

			input.close();

			if(!bank.empty()) {
				uint16_t count = 0;
				std::vector<nescc::core::memory<uint8_t>>::iterator iter;

				if(!extract_path(m_path, directory, file, extension)) {
					directory = EXTRACT_DIRECTORY_DEFAULT;
					file = EXTRACT_FILE_DEFAULT;
				}

				for(iter = bank.begin(); iter != bank.end(); ++count, ++iter) {
					std::ofstream output;
					std::stringstream path;

					path << directory << DIRECTORY_DELIMITER << file << "-" << count << EXTENSION_DELIMITER
						<< EXTRACT_ROM_CHR_EXTENSION;
					result << std::endl << "Writing character bank-" << count << ", " << FLOAT_PRECISION(1, iter->size() / KILOBYTE)
						<< " KB (" << iter->size() << " bytes) to " << path.str() << "...";

					output = std::ofstream(path.str().c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
					if(!output) {
						THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_NOT_CREATED,
							"Path[%u]=%s", path.str().size(), STRING_CHECK(path.str()));
					}

					output.write((char *) iter->raw(), iter->size());
					output.close();
					result << " Done.";

					if(decode) {
						path.clear();
						path.str(std::string());
						path << directory << DIRECTORY_DELIMITER << file << "-" << count << EXTENSION_DELIMITER
							<< EXTRACT_ROM_CHR_DECODE_EXTENSION;
						result << std::endl << "Writing character bank-" << count << " image to " << path.str() << "...";
						decode_character_bank(path.str(), iter);
						result << " Done.";
					}
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		std::string
		extractor::extract_rom_program(
			__in bool decode
			)
		{
			uint32_t offset;
			std::ifstream input;
			std::stringstream result;
			std::vector<nescc::core::memory<uint8_t>> bank;
			std::string directory, extension, file;

			TRACE_ENTRY_FORMAT("Decode=%x", decode);

			result << "Extracting program banks from " << m_path
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- PRG ROM: "
					<< m_program_rom_count << " (" << FLOAT_PRECISION(1, m_program_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Base: "
					<< m_program_rom_base << " (" << FLOAT_PRECISION(1, m_program_rom_base / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Length: "
					<< m_program_rom_size << " (" << FLOAT_PRECISION(1, m_program_rom_size / KILOBYTE) << " KB)"
				<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "   |- Width: "
					<< m_program_rom_width << " (" << FLOAT_PRECISION(1, m_program_rom_width / KILOBYTE) << " KB)"
				<< std::endl;

			input = std::ifstream(m_path.c_str(), std::ios::in | std::ios::binary);
			if(!input) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_NOT_FOUND, "Path[%u]=%s",
					m_path.size(), STRING_CHECK(m_path));
			}

			input.seekg(m_program_rom_base, std::ios::beg);

			for(offset = m_program_rom_base; offset < (m_program_rom_base + m_program_rom_size);
					offset += m_program_rom_width) {
				bank.push_back(nescc::core::memory<uint8_t>());

				if(m_program_rom_width) {
					bank.back().set_size(m_program_rom_width);
					input.read((char *) bank.back().raw(), bank.back().size());
				}
			}

			input.close();

			if(!bank.empty()) {
				uint16_t count = 0;
				std::vector<nescc::core::memory<uint8_t>>::iterator iter;

				if(!extract_path(m_path, directory, file, extension)) {
					directory = EXTRACT_DIRECTORY_DEFAULT;
					file = EXTRACT_FILE_DEFAULT;
				}

				for(iter = bank.begin(); iter != bank.end(); ++count, ++iter) {
					std::ofstream output;
					std::stringstream path;

					path << directory << DIRECTORY_DELIMITER << file << "-" << count << EXTENSION_DELIMITER
						<< EXTRACT_ROM_PRG_EXTENSION;
					result << std::endl << "Writing program bank-" << count << ", " << FLOAT_PRECISION(1, iter->size() / KILOBYTE)
						<< " KB (" << iter->size() << " bytes) to " << path.str() << "...";

					output = std::ofstream(path.str().c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
					if(!output) {
						THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_NOT_CREATED,
							"Path[%u]=%s", path.str().size(), STRING_CHECK(path.str()));
					}

					output.write((char *) iter->raw(), iter->size());
					output.close();
					result << " Done.";

					if(decode) {
						path.clear();
						path.str(std::string());
						path << directory << DIRECTORY_DELIMITER << file << "-" << count << EXTENSION_DELIMITER
							<< EXTRACT_ROM_PRG_DECODE_EXTENSION;
						result << std::endl << "Writing program bank-" << count << " disassembly to " << path.str() << "...";
						decode_program_bank(path.str(), iter);
						result << " Done.";
					}
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		nescc::console::cartridge_header
		extractor::header(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION(NESCC_TOOL_EXTRACTOR_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_header;
		}

		void
		extractor::invoke(
			__in const std::vector<std::string> &arguments
			)
		{
			int index = 1;
			std::stringstream result;
			std::vector<std::string>::const_iterator iter;
			bool decode = false, extract_chr = false, extract_prg = false, help = false, verbose = false,
				version = false;

			TRACE_ENTRY_FORMAT("Arguments[%u]=%p", arguments.size(), &arguments);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION(NESCC_TOOL_EXTRACTOR_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(arguments.size() <= 1) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_ARGUMENT_MISSING, "%s",
					STRING_CHECK(display_usage()));
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
						THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_ARGUMENT_MALFORMED,
							"Argument[%u]=%s", index, (*iter).c_str());
					}

					switch(entry->second) {
						case ARGUMENT_DECODE:
							decode = true;
							break;
						case ARGUMENT_EXTRACT_CHR:
							extract_chr = true;
							break;
						case ARGUMENT_EXTRACT_PRG:
							extract_prg = true;
							break;
						case ARGUMENT_HELP:
							help = true;
							break;
						case ARGUMENT_VERBOSE:
							verbose = true;
							break;
						case ARGUMENT_VERSION:
							version = true;
							break;
						default:
							THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_ARGUMENT_UNSUPPORTED,
								"Argument[%u]=%s", index, (*iter).c_str());
					}
				} else {

					if(!m_path.empty()) {
						THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_PATH_REASSIGNED, "%s",
							STRING_CHECK(display_usage()));
					}

					m_path = str;
				}
			}

			if(help) {
				std::cout << display_help(true) << std::endl;
			} else if(version) {
				std::cout << display_version(true) << std::endl;
			} else if(m_path.empty()) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_PATH_UNASSIGNED, "%s",
					STRING_CHECK(display_usage()));
			} else {
				load(m_path, decode, extract_chr, extract_prg, verbose);

				if(!extract_chr && !extract_prg) {

					result << as_string(verbose);
					if(!result.str().empty()) {
						std::cout << result.str() << std::endl;
					}
				}
			}

			TRACE_EXIT();
		}

		void
		extractor::load(
			__in const std::string &path,
			__in_opt bool decode,
			__in_opt bool extract_chr,
			__in_opt bool extract_prg,
			__in_opt bool verbose
			)
		{
			int length;
			std::ifstream file;
			std::string input_path;

			TRACE_ENTRY_FORMAT("Path[%u]=%s, Decode=%x, Extract={chr=%x, prg=%x}, Verbose=%x", path.size(), STRING_CHECK(path),
				decode, extract_chr, extract_prg, verbose);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION(NESCC_TOOL_EXTRACTOR_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			input_path = path;
			clear();
			m_path = input_path;

			file = std::ifstream(path.c_str(), std::ios::in | std::ios::binary);
			if(!file) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_NOT_FOUND, "Path[%u]=%s",
					path.size(), STRING_CHECK(path));
			}

			file.seekg(0, std::ios::end);

			length = file.tellg();
			if(length < sizeof(m_header)) {
				THROW_NESCC_TOOL_EXTRACTOR_EXCEPTION_FORMAT(NESCC_TOOL_EXTRACTOR_EXCEPTION_FILE_MALFORMED, "Path[%u]=%s",
					path.size(), STRING_CHECK(path));
			}

			m_length = length;
			file.seekg(0, std::ios::beg);
			m_raw.set_size(sizeof(m_header));
			file.read((char *) m_raw.raw(), m_raw.size());
			file.close();
			std::memcpy(&m_header, m_raw.raw(), m_raw.size());
			extract_header();

			if(extract_chr && m_character_rom_count) {
				std::cout << extract_rom_character(decode) << std::endl;
			}

			if(extract_prg && m_program_rom_count) {
				std::cout << extract_rom_program(decode) << std::endl;
			}

			TRACE_EXIT();
		}

		bool
		extractor::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Extractor initializing...");

			TRACE_MESSAGE(TRACE_INFORMATION, "Extractor initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		extractor::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Extractor uninitializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Extractor uninitialized.");

			TRACE_EXIT();
		}

		std::string
		extractor::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_TOOL_EXTRACTOR_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::tool::extractor>::to_string(verbose);

				if(m_initialized) {
					result << ", Path[" << m_path.size() << "]=" << STRING_CHECK(m_path)
						<< ", Header=" << SCALAR_AS_HEX(uintptr_t, &m_header)
						<< ", Raw=" << m_raw.to_string(verbose);
				}
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}
