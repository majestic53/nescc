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

#ifndef NESCC_CORE_TOKEN_H_
#define NESCC_CORE_TOKEN_H_

#include <map>
#include <set>
#include <vector>
#include "./unique_id.h"

namespace nescc {

	namespace core {

		enum {
			ADDRESS_MODE_ABSOLUTE = 0, // <expression>
			ADDRESS_MODE_ABSOLUTE_X, // <expression> , x
			ADDRESS_MODE_ABSOLUTE_Y, // <expression> , y
			ADDRESS_MODE_ACCUMULATOR, // a
			ADDRESS_MODE_IMMEDIATE, // @ <expression>
			ADDRESS_MODE_IMPLIED,
			ADDRESS_MODE_INDIRECT, // ( <expression> )
			ADDRESS_MODE_INDIRECT_X, // ( <expression> , x )
			ADDRESS_MODE_INDIRECT_Y, // ( <expression> ) , y
			ADDRESS_MODE_RELATIVE, // <expression>
			ADDRESS_MODE_ZERO_PAGE, // <expression>
			ADDRESS_MODE_ZERO_PAGE_X, // <expression> , x
			ADDRESS_MODE_ZERO_PAGE_Y, // <expression> , y
		};

		#define ADDRESS_MODE_MAX ADDRESS_MODE_ZERO_PAGE_Y

		static const std::string ADDRESS_MODE_STR[] = {
			"Absolute", "Absolute-X", "Absolute-Y", "Accumulator", "Immediate", "Implied", "Indirect",
			"Indirect-X", "Indirect-Y", "Relative", "Zero-Page", "Zero-Page-X", "Zero-Page-Y",
			};

		#define ADDRESS_MODE_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::ADDRESS_MODE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::ADDRESS_MODE_STR[_TYPE_]))

		enum {
			COMMAND_ADC = 0,
			COMMAND_AND,
			COMMAND_ASL,
			COMMAND_BCC,
			COMMAND_BCS,
			COMMAND_BEQ,
			COMMAND_BIT,
			COMMAND_BMI,
			COMMAND_BNE,
			COMMAND_BPL,
			COMMAND_BRK,
			COMMAND_BVC,
			COMMAND_BVS,
			COMMAND_CLC,
			COMMAND_CLD,
			COMMAND_CLI,
			COMMAND_CLV,
			COMMAND_CMP,
			COMMAND_CPX,
			COMMAND_CPY,
			COMMAND_DEC,
			COMMAND_DEX,
			COMMAND_DEY,
			COMMAND_EOR,
			COMMAND_ILLEGAL_AHX,
			COMMAND_ILLEGAL_ALR,
			COMMAND_ILLEGAL_ANC,
			COMMAND_ILLEGAL_ARR,
			COMMAND_ILLEGAL_AXS,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_KIL,
			COMMAND_ILLEGAL_LAS,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_NOP,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SAX,
			COMMAND_ILLEGAL_SBC,
			COMMAND_ILLEGAL_SHX,
			COMMAND_ILLEGAL_SHY,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_ILLEGAL_TAS,
			COMMAND_ILLEGAL_XAA,
			COMMAND_INC,
			COMMAND_INX,
			COMMAND_INY,
			COMMAND_JMP,
			COMMAND_JSR,
			COMMAND_LDA,
			COMMAND_LDX,
			COMMAND_LDY,
			COMMAND_LSR,
			COMMAND_NOP,
			COMMAND_ORA,
			COMMAND_PHA,
			COMMAND_PHP,
			COMMAND_PLA,
			COMMAND_PLP,
			COMMAND_ROL,
			COMMAND_ROR,
			COMMAND_RTI,
			COMMAND_RTS,
			COMMAND_SBC,
			COMMAND_SEC,
			COMMAND_SED,
			COMMAND_SEI,
			COMMAND_STA,
			COMMAND_STX,
			COMMAND_STY,
			COMMAND_TAX,
			COMMAND_TAY,
			COMMAND_TSX,
			COMMAND_TXA,
			COMMAND_TXS,
			COMMAND_TYA,
		};

		#define COMMAND_MAX COMMAND_TYA

		static const std::string COMMAND_STR[] = {
			"adc", "and", "asl", "bcc", "bcs", "beq", "bit", "bmi", "bne", "bpl",
			"brk", "bvc", "bvs", "clc", "cld", "cli", "clv", "cmp", "cpx", "cpy",
			"dec", "dex", "dey", "eor", "?ahx", "?alr", "?anc", "?arr", "?axs", "?dcp",
			"?isc", "?kil", "?las", "?lax", "?nop", "?rla", "?rra", "?sax", "?sbc", "?shx",
			"?shy", "?slo", "?sre", "?tas", "?xaa", "inc", "inx", "iny", "jmp", "jsr",
			"lda", "ldx", "ldy", "lsr", "nop", "ora", "pha", "php", "pla", "plp",
			"rol", "ror", "rti", "rts", "sbc", "sec", "sed", "sei", "sta", "stx",
			"sty", "tax", "tay", "tsx", "txa", "txs", "tya",
			};

		#define COMMAND_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::COMMAND_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::COMMAND_STR[_TYPE_]))

