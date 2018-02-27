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

#ifndef NESCC_CORE_NODE_H_
#define NESCC_CORE_NODE_H_

#include <vector>
#include "./unique_id.h"

namespace nescc {

	namespace core {

		#define NODE_INVALID ((int) -1)

		enum {
			NODE_BEGIN = 0,
			NODE_BLOCK,
			NODE_COMMAND,
			NODE_CONDITION,
			NODE_END,
			NODE_EXPRESSION,
			NODE_LABEL,
			NODE_LEAF,
			NODE_MODIFIER,
			NODE_OPERATOR,
			NODE_PRAGMA,
		};

		#define NODE_MAX NODE_PRAGMA

		static const std::string NODE_STR[] = {
			"Begin", "Block", "Command", "Condition", "End", "Expression", "Label",
			"Leaf", "Modifier", "Operator", "Pragma",
			};

		#define NODE_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::NODE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::NODE_STR[_TYPE_]))

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
			ADDRESS_MODE_ZERO_PAGE, // <expression>
			ADDRESS_MODE_ZERO_PAGE_X, // <expression> , x
			ADDRESS_MODE_ZERO_PAGE_Y, // <expression> , y
		};

		#define ADDRESS_MODE_MAX ADDRESS_MODE_ZERO_PAGE_Y

		static const std::string ADDRESS_MODE_STR[] = {
			"Absolute", "Absolute-X", "Absolute-Y", "Accumulator", "Immediate", "Implied", "Indirect",
			"Indirect-X", "Indirect-Y", "Zero-Page", "Zero-Page-X", "Zero-Page-Y",
			};

		#define ADDRESS_MODE_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::ADDRESS_MODE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::ADDRESS_MODE_STR[_TYPE_]))

		class node :
				public nescc::core::unique_id {

			public:

				node(void);

				node(
					__in int type,
					__in_opt int subtype = NODE_INVALID
					);

				node(
					__in const node &other
					);

				virtual ~node(void);

				node &operator=(
					__in const node &other
					);

				virtual std::string as_string(
					__in_opt bool verbose = false
					) const;

				std::vector<nescc::core::uuid_t> &children(void);

				bool has_subtype(void) const;

				bool has_token(void) const;

				void set(
					__in int type,
					__in_opt int subtype = NODE_INVALID
					);

				int subtype(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				nescc::core::uuid_t &token(void);

				int type(void) const;

			protected:

				std::vector<nescc::core::uuid_t> m_children;

				int m_subtype;

				nescc::core::uuid_t m_token;

				int m_type;
		};
	}
}

#endif // NESCC_CORE_NODE_H_
