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

#include "../../include/console/ppu.h"
#include "../../include/console/cartridge.h"
#include "../../include/core/bitmap.h"
#include "../../include/trace.h"
#include "./ppu_type.h"

namespace nescc {

	namespace console {

		ppu::ppu(void) :
			m_address(0),
			m_attribute_table_current(0),
			m_attribute_table_latch_high(false),
			m_attribute_table_latch_low(false),
			m_attribute_table_shift_high(0),
			m_attribute_table_shift_low(0),
			m_background_high(0),
			m_background_low(0),
			m_background_shift_high(0),
			m_background_shift_low(0),
			m_cycle(0),
			m_debug(false),
			m_dot(0),
			m_fine_x(0),
			m_frame_odd(false),
			m_mirroring(0),
			m_nametable_current(0),
			m_port_latch(false),
			m_port_value(0),
			m_port_value_buffer(0),
			m_scanline(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		ppu::~ppu(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		uint8_t
		ppu::address(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

			result = m_port.read(PPU_PORT_ADDRESS);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		std::string
		ppu::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << std::left << std::setw(COLUMN_WIDTH_LONG) << "Control" << SCALAR_AS_HEX(uint8_t, m_control.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Nametable"
						<< SCALAR_AS_HEX(uint8_t, m_control.nametable)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Increment"
						<< SCALAR_AS_HEX(uint8_t, m_control.increment)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Sprite table"
						<< SCALAR_AS_HEX(uint8_t, m_control.sprite_pattern_table)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Background table"
						<< SCALAR_AS_HEX(uint8_t, m_control.background_pattern_table)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Sprite size"
						<< SCALAR_AS_HEX(uint8_t, m_control.sprite_size)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Slave mode"
						<< SCALAR_AS_HEX(uint8_t, m_control.slave)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- NMI enabled"
						<< SCALAR_AS_HEX(uint8_t, m_control.nmi);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Mask" << SCALAR_AS_HEX(uint8_t, m_mask.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Greyscale"
						<< SCALAR_AS_HEX(uint8_t, m_mask.greyscale)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Background left"
						<< SCALAR_AS_HEX(uint8_t, m_mask.background_left)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Sprite left"
						<< SCALAR_AS_HEX(uint8_t, m_mask.sprite_left)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Background enable"
						<< SCALAR_AS_HEX(uint8_t, m_mask.background)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Sprite enable"
						<< SCALAR_AS_HEX(uint8_t, m_mask.sprite)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Red"
						<< SCALAR_AS_HEX(uint8_t, m_mask.red)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Green"
						<< SCALAR_AS_HEX(uint8_t, m_mask.green)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Blue"
						<< SCALAR_AS_HEX(uint8_t, m_mask.blue);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Status" << SCALAR_AS_HEX(uint8_t, m_status.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Sprite overflow"
						<< SCALAR_AS_HEX(uint8_t, m_status.sprite_overflow)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Sprite 0 hit"
						<< SCALAR_AS_HEX(uint8_t, m_status.sprite_0_hit)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- VBlank"
						<< SCALAR_AS_HEX(uint8_t, m_status.vertical_blank);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "OAM address"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_ADDRESS))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "OAM data"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_DATA))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Scroll"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_SCROLL))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Address"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_ADDRESS))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Data"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_DATA))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << std::endl << "Cycle"
					<< m_cycle
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << std::endl << "Scanline/Dot"
					<< m_scanline << ", " << m_dot
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Coarse X/Y"
					<< SCALAR_AS_HEX(uint8_t, m_address_temp.coarse_x) << ", " << SCALAR_AS_HEX(uint8_t, m_address_temp.coarse_y)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Fine X/Y"
					<< SCALAR_AS_HEX(uint8_t, m_fine_x) << ", " << SCALAR_AS_HEX(uint8_t, m_address_temp.fine_y)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Nametable"
					<< SCALAR_AS_HEX(uint8_t, m_address_temp.nametable)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Frame"
					<< (m_frame_odd ? "Odd" : "Even") << std::endl
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Current address"
					<< SCALAR_AS_HEX(uint16_t, m_address)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Current attribute table"
					<< SCALAR_AS_HEX(uint8_t, m_attribute_table_current)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Current nametable"
					<< SCALAR_AS_HEX(uint8_t, m_nametable_current);

			TRACE_EXIT();
			return result.str();
		}

		uint16_t
		ppu::calculate_attribute_table_address(void)
		{
			uint16_t result;

			TRACE_ENTRY();

			result = (PPU_ATTRIBUTE_TABLE_BASE | (m_address_vram.nametable * PPU_NAMETABLE_LENGTH)
					| ((m_address_vram.coarse_y >> 2) << 3) | (m_address_vram.coarse_x >> 2));

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result);
			return result;
		}

		uint16_t
		ppu::calculate_background_address(void)
		{
			uint16_t result;

			TRACE_ENTRY();

			result = ((m_control.background_pattern_table * PPU_BACKGROUND_PATTERN_TABLE_LENGTH)
					+ (m_nametable_current << 4) + m_address_vram.fine_y);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result);
			return result;
		}

		uint16_t
		ppu::calculate_nametable_address(void)
		{
			uint16_t result;

			TRACE_ENTRY();

			result = (PPU_NAMETABLE_BASE | (m_address_vram.raw & 0xfff));

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result);
			return result;
		}

		void
		ppu::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu clearing...");

			m_address = 0;
			m_address_temp.raw = 0;
			m_address_vram.raw = 0;
			m_attribute_table_current = 0;
			m_attribute_table_latch_high = false;
			m_attribute_table_latch_low = false;
			m_attribute_table_shift_high = 0;
			m_attribute_table_shift_low = 0;
			m_background_high = 0;
			m_background_low = 0;
			m_background_shift_high = 0;
			m_background_shift_low = 0;
			m_control.raw = 0;
			m_cycle = 0;
			m_debug = false;
			m_dot = 0;
			m_fine_x = 0;
			m_frame_odd = false;
			m_mask.raw = 0;
			m_mirroring = 0;
			m_nametable.clear();
			m_nametable_current = 0;
			m_oam.clear();
			m_palette.clear();
			m_port.clear();
			m_port_latch = false;
			m_port_value = 0;
			m_port_value_buffer = 0;
			m_scanline = 0;
			m_sprite.clear();
			m_sprite_secondary.clear();
			m_status.raw = 0;

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu cleared.");

			TRACE_EXIT();
		}

		void
		ppu::clear_oam_secondary(void)
		{
			TRACE_ENTRY();

			m_sprite_secondary.clear();
			m_sprite_secondary.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);

			TRACE_EXIT();
		}

		nescc::console::port_control_t
		ppu::control(void) const
		{
			nescc::console::port_control_t result;

			TRACE_ENTRY();

			result = m_control;

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result.raw, result.raw);
			return result;
		}

		uint32_t
		ppu::cycle(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_cycle);
			return m_cycle;
		}

		uint8_t
		ppu::data(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

			result = m_port.read(PPU_PORT_DATA);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint32_t
		ppu::dot(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_dot);
			return m_dot;
		}

		void
		ppu::execute_post_render(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(m_dot == PPU_DOT_POST_BEGIN) {
				bus.display_update();
			}

			TRACE_EXIT();
		}

		void
		ppu::execute_render(
			__in nescc::console::interface::bus &bus,
			__in int type
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Type=%u(%s)", &bus, type, PPU_RENDER_STRING(type));

			switch(type) {
				case PPU_RENDER_PRE_RENDER:
				case PPU_RENDER_VISIBLE:
					execute_render_sprite(bus, type);
					execute_render_background(bus, type);

					if((m_dot == PPU_DOT_MAPPER_INTERRUPT) && (m_mask.background || m_mask.sprite)) {
						bus.mapper_interrupt();
					}
					break;
				default:
					break;
			}

			TRACE_EXIT();
		}

		void
		ppu::execute_render_background(
			__in nescc::console::interface::bus &bus,
			__in int type
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Type=%u(%s)", &bus, type, PPU_RENDER_STRING(type));

			switch(m_dot) {
				case PPU_DOT_VBLANK_CLEAR: // 1
					m_address = calculate_nametable_address();

					if(type == PPU_RENDER_PRE_RENDER) {
						m_status.vertical_blank = 0;
					}
					break;
				case PPU_DOT_RENDER_PIXEL_LOW_MIN ... PPU_DOT_RENDER_PIXEL_LOW_MAX: // 2 - 255
				case PPU_DOT_RENDER_PIXEL_HIGH_MIN ... PPU_DOT_RENDER_PIXEL_HIGH_MAX: // 322 - 337
					generate_pixel(bus);

					switch(m_dot % (PPU_RENDER_PIXEL_MAX + 1)) {
						case PPU_RENDER_PIXEL_NAMETABLE_CALCULATE:
							m_address = calculate_nametable_address();
							update_shift();
							break;
						case PPU_RENDER_PIXEL_NAMETABLE_READ:
							m_nametable_current = bus.ppu_read(m_address);
							break;
						case PPU_RENDER_PIXEL_ATTRIBUTE_TABLE_CALCULATE:
							m_address = calculate_attribute_table_address();
							break;
						case PPU_RENDER_PIXEL_ATTRIBUTE_TABLE_READ:
							m_attribute_table_current = bus.ppu_read(m_address);

							if(m_address_vram.coarse_y & 2) {
								m_attribute_table_current >>= 4;
							}

							if(m_address_vram.coarse_x & 2) {
								m_attribute_table_current >>= 2;
							}
							break;
						case PPU_RENDER_PIXEL_BACKGROUND_LOW_CALCULATE:
							m_address = calculate_background_address();
							break;
						case PPU_RENDER_PIXEL_BACKGROUND_LOW_READ:
							m_background_low = bus.ppu_read(m_address);
							break;
						case PPU_RENDER_PIXEL_BACKGROUND_HIGH_CALCULATE:
							m_address += PPU_TILE_WIDTH;
							break;
						case PPU_RENDER_PIXEL_BACKGROUND_HIGH_READ:
							m_background_high = bus.ppu_read(m_address);
							update_scroll_horizontal();
							break;
						default:
							break;
					}
					break;
				case PPU_DOT_SCROLL_VERTICAL: // 256
					generate_pixel(bus);
					m_background_high = bus.ppu_read(m_address);
					update_scroll_vertical();
					break;
				case PPU_DOT_POSITION_UPDATE_HORIZONTAL: // 257
					generate_pixel(bus);
					update_shift();
					update_position_horizontal();
					break;
				case PPU_DOT_POSITION_UPDATE_VERTICAL_MIN ... PPU_DOT_POSITION_UPDATE_VERTICAL_MAX: // 280 - 304

					if(type == PPU_RENDER_PRE_RENDER) {
						update_position_vertical();
					}
					break;
				case PPU_DOT_NAMETABLE_UPDATE_LOW: // 321
				case PPU_DOT_NAMETABLE_UPDATE_HIGH: // 339
					m_address = calculate_nametable_address();
					break;
				case PPU_DOT_NAMETABLE_READ_LOW: // 338
					m_nametable_current = bus.ppu_read(m_address);
					break;
				case PPU_DOT_NAMETABLE_READ_HIGH: // 340
					m_nametable_current = bus.ppu_read(m_address);

					if(m_frame_odd && (type == PPU_RENDER_PRE_RENDER)
							&& (m_mask.background || m_mask.sprite)) {
						++m_dot;
					}
					break;
				default:
					break;
			}

			TRACE_EXIT();
		}

		void
		ppu::execute_render_sprite(
			__in nescc::console::interface::bus &bus,
			__in int type
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Type=%u(%s)", &bus, type, PPU_RENDER_STRING(type));

			switch(m_dot) {
				case PPU_DOT_CLEAR_OAM: // 1
					clear_oam_secondary();

					if(type == PPU_RENDER_PRE_RENDER) {
						m_status.sprite_overflow = 0;
						m_status.sprite_0_hit = 0;
					}
					break;
				case PPU_DOT_SPRITE_EVALUATE: // 257
					sprite_evaluate();
					break;
				case PPU_DOT_SPRITE_LOAD: // 321
					sprite_load(bus);
					break;
				default:
					break;
			}

			TRACE_EXIT();
		}

		void
		ppu::execute_vblank(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(m_dot == PPU_DOT_VBLANK) {
				m_status.vertical_blank = 1;

				if(m_control.nmi) {
					bus.cpu_interrupt_non_maskable();
				}
			}

			TRACE_EXIT();
		}

		void
		ppu::generate_pixel(
			__in nescc::console::interface::bus &bus
			)
		{
			int dot = (m_dot - PPU_DOT_RENDER_PIXEL_LOW_MIN);

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if((m_scanline < PPU_SCANLINE_POST_RENDER) && (dot >= 0) && (dot < PPU_DOT_SCROLL_VERTICAL)) {
				bool priority = false;
				nescc::core::pixel_t color;
				uint8_t palette = 0, palette_object = 0;

				generate_pixel_background(dot, palette);
				generate_pixel_sprite(dot, palette, palette_object, priority);

				if(palette_object && (!palette || !priority)) {
					palette = palette_object;
				}

				if(!m_mask.background && !m_mask.sprite) {
					palette = 0;
				}

				color.raw = PPU_PALETTE_COLOR(bus.ppu_read(PPU_PALETTE_TABLE_BASE + palette));
				if(m_mask.greyscale) {
					uint8_t average = ((color.blue + color.green + color.red) / 3);
					color.blue = average;
					color.green = average;
					color.red = average;
				}

				bus.display_write(dot, m_scanline, color.raw);
			}

			m_background_shift_high <<= 1;
			m_background_shift_low <<= 1;
			m_attribute_table_shift_high = ((m_attribute_table_shift_high << 1) | (m_attribute_table_latch_high));
			m_attribute_table_shift_low = ((m_attribute_table_shift_low << 1) | (m_attribute_table_latch_low));

			TRACE_EXIT();
		}

		void
		ppu::generate_pixel_background(
			__in int dot,
			__inout uint8_t &palette
			)
		{
			TRACE_ENTRY_FORMAT("Dot=%u, Palette=%p", dot, &palette);

			if(m_mask.background && !(!m_mask.background_left && (dot < PPU_TILE_WIDTH))) {

				palette = ((((m_background_shift_high >> ((PPU_BLOCK_WIDTH - 1) - m_fine_x)) & 1) << 1)
						| ((m_background_shift_low >> ((PPU_BLOCK_WIDTH - 1) - m_fine_x)) & 1));
				if(palette) {
					palette |= (((((m_attribute_table_shift_high >> ((PPU_TILE_WIDTH - 1) - m_fine_x)) & 1) << 1)
							| ((m_attribute_table_shift_low >> ((PPU_TILE_WIDTH - 1) - m_fine_x)) & 1)) << 2);
				}
			}

			TRACE_EXIT();
		}

		void
		ppu::generate_pixel_sprite(
			__in int dot,
			__in uint8_t palette,
			__inout uint8_t &palette_object,
			__inout bool &priority
			)
		{
			TRACE_ENTRY_FORMAT("Dot=%u, Palette=%u, Palette Object=%p, Priority=%p", dot, palette, &palette_object, &priority);

			if(m_mask.sprite && !(!m_mask.sprite_left && (dot < PPU_SPRITE_LENGTH))) {
				std::vector<nescc::console::sprite_t>::reverse_iterator entry;

				for(entry = m_sprite.rbegin(); entry != m_sprite.rend(); ++entry) {

					if((entry->id != PPU_SPRITE_ID_INVALID) && (entry->position_x <= dot)) {

						int sprite_x = (dot - entry->position_x);
						if(sprite_x < PPU_SPRITE_LENGTH) {
							uint8_t palette_sprite;

							if(entry->attributes & 0x40) {
								sprite_x ^= (PPU_SPRITE_LENGTH - 1);
							}

							palette_sprite = ((((entry->data_high >> ((PPU_SPRITE_LENGTH - 1) - sprite_x)) & 1) << 1)
										| ((entry->data_low >> ((PPU_SPRITE_LENGTH - 1) - sprite_x)) & 1));

							if(palette_sprite) {

								if(!entry->id && palette && (dot != PPU_DOT_RENDER_PIXEL_LOW_MAX)) {
									m_status.sprite_0_hit = 1;
								}

								palette_object = ((palette_sprite | ((entry->attributes & 3) << 2))
											+ PPU_SPRITE_LENGTH_LONG);
								priority = ((entry->attributes & 0x20) ? true : false);
							}
						}
					}
				}
			}

			TRACE_EXIT();
		}

		nescc::console::port_mask_t
		ppu::mask(void) const
		{
			nescc::console::port_mask_t result;

			TRACE_ENTRY();

			result = m_mask;

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result.raw, result.raw);
			return result;
		}

		nescc::core::memory &
		ppu::nametable(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_nametable;
		}

		nescc::core::memory &
		ppu::oam(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_oam;
		}

		uint8_t
		ppu::oam_address(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

			result = m_port.read(PPU_PORT_OAM_ADDRESS);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::oam_data(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

			result = m_port.read(PPU_PORT_OAM_DATA);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		bool
		ppu::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu initializing...");

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		ppu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu uninitializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu uninitialized.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		ppu::palette(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_palette;
		}

		nescc::core::memory &
		ppu::port(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_port;
		}

		uint8_t
		ppu::read_nametable(
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(m_mirroring) {
				case CARTRIDGE_MIRRORING_HORIZONTAL:
					address = (((address >> 1) & PPU_NAMETABLE_LENGTH) + (address % PPU_NAMETABLE_LENGTH));
					break;
				case CARTRIDGE_MIRRORING_VERTICAL:
					address %= (PPU_NAMETABLE_LENGTH << 1);
					break;
				default:
					break;
			}

			result = m_nametable.read(address);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu nametable read", "[%04x] -> %u(%02x)", address, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_palette(
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			address %= PPU_PALETTE_LENGTH;
			switch(address) {
				case 0x10: // mirrored addresses
				case 0x14:
				case 0x18:
				case 0x1c:
					address -= PPU_PALETTE_TABLE_OFFSET;
					break;
				default:
					break;
			}

			result = m_palette.read(address);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu palette read", "[%04x] -> %u(%02x)", address, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port(
			__in nescc::console::interface::bus &bus,
			__in uint8_t port
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Port=%u(%s)", &bus, port, PPU_PORT_STRING(port));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(port) {
				case PPU_PORT_STATUS: // 0x2002
					result = read_port_status();
					break;
				case PPU_PORT_OAM_DATA: // 0x2004
					result = read_port_oam_data();
					break;
				case PPU_PORT_DATA: // 0x2007
					result = read_port_data(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, PPU_PORT_STRING(port));
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu port read", "[%s] -> %u(%02x)", PPU_PORT_STRING(port),
				result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port_data(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(m_address_vram.address <= PPU_DATA_ADDRESS_MAX) {
				m_port_value = m_port_value_buffer;
				m_port_value_buffer = bus.ppu_read(m_address_vram.address);
			} else {
				m_port_value = bus.ppu_read(m_address_vram.address);
				m_port_value_buffer = m_port_value;
			}

			m_address_vram.address += (m_control.increment ? 32 : 1);
			m_port.write(PPU_PORT_DATA, m_port_value);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_port_value, m_port_value);
			return m_port_value;
		}

		uint8_t
		ppu::read_port_oam_data(void)
		{
			TRACE_ENTRY();

			m_port_value = m_oam.read(m_port.read(PPU_PORT_OAM_ADDRESS));
			m_port.write(PPU_PORT_OAM_DATA, m_port_value);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_port_value, m_port_value);
			return m_port_value;
		}

		uint8_t
		ppu::read_port_status(void)
		{
			TRACE_ENTRY();

			m_port_value = ((m_port_value & 0x1f) | m_status.raw);
			m_port.write(PPU_PORT_STATUS, m_port_value);
			m_status.vertical_blank = 0;
			m_port_latch = false;

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_port_value, m_port_value);
			return m_port_value;
		}

		void
		ppu::reset(
			__in nescc::console::interface::bus &bus,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Debug=%x", &bus, debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu resetting...");

			m_address = 0;
			m_address_temp.raw = 0;
			m_address_vram.raw = 0;
			m_attribute_table_current = 0;
			m_attribute_table_latch_high = false;
			m_attribute_table_latch_low = false;
			m_attribute_table_shift_high = 0;
			m_attribute_table_shift_low = 0;
			m_background_high = 0;
			m_background_low = 0;
			m_background_shift_high = 0;
			m_background_shift_low = 0;
			m_control.raw = 0;
			m_cycle = 0;
			m_debug = debug;
			m_dot = 0;
			m_fine_x = 0;
			m_frame_odd = false;
			m_mask.raw = 0;
			m_mirroring = bus.mirroring();
			m_nametable.set_size(PPU_NAMETABLE_LENGTH * 2);
			m_nametable_current = 0;
			m_oam.set_size(PPU_OAM_LENGTH);
			m_palette.set_size(PPU_PALETTE_LENGTH);
			m_port.set_size(PPU_PORT_MAX + 1);
			m_port_latch = false;
			m_port_value = 0;
			m_port_value_buffer = 0;
			m_scanline = 0;
			m_sprite.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);
			m_sprite_secondary.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);
			m_status.raw = 0;

			TRACE_DEBUG(m_debug, "Ppu reset");
			TRACE_DEBUG_FORMAT(m_debug, "Ppu state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu reset.");

			TRACE_EXIT();
		}

		uint32_t
		ppu::scanline(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_scanline);
			return m_scanline;
		}

		uint8_t
		ppu::scroll(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

			result = m_port.read(PPU_PORT_SCROLL);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		ppu::set_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_ADDRESS, value);

			TRACE_EXIT();
		}

		void
		ppu::set_control(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_control = *((nescc::console::port_control_t *) &value);

			TRACE_EXIT();
		}

		void
		ppu::set_data(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_DATA, value);

			TRACE_EXIT();
		}

		void
		ppu::set_mask(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_mask = *((nescc::console::port_mask_t *) &value);

			TRACE_EXIT();
		}

		void
		ppu::set_oam_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_OAM_ADDRESS, value);

			TRACE_EXIT();
		}

		void
		ppu::set_oam_data(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_OAM_DATA, value);

			TRACE_EXIT();
		}

		void
		ppu::set_scroll(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_SCROLL, value);

			TRACE_EXIT();
		}

		void
		ppu::set_status(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_status = *((nescc::console::port_status_t *) &value);

			TRACE_EXIT();
		}

		void
		ppu::sprite_evaluate(void)
		{
			uint8_t count = 0, iter = 0;

			TRACE_ENTRY();

			for(; iter < PPU_SPRITE_OAM_MAX; ++iter) {

				int offset = (iter << 2), row = (((m_scanline == PPU_SCANLINE_MAX) ? -1 : m_scanline) - m_oam.read(offset));
				if(((row >= 0) && (row < (m_control.sprite_size ? PPU_SPRITE_LENGTH_LONG : PPU_SPRITE_LENGTH)))) {
					nescc::console::sprite_t &entry = m_sprite_secondary.at(count);

					entry.attributes = m_oam.read(offset + PPU_SPRITE_ATTRIBUTES);
					entry.id = iter;
					entry.position_x = m_oam.read(offset + PPU_SPRITE_POSITION_X);
					entry.position_y = m_oam.read(offset + PPU_SPRITE_POSITION_Y);
					entry.tile = m_oam.read(offset + PPU_SPRITE_TILE_ID);

					if(++count >= PPU_SPRITE_LENGTH) {
						m_status.sprite_overflow = 1;
						break;
					}
				}
			}

			TRACE_EXIT();
		}

		void
		ppu::sprite_load(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t iter = 0;
			uint16_t address;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			for(; iter < PPU_SPRITE_LENGTH; ++iter) {
				uint8_t sprite_height, sprite_y;

				m_sprite.at(iter) = m_sprite_secondary.at(iter);

				nescc::console::sprite_t &entry = m_sprite.at(iter);

				sprite_height = (m_control.sprite_size ? PPU_SPRITE_LENGTH_LONG : PPU_SPRITE_LENGTH);

				if(sprite_height == PPU_SPRITE_LENGTH_LONG) {
					address = (((entry.tile & 1) << 12) + ((entry.tile & ~1) << 4));
				} else {
					address = ((m_control.sprite_pattern_table << 12) + (entry.tile << 4));
				}

				sprite_y = ((m_scanline - entry.position_y) % sprite_height);

				if(entry.attributes & 0x80) {
					sprite_y ^= (sprite_height - 1);
				}

				address += ((sprite_y & 8) + sprite_y);
				entry.data_low = bus.ppu_read(address);
				entry.data_high = bus.ppu_read(address + PPU_SPRITE_LENGTH);
			}

			TRACE_EXIT();
		}

		nescc::console::port_status_t
		ppu::status(void) const
		{
			nescc::console::port_status_t result;

			TRACE_ENTRY();

			result = m_status;

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result.raw, result.raw);
			return result;
		}

		std::string
		ppu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_PPU_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::ppu>::to_string(verbose);

				if(m_initialized) {
					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", Port=" << m_port.to_string(verbose)
						<< ", OAM=" << m_oam.to_string(verbose)
						<< ", Nametable=" << m_nametable.to_string(verbose)
						<< ", Palette=" << m_palette.to_string(verbose)
						<< ", Sprite={First=" << SCALAR_AS_HEX(uintptr_t, &m_sprite)
							<< ", Secondary=" << SCALAR_AS_HEX(uintptr_t, &m_sprite_secondary) << "}"
						<< ", Cycle=" << m_cycle
						<< ", Address={T=" << SCALAR_AS_HEX(uint16_t, m_address_temp.raw)
							<< ", V=" << SCALAR_AS_HEX(uint16_t, m_address_vram.raw) << "}"
						<< ", Port={Buffer={" << SCALAR_AS_HEX(uint8_t, m_port_value)
								<< ", " << SCALAR_AS_HEX(uint8_t, m_port_value_buffer) << "}"
							<< ", State=" << (m_port_latch ? "Latched" : "Unlatched")
							<< ", Control=" << SCALAR_AS_HEX(uint8_t, m_control.raw)
							<< ", Mask=" << SCALAR_AS_HEX(uint8_t, m_mask.raw)
							<< ", Status=" << SCALAR_AS_HEX(uint8_t, m_status.raw)
							<< ", OAM Address=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_ADDRESS))
							<< ", OAM Data=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_DATA))
							<< ", Scroll=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_SCROLL))
							<< ", Address=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_ADDRESS))
							<< ", Data=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_DATA)) << "}"
						<< ", Fine X=" << SCALAR_AS_HEX(uint8_t, m_fine_x)
						<< ", Frame=" << (m_frame_odd ? "Odd" : "Even")
						<< ", Mirroring=" << SCALAR_AS_HEX(uint8_t, m_mirroring)
						<< ", Position={Scanline=" << m_scanline << ", Dot=" << m_dot << "}"
						<< ", Current address=" << SCALAR_AS_HEX(uint16_t, m_address)
						<< ", Current attribute table=" << SCALAR_AS_HEX(uint8_t, m_attribute_table_current)
						<< ", Current nametable=" << SCALAR_AS_HEX(uint8_t, m_nametable_current);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		ppu::update(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_DEBUG_FORMAT(m_debug, "Ppu update", "%u, %u (%s)", m_scanline, m_dot,
				m_frame_odd ? "Odd" : "Even");

			switch(m_scanline) {
				case PPU_SCANLINE_VISIBLE_START ... PPU_SCANLINE_VISIBLE_END: // 0 - 239
					execute_render(bus, PPU_RENDER_VISIBLE);
					break;
				case PPU_SCANLINE_POST_RENDER: // 240
					execute_post_render(bus);
					break;
				case PPU_SCANLINE_VBLANK_START: // 241
					execute_vblank(bus);
					break;
				case PPU_SCANLINE_PRE_RENDER: // 261
					execute_render(bus, PPU_RENDER_PRE_RENDER);
					break;
				default:
					break;
			}

			if(++m_dot > PPU_DOT_MAX) {
				m_dot %= (PPU_DOT_MAX + 1);

				if(++m_scanline > PPU_SCANLINE_MAX) {
					m_scanline = 0;
					m_frame_odd = !m_frame_odd;
				}
			}

			++m_cycle;

			TRACE_DEBUG_FORMAT(m_debug, "Ppu state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_EXIT();
		}

		void
		ppu::update_position_horizontal(void)
		{
			TRACE_ENTRY();

			if(m_mask.background || m_mask.sprite) {
				m_address_vram.raw = ((m_address_vram.raw & ~0x41f) | (m_address_temp.raw & 0x41f));
			}

			TRACE_EXIT();
		}

		void
		ppu::update_position_vertical(void)
		{
			TRACE_ENTRY();

			if(m_mask.background || m_mask.sprite) {
				m_address_vram.raw = ((m_address_vram.raw & ~0x7be0) | (m_address_temp.raw & 0x7be0));
			}

			TRACE_EXIT();
		}

		void
		ppu::update_scroll_horizontal(void)
		{
			TRACE_ENTRY();

			if(m_mask.background || m_mask.sprite) {

				if(m_address_vram.coarse_x == 31) {
					m_address_vram.raw ^= 0x41f;
				} else {
					++m_address_vram.coarse_x;
				}
			}

			TRACE_EXIT();
		}

		void
		ppu::update_scroll_vertical(void)
		{
			TRACE_ENTRY();

			if(m_mask.background || m_mask.sprite) {

				if(m_address_vram.fine_y >= 7) {
					m_address_vram.fine_y = 0;

					if(m_address_vram.coarse_y == 29) {
						m_address_vram.coarse_y = 0;
						m_address_vram.nametable ^= 2;
					} else if(m_address_vram.coarse_y == 31) {
						m_address_vram.coarse_y = 0;
					} else {
						++m_address_vram.coarse_y;
					}
				} else {
					++m_address_vram.fine_y;
				}
			}

			TRACE_EXIT();
		}

		void
		ppu::update_shift(void)
		{
			TRACE_ENTRY();

			m_background_shift_high = ((m_background_shift_high & 0xff00) | m_background_high);
			m_background_shift_low = ((m_background_shift_low & 0xff00) | m_background_low);
			m_attribute_table_latch_high = ((m_attribute_table_current & 2) ? true : false);
			m_attribute_table_latch_low = ((m_attribute_table_current & 1) ? true : false);

			TRACE_EXIT();
		}

		void
		ppu::write_nametable(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(m_mirroring) {
				case CARTRIDGE_MIRRORING_HORIZONTAL:
					address = (((address >> 1) & PPU_NAMETABLE_LENGTH) + (address % PPU_NAMETABLE_LENGTH));
					break;
				case CARTRIDGE_MIRRORING_VERTICAL:
					address %= (PPU_NAMETABLE_LENGTH << 1);
					break;
				default:
					break;
			}

			m_nametable.write(address, value);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu nametable write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_oam(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_oam.write(address, value);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu oam write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_palette(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			address %= PPU_PALETTE_LENGTH;
			switch(address) {
				case 0x10: // mirrored addresses
				case 0x14:
				case 0x18:
				case 0x1c:
					address -= PPU_PALETTE_TABLE_OFFSET;
					break;
				default:
					break;
			}

			m_palette.write(address, value);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu palette write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port(
			__in nescc::console::interface::bus &bus,
			__in uint8_t port,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Port=%u(%s), Value=%u(%02x)", &bus, port, PPU_PORT_STRING(port), value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_port_value = value;

			switch(port) {
				case PPU_PORT_CONTROL: // 0x2000
					write_port_control(value);
					break;
				case PPU_PORT_MASK: // 0x2001
					write_port_mask(value);
					break;
				case PPU_PORT_OAM_ADDRESS: // 0x2003
					write_port_oam_address(value);
					break;
				case PPU_PORT_OAM_DATA: // 0x2004
					write_port_oam_data(value);
					break;
				case PPU_PORT_SCROLL: // 0x2005
					write_port_scroll(value);
					break;
				case PPU_PORT_ADDRESS: // 0x2006
					write_port_address(value);
					break;
				case PPU_PORT_DATA: // 0x2007
					write_port_data(bus, value);
					break;
				default:
					THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, PPU_PORT_STRING(port));
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu port write", "[%s] <- %u(%02x)", PPU_PORT_STRING(port),
				value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			if(!m_port_latch) {
				m_address_temp.high = (value & 0x3f);
				m_port.write(PPU_PORT_ADDRESS, value);
			} else {
				m_address_temp.low = value;
				m_address_vram.raw = m_address_temp.raw;
				m_port.write(PPU_PORT_ADDRESS, value);
			}

			m_port_latch = !m_port_latch;

			TRACE_EXIT();
		}

		void
		ppu::write_port_control(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_control.raw = value;
			m_address_temp.nametable = m_control.nametable;
			m_port.write(PPU_PORT_CONTROL, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_data(
			__in nescc::console::interface::bus &bus,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%02x)", &bus, value, value);

			bus.ppu_write(m_address_vram.address, value);
			m_address_vram.address += (m_control.increment ? 32 : 1);
			m_port.write(PPU_PORT_DATA, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_mask(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_mask.raw = value;
			m_port.write(PPU_PORT_MASK, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_OAM_ADDRESS, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_data(
			__in uint8_t value
			)
		{
			uint8_t address;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			address = m_port.read(PPU_PORT_OAM_ADDRESS);
			m_oam.write(address, value);
			m_port.write(PPU_PORT_OAM_DATA, value);
			m_port.write(PPU_PORT_OAM_ADDRESS, address + 1);

			TRACE_EXIT();
		}

		void
		ppu::write_port_scroll(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			if(!m_port_latch) {
				m_fine_x = (value & 7);
				m_address_temp.coarse_x = (value >> 3);
				m_port.write(PPU_PORT_SCROLL, value);
			} else {
				m_address_temp.fine_y = (value & 7);
				m_address_temp.coarse_y = (value >> 3);
				m_port.write(PPU_PORT_SCROLL, value);
			}

			m_port_latch = !m_port_latch;

			TRACE_EXIT();
		}
	}
}