		static const std::map<std::string, int> COMMAND_MAP = {
			std::make_pair(COMMAND_STR[COMMAND_ADC], COMMAND_ADC), // COMMAND_ADC
			std::make_pair(COMMAND_STR[COMMAND_AND], COMMAND_AND), // COMMAND_AND
			std::make_pair(COMMAND_STR[COMMAND_ASL], COMMAND_ASL), // COMMAND_ASL
			std::make_pair(COMMAND_STR[COMMAND_BCC], COMMAND_BCC), // COMMAND_BCC
			std::make_pair(COMMAND_STR[COMMAND_BCS], COMMAND_BCS), // COMMAND_BCS
			std::make_pair(COMMAND_STR[COMMAND_BEQ], COMMAND_BEQ), // COMMAND_BEQ
			std::make_pair(COMMAND_STR[COMMAND_BIT], COMMAND_BIT), // COMMAND_BIT
			std::make_pair(COMMAND_STR[COMMAND_BMI], COMMAND_BMI), // COMMAND_BMI
			std::make_pair(COMMAND_STR[COMMAND_BNE], COMMAND_BNE), // COMMAND_BNE
			std::make_pair(COMMAND_STR[COMMAND_BPL], COMMAND_BPL), // COMMAND_BPL
			std::make_pair(COMMAND_STR[COMMAND_BRK], COMMAND_BRK), // COMMAND_BRK
			std::make_pair(COMMAND_STR[COMMAND_BVC], COMMAND_BVC), // COMMAND_BVC
			std::make_pair(COMMAND_STR[COMMAND_BVS], COMMAND_BVS), // COMMAND_BVS
			std::make_pair(COMMAND_STR[COMMAND_CLC], COMMAND_CLC), // COMMAND_CLC
			std::make_pair(COMMAND_STR[COMMAND_CLD], COMMAND_CLD), // COMMAND_CLD
			std::make_pair(COMMAND_STR[COMMAND_CLI], COMMAND_CLI), // COMMAND_CLI
			std::make_pair(COMMAND_STR[COMMAND_CLV], COMMAND_CLV), // COMMAND_CLV
			std::make_pair(COMMAND_STR[COMMAND_CMP], COMMAND_CMP), // COMMAND_CMP
			std::make_pair(COMMAND_STR[COMMAND_CPX], COMMAND_CPX), // COMMAND_CPX
			std::make_pair(COMMAND_STR[COMMAND_CPY], COMMAND_CPY), // COMMAND_CPY
			std::make_pair(COMMAND_STR[COMMAND_DEC], COMMAND_DEC), // COMMAND_DEC
			std::make_pair(COMMAND_STR[COMMAND_DEX], COMMAND_DEX), // COMMAND_DEX
			std::make_pair(COMMAND_STR[COMMAND_DEY], COMMAND_DEY), // COMMAND_DEY
			std::make_pair(COMMAND_STR[COMMAND_EOR], COMMAND_EOR), // COMMAND_EOR
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_AHX], COMMAND_ILLEGAL_AHX), // COMMAND_ILLEGAL_AHX
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_ALR], COMMAND_ILLEGAL_ALR), // COMMAND_ILLEGAL_ALR
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_ANC], COMMAND_ILLEGAL_ANC), // COMMAND_ILLEGAL_ANC
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_ARR], COMMAND_ILLEGAL_ARR), // COMMAND_ILLEGAL_ARR
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_AXS], COMMAND_ILLEGAL_AXS), // COMMAND_ILLEGAL_AXS
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_DCP], COMMAND_ILLEGAL_DCP), // COMMAND_ILLEGAL_DCP
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_ISC], COMMAND_ILLEGAL_ISC), // COMMAND_ILLEGAL_ISC
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_KIL], COMMAND_ILLEGAL_KIL), // COMMAND_ILLEGAL_KIL
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_LAS], COMMAND_ILLEGAL_LAS), // COMMAND_ILLEGAL_LAS
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_LAX], COMMAND_ILLEGAL_LAX), // COMMAND_ILLEGAL_LAX
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_NOP], COMMAND_ILLEGAL_NOP), // COMMAND_ILLEGAL_NOP
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_RLA], COMMAND_ILLEGAL_RLA), // COMMAND_ILLEGAL_RLA
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_RRA], COMMAND_ILLEGAL_RRA), // COMMAND_ILLEGAL_RRA
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_SAX], COMMAND_ILLEGAL_SAX), // COMMAND_ILLEGAL_SAX
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_SBC], COMMAND_ILLEGAL_SBC), // COMMAND_ILLEGAL_SBC
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_SHX], COMMAND_ILLEGAL_SHX), // COMMAND_ILLEGAL_SHX
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_SHY], COMMAND_ILLEGAL_SHY), // COMMAND_ILLEGAL_SHY
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_SLO], COMMAND_ILLEGAL_SLO), // COMMAND_ILLEGAL_SLO
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_SRE], COMMAND_ILLEGAL_SRE), // COMMAND_ILLEGAL_SRE
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_TAS], COMMAND_ILLEGAL_TAS), // COMMAND_ILLEGAL_TAS
			std::make_pair(COMMAND_STR[COMMAND_ILLEGAL_XAA], COMMAND_ILLEGAL_XAA), // COMMAND_ILLEGAL_XAA
			std::make_pair(COMMAND_STR[COMMAND_INC], COMMAND_INC), // COMMAND_INC
			std::make_pair(COMMAND_STR[COMMAND_INX], COMMAND_INX), // COMMAND_INX
			std::make_pair(COMMAND_STR[COMMAND_INY], COMMAND_INY), // COMMAND_INY
			std::make_pair(COMMAND_STR[COMMAND_JMP], COMMAND_JMP), // COMMAND_JMP
			std::make_pair(COMMAND_STR[COMMAND_JSR], COMMAND_JSR), // COMMAND_JSR
			std::make_pair(COMMAND_STR[COMMAND_LDA], COMMAND_LDA), // COMMAND_LDA
			std::make_pair(COMMAND_STR[COMMAND_LDX], COMMAND_LDX), // COMMAND_LDX
			std::make_pair(COMMAND_STR[COMMAND_LDY], COMMAND_LDY), // COMMAND_LDY
			std::make_pair(COMMAND_STR[COMMAND_LSR], COMMAND_LSR), // COMMAND_LSR
			std::make_pair(COMMAND_STR[COMMAND_NOP], COMMAND_NOP), // COMMAND_NOP
			std::make_pair(COMMAND_STR[COMMAND_ORA], COMMAND_ORA), // COMMAND_ORA
			std::make_pair(COMMAND_STR[COMMAND_PHA], COMMAND_PHA), // COMMAND_PHA
			std::make_pair(COMMAND_STR[COMMAND_PHP], COMMAND_PHP), // COMMAND_PHP
			std::make_pair(COMMAND_STR[COMMAND_PLA], COMMAND_PLA), // COMMAND_PLA
			std::make_pair(COMMAND_STR[COMMAND_PLP], COMMAND_PLP), // COMMAND_PLP
			std::make_pair(COMMAND_STR[COMMAND_ROL], COMMAND_ROL), // COMMAND_ROL
			std::make_pair(COMMAND_STR[COMMAND_ROR], COMMAND_ROR), // COMMAND_ROR
			std::make_pair(COMMAND_STR[COMMAND_RTI], COMMAND_RTI), // COMMAND_RTI
			std::make_pair(COMMAND_STR[COMMAND_RTS], COMMAND_RTS), // COMMAND_RTS
			std::make_pair(COMMAND_STR[COMMAND_SBC], COMMAND_SBC), // COMMAND_SBC
			std::make_pair(COMMAND_STR[COMMAND_SEC], COMMAND_SEC), // COMMAND_SEC
			std::make_pair(COMMAND_STR[COMMAND_SED], COMMAND_SED), // COMMAND_SED
			std::make_pair(COMMAND_STR[COMMAND_SEI], COMMAND_SEI), // COMMAND_SEI
			std::make_pair(COMMAND_STR[COMMAND_STA], COMMAND_STA), // COMMAND_STA
			std::make_pair(COMMAND_STR[COMMAND_STX], COMMAND_STX), // COMMAND_STX
			std::make_pair(COMMAND_STR[COMMAND_STY], COMMAND_STY), // COMMAND_STY
			std::make_pair(COMMAND_STR[COMMAND_TAX], COMMAND_TAX), // COMMAND_TAX
			std::make_pair(COMMAND_STR[COMMAND_TAY], COMMAND_TAY), // COMMAND_TAY
			std::make_pair(COMMAND_STR[COMMAND_TSX], COMMAND_TSX), // COMMAND_TSX
			std::make_pair(COMMAND_STR[COMMAND_TXA], COMMAND_TXA), // COMMAND_TXA
			std::make_pair(COMMAND_STR[COMMAND_TXS], COMMAND_TXS), // COMMAND_TXS
			std::make_pair(COMMAND_STR[COMMAND_TYA], COMMAND_TYA), // COMMAND_TYA
			};

		static const std::map<std::pair<uint8_t, uint8_t>, uint8_t> COMMAND_VALUE_MAP = {
			std::make_pair(std::make_pair(COMMAND_BRK, ADDRESS_MODE_IMPLIED), 0x00), // 0x00
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_INDIRECT_X), 0x01),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x02),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_INDIRECT_X), 0x03),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE), 0x04),
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_ZERO_PAGE), 0x05),
			std::make_pair(std::make_pair(COMMAND_ASL, ADDRESS_MODE_ZERO_PAGE), 0x06),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_ZERO_PAGE), 0x07),
			std::make_pair(std::make_pair(COMMAND_PHP, ADDRESS_MODE_IMPLIED), 0x08),
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_IMMEDIATE), 0x09),
			std::make_pair(std::make_pair(COMMAND_ASL, ADDRESS_MODE_ACCUMULATOR), 0x0a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ANC, ADDRESS_MODE_IMMEDIATE), 0x0b),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE), 0x0c),
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_ABSOLUTE), 0x0d),
			std::make_pair(std::make_pair(COMMAND_ASL, ADDRESS_MODE_ABSOLUTE), 0x0e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_ABSOLUTE), 0x0f),
			std::make_pair(std::make_pair(COMMAND_BPL, ADDRESS_MODE_RELATIVE), 0x10), // 0x10
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_INDIRECT_Y), 0x11),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x12),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_INDIRECT_Y), 0x13),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE_X), 0x14),
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_ZERO_PAGE_X), 0x15),
			std::make_pair(std::make_pair(COMMAND_ASL, ADDRESS_MODE_ZERO_PAGE_X), 0x16),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_ZERO_PAGE_X), 0x17),
			std::make_pair(std::make_pair(COMMAND_CLC, ADDRESS_MODE_IMPLIED), 0x18),
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_ABSOLUTE_Y), 0x19),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMPLIED), 0x1a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_ABSOLUTE_Y), 0x1b),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE_X), 0x1c),
			std::make_pair(std::make_pair(COMMAND_ORA, ADDRESS_MODE_ABSOLUTE_X), 0x1d),
			std::make_pair(std::make_pair(COMMAND_ASL, ADDRESS_MODE_ABSOLUTE_X), 0x1e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SLO, ADDRESS_MODE_ABSOLUTE_X), 0x1f),
			std::make_pair(std::make_pair(COMMAND_JSR, ADDRESS_MODE_ABSOLUTE), 0x20), // 0x20
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_INDIRECT_X), 0x21),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x22),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_INDIRECT_X), 0x23),
			std::make_pair(std::make_pair(COMMAND_BIT, ADDRESS_MODE_ZERO_PAGE), 0x24),
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_ZERO_PAGE), 0x25),
			std::make_pair(std::make_pair(COMMAND_ROL, ADDRESS_MODE_ZERO_PAGE), 0x26),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_ZERO_PAGE), 0x27),
			std::make_pair(std::make_pair(COMMAND_PLP, ADDRESS_MODE_IMPLIED), 0x28),
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_IMMEDIATE), 0x29),
			std::make_pair(std::make_pair(COMMAND_ROL, ADDRESS_MODE_ACCUMULATOR), 0x2a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ANC, ADDRESS_MODE_IMMEDIATE), 0x2b),
			std::make_pair(std::make_pair(COMMAND_BIT, ADDRESS_MODE_ABSOLUTE), 0x2c),
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_ABSOLUTE), 0x2d),
			std::make_pair(std::make_pair(COMMAND_ROL, ADDRESS_MODE_ABSOLUTE), 0x2e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_ABSOLUTE), 0x2f),
			std::make_pair(std::make_pair(COMMAND_BMI, ADDRESS_MODE_RELATIVE), 0x30), // 0x30
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_INDIRECT_Y), 0x31),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x32),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_INDIRECT_Y), 0x33),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE_X), 0x34),
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_ZERO_PAGE_X), 0x35),
			std::make_pair(std::make_pair(COMMAND_ROL, ADDRESS_MODE_ZERO_PAGE_X), 0x36),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_ZERO_PAGE_X), 0x37),
			std::make_pair(std::make_pair(COMMAND_SEC, ADDRESS_MODE_IMPLIED), 0x38),
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_ABSOLUTE_Y), 0x39),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMPLIED), 0x3a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_ABSOLUTE_Y), 0x3b),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE_X), 0x3c),
			std::make_pair(std::make_pair(COMMAND_AND, ADDRESS_MODE_ABSOLUTE_X), 0x3d),
			std::make_pair(std::make_pair(COMMAND_ROL, ADDRESS_MODE_ABSOLUTE_X), 0x3e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RLA, ADDRESS_MODE_ABSOLUTE_X), 0x3f),
			std::make_pair(std::make_pair(COMMAND_RTI, ADDRESS_MODE_IMPLIED), 0x40), // 0x40
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_INDIRECT_X), 0x41),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x42),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_INDIRECT_X), 0x43),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE), 0x44),
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_ZERO_PAGE), 0x45),
			std::make_pair(std::make_pair(COMMAND_LSR, ADDRESS_MODE_ZERO_PAGE), 0x46),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_ZERO_PAGE), 0x47),
			std::make_pair(std::make_pair(COMMAND_PHA, ADDRESS_MODE_IMPLIED), 0x48),
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_IMMEDIATE), 0x49),
			std::make_pair(std::make_pair(COMMAND_LSR, ADDRESS_MODE_ACCUMULATOR), 0x4a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ALR, ADDRESS_MODE_IMMEDIATE), 0x4b),
			std::make_pair(std::make_pair(COMMAND_JMP, ADDRESS_MODE_ABSOLUTE), 0x4c),
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_ABSOLUTE), 0x4d),
			std::make_pair(std::make_pair(COMMAND_LSR, ADDRESS_MODE_ABSOLUTE), 0x4e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_ABSOLUTE), 0x4f),
			std::make_pair(std::make_pair(COMMAND_BVC, ADDRESS_MODE_RELATIVE), 0x50), // 0x50
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_INDIRECT_Y), 0x51),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x52),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_INDIRECT_Y), 0x53),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE_X), 0x54),
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_ZERO_PAGE_X), 0x55),
			std::make_pair(std::make_pair(COMMAND_LSR, ADDRESS_MODE_ZERO_PAGE_X), 0x56),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_ZERO_PAGE_X), 0x57),
			std::make_pair(std::make_pair(COMMAND_CLI, ADDRESS_MODE_IMPLIED), 0x58),
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_ABSOLUTE_Y), 0x59),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMPLIED), 0x5a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_ABSOLUTE_Y), 0x5b),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE_X), 0x5c),
			std::make_pair(std::make_pair(COMMAND_EOR, ADDRESS_MODE_ABSOLUTE_X), 0x5d),
			std::make_pair(std::make_pair(COMMAND_LSR, ADDRESS_MODE_ABSOLUTE_X), 0x5e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SRE, ADDRESS_MODE_ABSOLUTE_X), 0x5f),
			std::make_pair(std::make_pair(COMMAND_RTS, ADDRESS_MODE_IMPLIED), 0x60), // 0x60
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_INDIRECT_X), 0x61),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x62),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_INDIRECT_X), 0x63),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE), 0x64),
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_ZERO_PAGE), 0x65),
			std::make_pair(std::make_pair(COMMAND_ROR, ADDRESS_MODE_ZERO_PAGE), 0x66),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_ZERO_PAGE), 0x67),
			std::make_pair(std::make_pair(COMMAND_PLA, ADDRESS_MODE_IMPLIED), 0x68),
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_IMMEDIATE), 0x69),
			std::make_pair(std::make_pair(COMMAND_ROR, ADDRESS_MODE_ACCUMULATOR), 0x6a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ARR, ADDRESS_MODE_IMMEDIATE), 0x6b),
			std::make_pair(std::make_pair(COMMAND_JMP, ADDRESS_MODE_INDIRECT), 0x6c),
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_ABSOLUTE), 0x6d),
			std::make_pair(std::make_pair(COMMAND_ROR, ADDRESS_MODE_ABSOLUTE), 0x6e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_ABSOLUTE), 0x6f),
			std::make_pair(std::make_pair(COMMAND_BVS, ADDRESS_MODE_RELATIVE), 0x70), // 0x70
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_INDIRECT_Y), 0x71),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x72),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_INDIRECT_Y), 0x73),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE_X), 0x74),
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_ZERO_PAGE_X), 0x75),
			std::make_pair(std::make_pair(COMMAND_ROR, ADDRESS_MODE_ZERO_PAGE_X), 0x76),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_ZERO_PAGE_X), 0x77),
			std::make_pair(std::make_pair(COMMAND_SEI, ADDRESS_MODE_IMPLIED), 0x78),
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_ABSOLUTE_Y), 0x79),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMPLIED), 0x7a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_ABSOLUTE_Y), 0x7b),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE_X), 0x7c),
			std::make_pair(std::make_pair(COMMAND_ADC, ADDRESS_MODE_ABSOLUTE_X), 0x7d),
			std::make_pair(std::make_pair(COMMAND_ROR, ADDRESS_MODE_ABSOLUTE_X), 0x7e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_RRA, ADDRESS_MODE_ABSOLUTE_X), 0x7f),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMMEDIATE), 0x80), // 0x80
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_INDIRECT_X), 0x81),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMMEDIATE), 0x82),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SAX, ADDRESS_MODE_INDIRECT_X), 0x83),
			std::make_pair(std::make_pair(COMMAND_STY, ADDRESS_MODE_ZERO_PAGE), 0x84),
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_ZERO_PAGE), 0x85),
			std::make_pair(std::make_pair(COMMAND_STX, ADDRESS_MODE_ZERO_PAGE), 0x86),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SAX, ADDRESS_MODE_ZERO_PAGE), 0x87),
			std::make_pair(std::make_pair(COMMAND_DEY, ADDRESS_MODE_IMPLIED), 0x88),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMMEDIATE), 0x89),
			std::make_pair(std::make_pair(COMMAND_TXA, ADDRESS_MODE_IMPLIED), 0x8a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_XAA, ADDRESS_MODE_IMMEDIATE), 0x8b),
			std::make_pair(std::make_pair(COMMAND_STY, ADDRESS_MODE_ABSOLUTE), 0x8c),
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_ABSOLUTE), 0x8d),
			std::make_pair(std::make_pair(COMMAND_STX, ADDRESS_MODE_ABSOLUTE), 0x8e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SAX, ADDRESS_MODE_ABSOLUTE), 0x8f),
			std::make_pair(std::make_pair(COMMAND_BCC, ADDRESS_MODE_RELATIVE), 0x90), // 0x90
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_INDIRECT_Y), 0x91),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0x92),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_AHX, ADDRESS_MODE_INDIRECT_Y), 0x93),
			std::make_pair(std::make_pair(COMMAND_STY, ADDRESS_MODE_ZERO_PAGE_X), 0x94),
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_ZERO_PAGE_X), 0x95),
			std::make_pair(std::make_pair(COMMAND_STX, ADDRESS_MODE_ZERO_PAGE_Y), 0x96),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SAX, ADDRESS_MODE_ZERO_PAGE_Y), 0x97),
			std::make_pair(std::make_pair(COMMAND_TYA, ADDRESS_MODE_IMPLIED), 0x98),
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_ABSOLUTE_Y), 0x99),
			std::make_pair(std::make_pair(COMMAND_TXS, ADDRESS_MODE_IMPLIED), 0x9a),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_TAS, ADDRESS_MODE_ABSOLUTE_Y), 0x9b),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SHY, ADDRESS_MODE_ABSOLUTE_X), 0x9c),
			std::make_pair(std::make_pair(COMMAND_STA, ADDRESS_MODE_ABSOLUTE_X), 0x9d),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SHX, ADDRESS_MODE_ABSOLUTE_Y), 0x9e),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_AHX, ADDRESS_MODE_ABSOLUTE_Y), 0x9f),
			std::make_pair(std::make_pair(COMMAND_LDY, ADDRESS_MODE_IMMEDIATE), 0xa0), // 0xa0
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_INDIRECT_X), 0xa1),
			std::make_pair(std::make_pair(COMMAND_LDX, ADDRESS_MODE_IMMEDIATE), 0xa2),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_INDIRECT_X), 0xa3),
			std::make_pair(std::make_pair(COMMAND_LDY, ADDRESS_MODE_ZERO_PAGE), 0xa4),
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_ZERO_PAGE), 0xa5),
			std::make_pair(std::make_pair(COMMAND_LDX, ADDRESS_MODE_ZERO_PAGE), 0xa6),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_ZERO_PAGE), 0xa7),
			std::make_pair(std::make_pair(COMMAND_TAY, ADDRESS_MODE_IMPLIED), 0xa8),
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_IMMEDIATE), 0xa9),
			std::make_pair(std::make_pair(COMMAND_TAX, ADDRESS_MODE_IMPLIED), 0xaa),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_IMMEDIATE), 0xab),
			std::make_pair(std::make_pair(COMMAND_LDY, ADDRESS_MODE_ABSOLUTE), 0xac),
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_ABSOLUTE), 0xad),
			std::make_pair(std::make_pair(COMMAND_LDX, ADDRESS_MODE_ABSOLUTE), 0xae),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_ABSOLUTE), 0xaf),
			std::make_pair(std::make_pair(COMMAND_BCS, ADDRESS_MODE_RELATIVE), 0xb0), // 0xb0
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_INDIRECT_Y), 0xb1),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0xb2),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_INDIRECT_Y), 0xb3),
			std::make_pair(std::make_pair(COMMAND_LDY, ADDRESS_MODE_ZERO_PAGE_X), 0xb4),
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_ZERO_PAGE_X), 0xb5),
			std::make_pair(std::make_pair(COMMAND_LDX, ADDRESS_MODE_ZERO_PAGE_Y), 0xb6),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_ZERO_PAGE_Y), 0xb7),
			std::make_pair(std::make_pair(COMMAND_CLV, ADDRESS_MODE_IMPLIED), 0xb8),
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_ABSOLUTE_Y), 0xb9),
			std::make_pair(std::make_pair(COMMAND_TSX, ADDRESS_MODE_IMPLIED), 0xba),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAS, ADDRESS_MODE_ABSOLUTE_Y), 0xbb),
			std::make_pair(std::make_pair(COMMAND_LDY, ADDRESS_MODE_ABSOLUTE_X), 0xbc),
			std::make_pair(std::make_pair(COMMAND_LDA, ADDRESS_MODE_ABSOLUTE_X), 0xbd),
			std::make_pair(std::make_pair(COMMAND_LDX, ADDRESS_MODE_ABSOLUTE_Y), 0xbe),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_LAX, ADDRESS_MODE_ABSOLUTE_Y), 0xbf),
			std::make_pair(std::make_pair(COMMAND_CPY, ADDRESS_MODE_IMMEDIATE), 0xc0), // 0xc0
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_INDIRECT_X), 0xc1),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMMEDIATE), 0xc2),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_INDIRECT_X), 0xc3),
			std::make_pair(std::make_pair(COMMAND_CPY, ADDRESS_MODE_ZERO_PAGE), 0xc4),
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_ZERO_PAGE), 0xc5),
			std::make_pair(std::make_pair(COMMAND_DEC, ADDRESS_MODE_ZERO_PAGE), 0xc6),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_ZERO_PAGE), 0xc7),
			std::make_pair(std::make_pair(COMMAND_INY, ADDRESS_MODE_IMPLIED), 0xc8),
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_IMMEDIATE), 0xc9),
			std::make_pair(std::make_pair(COMMAND_DEX, ADDRESS_MODE_IMPLIED), 0xca),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_AXS, ADDRESS_MODE_IMMEDIATE), 0xcb),
			std::make_pair(std::make_pair(COMMAND_CPY, ADDRESS_MODE_ABSOLUTE), 0xcc),
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_ABSOLUTE), 0xcd),
			std::make_pair(std::make_pair(COMMAND_DEC, ADDRESS_MODE_ABSOLUTE), 0xce),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_ABSOLUTE), 0xcf),
			std::make_pair(std::make_pair(COMMAND_BNE, ADDRESS_MODE_RELATIVE), 0xd0), // 0xd0
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_INDIRECT_Y), 0xd1),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0xd2),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_INDIRECT_Y), 0xd3),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE_X), 0xd4),
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_ZERO_PAGE_X), 0xd5),
			std::make_pair(std::make_pair(COMMAND_DEC, ADDRESS_MODE_ZERO_PAGE_X), 0xd6),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_ZERO_PAGE_X), 0xd7),
			std::make_pair(std::make_pair(COMMAND_CLD, ADDRESS_MODE_IMPLIED), 0xd8),
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_ABSOLUTE_Y), 0xd9),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMPLIED), 0xda),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_ABSOLUTE_Y), 0xdb),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE_X), 0xdc),
			std::make_pair(std::make_pair(COMMAND_CMP, ADDRESS_MODE_ABSOLUTE_X), 0xdd),
			std::make_pair(std::make_pair(COMMAND_DEC, ADDRESS_MODE_ABSOLUTE_X), 0xde),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_DCP, ADDRESS_MODE_ABSOLUTE_X), 0xdf),
			std::make_pair(std::make_pair(COMMAND_CPX, ADDRESS_MODE_IMMEDIATE), 0xe0), // 0xe0
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_INDIRECT_X), 0xe1),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMMEDIATE), 0xe2),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_INDIRECT_X), 0xe3),
			std::make_pair(std::make_pair(COMMAND_CPX, ADDRESS_MODE_ZERO_PAGE), 0xe4),
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_ZERO_PAGE), 0xe5),
			std::make_pair(std::make_pair(COMMAND_INC, ADDRESS_MODE_ZERO_PAGE), 0xe6),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_ZERO_PAGE), 0xe7),
			std::make_pair(std::make_pair(COMMAND_INX, ADDRESS_MODE_IMPLIED), 0xe8),
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_IMMEDIATE), 0xe9),
			std::make_pair(std::make_pair(COMMAND_NOP, ADDRESS_MODE_IMPLIED), 0xea),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_SBC, ADDRESS_MODE_IMMEDIATE), 0xeb),
			std::make_pair(std::make_pair(COMMAND_CPX, ADDRESS_MODE_ABSOLUTE), 0xec),
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_ABSOLUTE), 0xed),
			std::make_pair(std::make_pair(COMMAND_INC, ADDRESS_MODE_ABSOLUTE), 0xee),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_ABSOLUTE), 0xef),
			std::make_pair(std::make_pair(COMMAND_BEQ, ADDRESS_MODE_RELATIVE), 0xf0), // 0xf0
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_INDIRECT_Y), 0xf1),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_KIL, ADDRESS_MODE_IMPLIED), 0xf2),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_INDIRECT_Y), 0xf3),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ZERO_PAGE_X), 0xf4),
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_ZERO_PAGE_X), 0xf5),
			std::make_pair(std::make_pair(COMMAND_INC, ADDRESS_MODE_ZERO_PAGE_X), 0xf6),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_ZERO_PAGE_X), 0xf7),
			std::make_pair(std::make_pair(COMMAND_SED, ADDRESS_MODE_IMPLIED), 0xf8),
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_ABSOLUTE_Y), 0xf9),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_IMPLIED), 0xfa),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_ABSOLUTE_Y), 0xfb),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_NOP, ADDRESS_MODE_ABSOLUTE_X), 0xfc),
			std::make_pair(std::make_pair(COMMAND_SBC, ADDRESS_MODE_ABSOLUTE_X), 0xfd),
			std::make_pair(std::make_pair(COMMAND_INC, ADDRESS_MODE_ABSOLUTE_X), 0xfe),
			std::make_pair(std::make_pair(COMMAND_ILLEGAL_ISC, ADDRESS_MODE_ABSOLUTE_X), 0xff),
			};

		static const std::set<uint8_t> COMMAND_ABSOLUTE_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_ASL,
			COMMAND_BIT,
			COMMAND_CMP,
			COMMAND_CPX,
			COMMAND_CPY,
			COMMAND_DEC,
			COMMAND_EOR,
			COMMAND_JMP,
			COMMAND_JSR,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_NOP,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SAX,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_INC,
			COMMAND_LDA,
			COMMAND_LDX,
			COMMAND_LDY,
			COMMAND_LSR,
			COMMAND_ORA,
			COMMAND_ROL,
			COMMAND_ROR,
			COMMAND_SBC,
			COMMAND_STA,
			COMMAND_STX,
			COMMAND_STY,
			};

		static const std::set<uint8_t> COMMAND_ABSOLUTE_X_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_ASL,
			COMMAND_CMP,
			COMMAND_DEC,
			COMMAND_EOR,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_NOP,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SHY,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_INC,
			COMMAND_LDA,
			COMMAND_LDY,
			COMMAND_LSR,
			COMMAND_ORA,
			COMMAND_ROL,
			COMMAND_ROR,
			COMMAND_SBC,
			COMMAND_STA,
			};

		static const std::set<uint8_t> COMMAND_ABSOLUTE_Y_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_CMP,
			COMMAND_EOR,
			COMMAND_ILLEGAL_AHX,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_LAS,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_ILLEGAL_SHX,
			COMMAND_ILLEGAL_TAS,
			COMMAND_LDA,
			COMMAND_LDX,
			COMMAND_ORA,
			COMMAND_SBC,
			COMMAND_STA,
			};

		static const std::set<uint8_t> COMMAND_ACCUMULATOR_SET = {
			COMMAND_ASL,
			COMMAND_LSR,
			COMMAND_ROL,
			COMMAND_ROR,
			};

		static const std::set<uint8_t> COMMAND_IMMEDIATE_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_CMP,
			COMMAND_CPX,
			COMMAND_CPY,
			COMMAND_EOR,
			COMMAND_ILLEGAL_ALR,
			COMMAND_ILLEGAL_ANC,
			COMMAND_ILLEGAL_ARR,
			COMMAND_ILLEGAL_AXS,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_NOP,
			COMMAND_ILLEGAL_SBC,
			COMMAND_ILLEGAL_XAA,
			COMMAND_LDA,
			COMMAND_LDX,
			COMMAND_LDY,
			COMMAND_ORA,
			COMMAND_SBC,
			};

		static const std::set<uint8_t> COMMAND_IMPLIED_SET = {
			COMMAND_BRK,
			COMMAND_CLC,
			COMMAND_CLD,
			COMMAND_CLI,
			COMMAND_CLV,
			COMMAND_DEX,
			COMMAND_DEY,
			COMMAND_ILLEGAL_KIL,
			COMMAND_ILLEGAL_NOP,
			COMMAND_INX,
			COMMAND_INY,
			COMMAND_NOP,
			COMMAND_PHA,
			COMMAND_PHP,
			COMMAND_PLA,
			COMMAND_PLP,
			COMMAND_RTI,
			COMMAND_RTS,
			COMMAND_SEC,
			COMMAND_SED,
			COMMAND_SEI,
			COMMAND_TAX,
			COMMAND_TAY,
			COMMAND_TSX,
			COMMAND_TXA,
			COMMAND_TXS,
			COMMAND_TYA,
			};

		static const std::set<uint8_t> COMMAND_INDIRECT_SET = {
			COMMAND_JMP,
			};

		static const std::set<uint8_t> COMMAND_INDIRECT_X_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_CMP,
			COMMAND_EOR,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SAX,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_LDA,
			COMMAND_ORA,
			COMMAND_SBC,
			COMMAND_STA,
			};

		static const std::set<uint8_t> COMMAND_INDIRECT_Y_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_CMP,
			COMMAND_EOR,
			COMMAND_ILLEGAL_AHX,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_LDA,
			COMMAND_ORA,
			COMMAND_SBC,
			COMMAND_STA,
			};

		static const std::set<uint8_t> COMMAND_RELATIVE_SET = {
			COMMAND_BCC,
			COMMAND_BCS,
			COMMAND_BEQ,
			COMMAND_BMI,
			COMMAND_BNE,
			COMMAND_BPL,
			COMMAND_BVC,
			COMMAND_BVS,
			};

		static const std::set<uint8_t> COMMAND_ZERO_PAGE_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_ASL,
			COMMAND_BIT,
			COMMAND_CMP,
			COMMAND_CPX,
			COMMAND_CPY,
			COMMAND_DEC,
			COMMAND_EOR,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_NOP,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SAX,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_INC,
			COMMAND_LDA,
			COMMAND_LDX,
			COMMAND_LDY,
			COMMAND_LSR,
			COMMAND_ORA,
			COMMAND_ROL,
			COMMAND_ROR,
			COMMAND_SBC,
			COMMAND_STA,
			COMMAND_STX,
			COMMAND_STY,
			};

		static const std::set<uint8_t> COMMAND_ZERO_PAGE_X_SET = {
			COMMAND_ADC,
			COMMAND_AND,
			COMMAND_ASL,
			COMMAND_CMP,
			COMMAND_DEC,
			COMMAND_EOR,
			COMMAND_ILLEGAL_DCP,
			COMMAND_ILLEGAL_ISC,
			COMMAND_ILLEGAL_NOP,
			COMMAND_ILLEGAL_RLA,
			COMMAND_ILLEGAL_RRA,
			COMMAND_ILLEGAL_SLO,
			COMMAND_ILLEGAL_SRE,
			COMMAND_INC,
			COMMAND_LDA,
			COMMAND_LDY,
			COMMAND_LSR,
			COMMAND_ORA,
			COMMAND_ROL,
			COMMAND_ROR,
			COMMAND_SBC,
			COMMAND_STA,
			COMMAND_STY,
			};

		static const std::set<uint8_t> COMMAND_ZERO_PAGE_Y_SET = {
			COMMAND_ILLEGAL_LAX,
			COMMAND_ILLEGAL_SAX,
			COMMAND_LDX,
			COMMAND_STX,
			};

		enum {
			PRAGMA_COMMAND_DEFINE = 0,
			PRAGMA_COMMAND_INCLUDE,
			PRAGMA_COMMAND_ORIGIN,
			PRAGMA_COMMAND_PAGE_CHARACTER,
			PRAGMA_COMMAND_PAGE_PROGRAM,
			PAAGMA_COMMAND_PAGE_SIZE,
			PRAGMA_COMMAND_UNDEFINE,
			PRAGMA_CONDITION_ELSE,
			PRAGMA_CONDITION_END,
			PRAGMA_CONDITION_IF,
			PRAGMA_CONDITION_IF_DEFINE,
			PRAGMA_DATA_BYTE,
			PRAGMA_DATA_PART_HIGH,
			PRAGMA_DATA_PART_LOW,
			PRAGMA_DATA_RESERVE,
			PRAGMA_DATA_WORD,
			PRAGMA_INES_MAPPER,
			PRAGMA_INES_MIRRORING,
			PRAGMA_INES_ROM_CHARACTER,
			PRAGMA_INES_ROM_PROGRAM,
		};

		#define PRAGMA_MAX PRAGMA_INES_ROM_PROGRAM

		static const std::string PRAGMA_STR[] = {
			"def", "inc", "org", "chr", "prg", "size", "undef", "else", "end",
			"if", "ifdef", "db", "high", "low", "rs", "dw", "inesmap", "inesmir",
			"ineschr", "inesprg",
			};

		#define PRAGMA_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::PRAGMA_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::PRAGMA_STR[_TYPE_]))

		static const std::map<std::string, int> PRAGMA_MAP = {
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_DEFINE], PRAGMA_COMMAND_DEFINE), // PRAGMA_COMMAND_DEFINE
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_INCLUDE], PRAGMA_COMMAND_INCLUDE), // PRAGMA_COMMAND_INCLUDE
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_ORIGIN], PRAGMA_COMMAND_ORIGIN), // PRAGMA_COMMAND_ORIGIN
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_PAGE_CHARACTER], PRAGMA_COMMAND_PAGE_CHARACTER), // PRAGMA_COMMAND_PAGE_CHARACTER
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_PAGE_PROGRAM], PRAGMA_COMMAND_PAGE_PROGRAM), // PRAGMA_COMMAND_PAGE_PROGRAM
			std::make_pair(PRAGMA_STR[PAAGMA_COMMAND_PAGE_SIZE], PAAGMA_COMMAND_PAGE_SIZE), // PAAGMA_COMMAND_PAGE_SIZE
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_UNDEFINE], PRAGMA_COMMAND_UNDEFINE), // PRAGMA_COMMAND_UNDEFINE
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_ELSE], PRAGMA_CONDITION_ELSE), // PRAGMA_CONDITION_ELSE
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_END], PRAGMA_CONDITION_END), // PRAGMA_CONDITION_END
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_IF], PRAGMA_CONDITION_IF), // PRAGMA_CONDITION_IF
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_IF_DEFINE], PRAGMA_CONDITION_IF_DEFINE), // PRAGMA_CONDITION_IF_DEFINE
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_BYTE], PRAGMA_DATA_BYTE), // PRAGMA_DATA_BYTE
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_PART_HIGH], PRAGMA_DATA_PART_HIGH), // PRAGMA_DATA_PART_HIGH
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_PART_LOW], PRAGMA_DATA_PART_LOW), // PRAGMA_DATA_PART_LOW
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_RESERVE], PRAGMA_DATA_RESERVE), // PRAGMA_DATA_RESERVE
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_WORD], PRAGMA_DATA_WORD), // PRAGMA_DATA_WORD
			std::make_pair(PRAGMA_STR[PRAGMA_INES_MAPPER], PRAGMA_INES_MAPPER), // PRAGMA_INES_MAPPER
			std::make_pair(PRAGMA_STR[PRAGMA_INES_MIRRORING], PRAGMA_INES_MIRRORING), // PRAGMA_INES_MIRRORING
			std::make_pair(PRAGMA_STR[PRAGMA_INES_ROM_CHARACTER], PRAGMA_INES_ROM_CHARACTER), // PRAGMA_INES_ROM_CHARACTER
			std::make_pair(PRAGMA_STR[PRAGMA_INES_ROM_PROGRAM], PRAGMA_INES_ROM_PROGRAM), // PRAGMA_INES_ROM_PROGRAM
			};

		enum {
			REGISTER_ACCUMULATOR = 0,
			REGISTER_INDEX_X,
			REGISTER_INDEX_Y,
		};

		#define REGISTER_MAX REGISTER_INDEX_Y

		static const std::string REGISTER_STR[] = {
			"a", "x", "y"
			};

		#define REGISTER_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::REGISTER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::REGISTER_STR[_TYPE_]))

		static const std::map<std::string, int> REGISTER_MAP = {
			std::make_pair(REGISTER_STR[REGISTER_ACCUMULATOR], REGISTER_ACCUMULATOR), // REGISTER_ACCUMULATOR
			std::make_pair(REGISTER_STR[REGISTER_INDEX_X], REGISTER_INDEX_X), // REGISTER_INDEX_X
			std::make_pair(REGISTER_STR[REGISTER_INDEX_Y], REGISTER_INDEX_Y), // REGISTER_INDEX_Y
			};

		enum {
			SYMBOL_ARITHMETIC_ADD = 0,
			SYMBOL_ARITHMETIC_DIVIDE,
			SYMBOL_ARITHMETIC_MODULUS,
			SYMBOL_ARITHMETIC_MULTIPLY,
			SYMBOL_ARITHMETIC_SUBTRACT,
			SYMBOL_BINARY_AND,
			SYMBOL_BINARY_OR,
			SYMBOL_BINARY_XOR,
			SYMBOL_BLOCK_CLOSE,
			SYMBOL_BLOCK_OPEN,
			SYMBOL_BRACE_CLOSE,
			SYMBOL_BRACE_OPEN,
			SYMBOL_IMMEDIATE,
			SYMBOL_LOGICAL_SHIFT_LEFT,
			SYMBOL_LOGICAL_SHIFT_RIGHT,
			SYMBOL_OPERATOR_AND,
			SYMBOL_OPERATOR_EQUALS,
			SYMBOL_OPERATOR_GREATER_THAN,
			SYMBOL_OPERATOR_GREATER_THAN_EQUALS,
			SYMBOL_OPERATOR_LESS_THAN,
			SYMBOL_OPERATOR_LESS_THAN_EQUALS,
			SYMBOL_OPERATOR_NOT_EQUALS,
			SYMBOL_OPERATOR_OR,
			SYMBOL_PARENTHESIS_CLOSE,
			SYMBOL_PARENTHESIS_OPEN,
			SYMBOL_SEPERATOR,
			SYMBOL_UNARY_NEGATE,
			SYMBOL_UNARY_NOT,
		};

		#define SYMBOL_MAX SYMBOL_UNARY_NOT

		static const std::string SYMBOL_STR[] = {
			"+", "/", "%", "*", "-", "&", "|", "^", "}", "{", "]", "[",
			"@", "<<", ">>", "&&", "==", ">", ">=", "<", "<=", "!=",
			"||", ")", "(", ",", "~", "!",
			};

		#define SYMBOL_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::SYMBOL_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::SYMBOL_STR[_TYPE_]))

		static const std::map<std::string, int> SYMBOL_MAP = {
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_ADD], SYMBOL_ARITHMETIC_ADD), // SYMBOL_ARITHMETIC_ADD
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_DIVIDE], SYMBOL_ARITHMETIC_DIVIDE), // SYMBOL_ARITHMETIC_DIVIDE
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_MODULUS], SYMBOL_ARITHMETIC_MODULUS), // SYMBOL_ARITHMETIC_MODULUS
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_MULTIPLY], SYMBOL_ARITHMETIC_MULTIPLY), // SYMBOL_ARITHMETIC_MULTIPLY
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_SUBTRACT], SYMBOL_ARITHMETIC_SUBTRACT), // SYMBOL_ARITHMETIC_SUBTRACT
			std::make_pair(SYMBOL_STR[SYMBOL_BINARY_AND], SYMBOL_BINARY_AND), // SYMBOL_BINARY_AND
			std::make_pair(SYMBOL_STR[SYMBOL_BINARY_OR], SYMBOL_BINARY_OR), // SYMBOL_BINARY_OR
			std::make_pair(SYMBOL_STR[SYMBOL_BINARY_XOR], SYMBOL_BINARY_XOR), // SYMBOL_BINARY_XOR
			std::make_pair(SYMBOL_STR[SYMBOL_BLOCK_CLOSE], SYMBOL_BLOCK_CLOSE), // SYMBOL_BLOCK_CLOSE
			std::make_pair(SYMBOL_STR[SYMBOL_BLOCK_OPEN], SYMBOL_BLOCK_OPEN), // SYMBOL_BLOCK_OPEN
			std::make_pair(SYMBOL_STR[SYMBOL_BRACE_CLOSE], SYMBOL_BRACE_CLOSE), // SYMBOL_BRACE_CLOSE
			std::make_pair(SYMBOL_STR[SYMBOL_BRACE_OPEN], SYMBOL_BRACE_OPEN), // SYMBOL_BRACE_OPEN
			std::make_pair(SYMBOL_STR[SYMBOL_IMMEDIATE], SYMBOL_IMMEDIATE), // SYMBOL_IMMEDIATE
			std::make_pair(SYMBOL_STR[SYMBOL_LOGICAL_SHIFT_LEFT], SYMBOL_LOGICAL_SHIFT_LEFT), // SYMBOL_LOGICAL_SHIFT_LEFT
			std::make_pair(SYMBOL_STR[SYMBOL_LOGICAL_SHIFT_RIGHT], SYMBOL_LOGICAL_SHIFT_RIGHT), // SYMBOL_LOGICAL_SHIFT_RIGHT
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_AND], SYMBOL_OPERATOR_AND), // SYMBOL_OPERATOR_AND
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_EQUALS], SYMBOL_OPERATOR_EQUALS), // SYMBOL_OPERATOR_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_GREATER_THAN], SYMBOL_OPERATOR_GREATER_THAN), // SYMBOL_OPERATOR_GREATER_THAN
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_GREATER_THAN_EQUALS], SYMBOL_OPERATOR_GREATER_THAN_EQUALS),
				// SYMBOL_OPERATOR_GREATER_THAN_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_LESS_THAN], SYMBOL_OPERATOR_LESS_THAN), // SYMBOL_OPERATOR_LESS_THAN
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_LESS_THAN_EQUALS], SYMBOL_OPERATOR_LESS_THAN_EQUALS),
				// SYMBOL_OPERATOR_LESS_THAN_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_NOT_EQUALS], SYMBOL_OPERATOR_NOT_EQUALS), // SYMBOL_OPERATOR_NOT_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_OR], SYMBOL_OPERATOR_OR), // SYMBOL_OPERATOR_OR
			std::make_pair(SYMBOL_STR[SYMBOL_PARENTHESIS_CLOSE], SYMBOL_PARENTHESIS_CLOSE), // SYMBOL_PARENTHESIS_CLOSE
			std::make_pair(SYMBOL_STR[SYMBOL_PARENTHESIS_OPEN], SYMBOL_PARENTHESIS_OPEN), // SYMBOL_PARENTHESIS_OPEN
			std::make_pair(SYMBOL_STR[SYMBOL_SEPERATOR], SYMBOL_SEPERATOR), // SYMBOL_SEPERATOR
			std::make_pair(SYMBOL_STR[SYMBOL_UNARY_NOT], SYMBOL_UNARY_NOT), // SYMBOL_UNARY_NOT
			std::make_pair(SYMBOL_STR[SYMBOL_UNARY_NEGATE], SYMBOL_UNARY_NEGATE), // SYMBOL_UNARY_NEGATE
			};

		#define TOKEN_INVALID ((int) -1)

		enum {
			TOKEN_BEGIN = 0,
			TOKEN_END,
			TOKEN_BOOLEAN,
			TOKEN_COMMAND,
			TOKEN_IDENTIFIER,
			TOKEN_LABEL,
			TOKEN_LITERAL,
			TOKEN_PRAGMA,
			TOKEN_REGISTER,
			TOKEN_SCALAR,
			TOKEN_SYMBOL,
		};

		#define TOKEN_MAX TOKEN_SYMBOL

		static const std::string TOKEN_STR[] = {
			"Begin", "End", "Boolean", "Command", "Identifier", "Label", "Literal",
			"Pragma", "Register", "Scalar", "Symbol",
			};

		#define TOKEN_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::TOKEN_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::TOKEN_STR[_TYPE_]))

		class token :
				public nescc::core::unique_id {

			public:

				token(void);

				token(
					__in int type,
					__in_opt int subtype = TOKEN_INVALID,
					__in_opt size_t line = 0
					);

				token(
					__in const token &other
					);

				virtual ~token(void);

				token &operator=(
					__in const token &other
					);

				bool &as_boolean(void);

				std::string &as_literal(void);

				int32_t &as_scalar(void);

				virtual std::string as_string(
					__in_opt bool verbose = false
					) const;

				bool has_subtype(void) const;

				size_t line(void) const;

				void set(
					__in int type,
					__in_opt int subtype = TOKEN_INVALID,
					__in_opt size_t line = 0
					);

				int subtype(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				int type(void) const;

			protected:

				bool m_boolean;

				size_t m_line;

				std::string m_literal;

				int32_t m_scalar;

				int m_subtype;

				int m_type;
		};
	}
}

#endif // NESCC_CORE_TOKEN_H_
