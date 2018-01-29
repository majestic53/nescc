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

#ifndef NESCC_CONSOLE_CPU_TYPE_H_
#define NESCC_CONSOLE_CPU_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_CPU_HEADER "[NESCC::CONSOLE::CPU]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_CPU_EXCEPTION_HEADER NESCC_CONSOLE_CPU_HEADER " "
#else
		#define NESCC_CONSOLE_CPU_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED = 0,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_BRANCH,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_COMMAND,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_COMPARE,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_DECREMENT,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_FLAG,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_INCREMENT,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_LOAD,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_NOP,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STACK_PULL,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STACK_PUSH,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STORE,
			NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_TRANSFER,
		};

		#define NESCC_CONSOLE_CPU_EXCEPTION_MAX NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_TRANSFER

		static const std::string NESCC_CONSOLE_CPU_EXCEPTION_STR[] = {
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Cpu is uninitialized",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu branch command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu compare command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu decrement command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu flag command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu increment command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu load command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu address mode",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu nop command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu stack pull command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu stack push command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu store command",
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Unsupported cpu transfer command",
			};

		#define NESCC_CONSOLE_CPU_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_CPU_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_CPU_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_CPU_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define CPU_CYCLES_BRANCH 1
		#define CPU_CYCLES_PAGE_BOUNDARY 1
		#define CPU_CYCLES_READ_WRITE 2

		#define CPU_FLAG_CARRY 0x01
		#define CPU_FLAG_ZERO 0x02
		#define CPU_FLAG_INTERRUPT_DISABLE 0x04
		#define CPU_FLAG_DECIMAL 0x08
		#define CPU_FLAG_BREAK 0x10
		#define CPU_FLAG_UNUSED 0x20
		#define CPU_FLAG_OVERFLOW 0x40
		#define CPU_FLAG_SIGN 0x80

		#define CPU_FLAG_RESET (CPU_FLAG_UNUSED | CPU_FLAG_BREAK | CPU_FLAG_INTERRUPT_DISABLE)

		#define CPU_INTERRUPT_MASKABLE_ADDRESS 0xfffe
		#define CPU_INTERRUPT_NON_MASKABLE_ADDRESS 0xfffa
		#define CPU_STACK_POINTER_ADDRESS_OFFSET 3
		#define CPU_INTERRUPT_RESET_ADDRESS 0xfffc

		#define CPU_OAM_DMA_LENGTH 1

		#define CPU_STACK_POINTER_ADDRESS_BASE (UINT8_MAX + 1)
		#define CPU_STACK_POINTER_ADDRESS_RESET (UINT8_MAX - 2)
		#define CPU_STACK_POINTER_ADDRESS_MAX UINT8_MAX

		#define CPU_FLAG_MAX (CHAR_BIT - 1)

		static const std::string CPU_FLAG_STR[] = {
			"C", "Z", "I", "D", "B", "-", "V", "N",
			};

		#define CPU_FLAG_STRING(_TYPE_) \
			(((_TYPE_) > CPU_FLAG_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CPU_FLAG_STR[_TYPE_]))

		#define CPU_FLAG_VALUE(_FLAG_) (uint8_t) (1 << (_FLAG_))

		enum {
			CPU_MODE_ABSOLUTE = 0,
			CPU_MODE_ABSOLUTE_X,
			CPU_MODE_ABSOLUTE_Y,
			CPU_MODE_ACCUMULATOR,
			CPU_MODE_IMMEDIATE,
			CPU_MODE_IMPLIED,
			CPU_MODE_INDIRECT,
			CPU_MODE_INDIRECT_X,
			CPU_MODE_INDIRECT_Y,
			CPU_MODE_INTERRUPT,
			CPU_MODE_INTERRUPT_RETURN,
			CPU_MODE_RELATIVE,
			CPU_MODE_STACK_PULL,
			CPU_MODE_STACK_PUSH,
			CPU_MODE_ZERO_PAGE,
			CPU_MODE_ZERO_PAGE_X,
			CPU_MODE_ZERO_PAGE_Y,
		};

		#define CPU_MODE_MAX CPU_MODE_ZERO_PAGE_Y

		static const std::string CPU_MODE_STR[] = {
			"abs", "abs, x", "abs, y", "Acc", "#", "imp", "(abs)", "(zp, x)", "(zp), y",
			"int", "intr", "rel", "pull", "push", "zp", "zp, x", "zp, y",
			};

		#define CPU_MODE_STRING(_TYPE_) \
			(((_TYPE_) > CPU_MODE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CPU_MODE_STR[_TYPE_]))

		static const uint8_t CPU_MODE_CYC[] = {
			4, 4, 4, 2, 2, 2, 5, 6, 5, 7, 6, 2, 4, 3, 3, 4, 4,
			};

		#define CPU_MODE_CYCLES(_TYPE_) \
			(((_TYPE_) > CPU_MODE_MAX) ? CPU_MODE_CYC[CPU_MODE_IMPLIED] : \
				CPU_MODE_CYC[_TYPE_])

		static const uint8_t CPU_MODE_LEN[] = {
			3, 3, 3, 1, 2, 1, 3, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2,
			};

		#define CPU_MODE_LENGTH(_TYPE_) \
			(((_TYPE_) > CPU_MODE_MAX) ? CPU_MODE_LEN[CPU_MODE_IMPLIED] : \
				CPU_MODE_LEN[_TYPE_])

		enum {
			CPU_COMMAND_ADC = 0, CPU_COMMAND_AND, CPU_COMMAND_ASL, CPU_COMMAND_BCC,
			CPU_COMMAND_BCS, CPU_COMMAND_BEQ, CPU_COMMAND_BIT, CPU_COMMAND_BMI,
			CPU_COMMAND_BNE, CPU_COMMAND_BPL, CPU_COMMAND_BRK, CPU_COMMAND_BVC,
			CPU_COMMAND_BVS, CPU_COMMAND_CLC, CPU_COMMAND_CLD, CPU_COMMAND_CLI,
			CPU_COMMAND_CLV, CPU_COMMAND_CMP, CPU_COMMAND_CPX, CPU_COMMAND_CPY,
			CPU_COMMAND_DEC, CPU_COMMAND_DEX, CPU_COMMAND_DEY, CPU_COMMAND_EOR,
			CPU_COMMAND_INC, CPU_COMMAND_INX, CPU_COMMAND_INY, CPU_COMMAND_JMP,
			CPU_COMMAND_JSR, CPU_COMMAND_LDA, CPU_COMMAND_LDX, CPU_COMMAND_LDY,
			CPU_COMMAND_LSR, CPU_COMMAND_NOP, CPU_COMMAND_ORA, CPU_COMMAND_PHA,
			CPU_COMMAND_PHP, CPU_COMMAND_PLA, CPU_COMMAND_PLP, CPU_COMMAND_ROL,
			CPU_COMMAND_ROR, CPU_COMMAND_RTI, CPU_COMMAND_RTS, CPU_COMMAND_SBC,
			CPU_COMMAND_SEC, CPU_COMMAND_SED, CPU_COMMAND_SEI, CPU_COMMAND_STA,
			CPU_COMMAND_STX, CPU_COMMAND_STY, CPU_COMMAND_TAX, CPU_COMMAND_TAY,
			CPU_COMMAND_TSX, CPU_COMMAND_TXA, CPU_COMMAND_TXS, CPU_COMMAND_TYA,

			// illegal instructions
			CPU_COMMAND_ILLEGAL_AHX, CPU_COMMAND_ILLEGAL_ALR, CPU_COMMAND_ILLEGAL_ANC,
			CPU_COMMAND_ILLEGAL_ARR, CPU_COMMAND_ILLEGAL_AXS, CPU_COMMAND_ILLEGAL_DCP,
			CPU_COMMAND_ILLEGAL_ISC, CPU_COMMAND_ILLEGAL_KIL, CPU_COMMAND_ILLEGAL_LAS,
			CPU_COMMAND_ILLEGAL_LAX, CPU_COMMAND_ILLEGAL_NOP, CPU_COMMAND_ILLEGAL_RLA,
			CPU_COMMAND_ILLEGAL_RRA, CPU_COMMAND_ILLEGAL_SAX, CPU_COMMAND_ILLEGAL_SBC,
			CPU_COMMAND_ILLEGAL_SHX, CPU_COMMAND_ILLEGAL_SHY, CPU_COMMAND_ILLEGAL_SLO,
			CPU_COMMAND_ILLEGAL_SRE, CPU_COMMAND_ILLEGAL_TAS, CPU_COMMAND_ILLEGAL_XAA,
		};

		#define CPU_COMMAND_MAX CPU_COMMAND_ILLEGAL_XAA

		static const std::string CPU_COMMAND_STR[] = {
			"adc", "and", "asl", "bcc", "bcs", "beq", "bit", "bmi", "bne", "bpl",
			"brk", "bvc", "bvs", "clc", "cld", "cli", "clv", "cmp", "cpx", "cpy",
			"dec", "dex", "dey", "eor", "inc", "inx", "iny", "jmp", "jsr", "lda",
			"ldx", "ldy", "lsr", "nop", "ora", "pha", "php", "pla", "plp", "rol",
			"ror", "rti", "rts", "sbc", "sec", "sed", "sei", "sta", "stx", "sty",
			"tax", "tay", "tsx", "txa", "txs", "tya",

			// illegal instructions
			"*ahx", "*alr", "*anc", "*arr", "*axs", "*dcp", "*isc", "*kil", "*las",
			"*lax", "*nop", "*rla", "*rra", "*sax", "*sbc", "*shx", "*shy", "*slo",
			"*sre", "*tas", "*xaa",
			};

		#define CPU_COMMAND_STRING(_TYPE_) \
			(((_TYPE_) > CPU_COMMAND_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CPU_COMMAND_STR[_TYPE_]))

		static const std::vector<std::pair<uint8_t, uint8_t>> CPU_COMMAND_MAP = {
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BRK, CPU_MODE_INTERRUPT), // 0x00
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ASL, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_PHP, CPU_MODE_STACK_PUSH),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ASL, CPU_MODE_ACCUMULATOR),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ANC, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ASL, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BPL, CPU_MODE_RELATIVE), // 0x10
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ASL, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CLC, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ORA, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ASL, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SLO, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_JSR, CPU_MODE_ABSOLUTE), // 0x20
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BIT, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROL, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_PLP, CPU_MODE_STACK_PULL),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROL, CPU_MODE_ACCUMULATOR),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ANC, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BIT, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROL, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BMI, CPU_MODE_RELATIVE), // 0x30
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROL, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SEC, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_AND, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROL, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RLA, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_RTI, CPU_MODE_INTERRUPT_RETURN), // 0x40
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LSR, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_PHA, CPU_MODE_STACK_PUSH),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LSR, CPU_MODE_ACCUMULATOR),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ALR, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_JMP, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LSR, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BVC, CPU_MODE_RELATIVE), // 0x50
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LSR, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CLI, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_EOR, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LSR, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SRE, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_RTS, CPU_MODE_INTERRUPT_RETURN), // 0x60
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROR, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_PLA, CPU_MODE_STACK_PULL),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROR, CPU_MODE_ACCUMULATOR),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ARR, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_JMP, CPU_MODE_INDIRECT),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROR, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BVS, CPU_MODE_RELATIVE), // 0x70
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROR, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SEI, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ADC, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ROR, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_RRA, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMMEDIATE), // 0x80
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SAX, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STY, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STX, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SAX, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_DEY, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_TXA, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_XAA, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STY, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STX, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SAX, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BCC, CPU_MODE_RELATIVE), // 0x90
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_AHX, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STY, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STX, CPU_MODE_ZERO_PAGE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SAX, CPU_MODE_ZERO_PAGE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_TYA, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_TXS, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_TAS, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SHY, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_STA, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SHX, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_AHX, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDY, CPU_MODE_IMMEDIATE), // 0xa0
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDX, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDY, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDX, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_TAY, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_TAX, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDY, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDX, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BCS, CPU_MODE_RELATIVE), // 0xb0
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDY, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDX, CPU_MODE_ZERO_PAGE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_ZERO_PAGE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CLV, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_TSX, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAS, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDY, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDA, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_LDX, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_LAX, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CPY, CPU_MODE_IMMEDIATE), // 0xc0
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CPY, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_DEC, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_INY, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_DEX, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_AXS, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CPY, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_DEC, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BNE, CPU_MODE_RELATIVE), // 0xd0
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_DEC, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CLD, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CMP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_DEC, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_DCP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CPX, CPU_MODE_IMMEDIATE), // 0xe0
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_INDIRECT_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CPX, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_INC, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_ZERO_PAGE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_INX, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_SBC, CPU_MODE_IMMEDIATE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_CPX, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_INC, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_ABSOLUTE),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_BEQ, CPU_MODE_RELATIVE), // 0xf0
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_KIL, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_INDIRECT_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_INC, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_ZERO_PAGE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SED, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_IMPLIED),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_ABSOLUTE_Y),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_NOP, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_SBC, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_INC, CPU_MODE_ABSOLUTE_X),
			std::pair<uint8_t, uint8_t>(CPU_COMMAND_ILLEGAL_ISC, CPU_MODE_ABSOLUTE_X),
			};
	}
}

#endif // NESCC_CONSOLE_CPU_TYPE_H_
