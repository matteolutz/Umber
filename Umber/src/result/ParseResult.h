#pragma once

#include "../Node.h"
#include "../Error.h"

#include <optional>

namespace umber
{

	namespace result
	{

		class ParseResult
		{
		private:
			std::optional<Error> m_error;
			std::optional<Node> m_node;

			unsigned int m_last_registered_advance_count, m_advance_count, m_to_reverse_count;

		public:
			void register_advancement();

			std::optional<Node> register_res(ParseResult res);
			std::optional<Node> try_register_res(ParseResult res);

			void success(Node node);
			ParseResult copy_success(Node node) const;

			void failure(Error error);
			ParseResult copy_error(Error error);

			bool has_error() const;
			bool has_node() const;

			inline const std::optional<Error>& error() const { return this->m_error; }
			inline const std::optional<Node>& node() const { return this->m_node; }
			inline const unsigned int last_registered_advance_count() const { return this->m_last_registered_advance_count; }
			inline const unsigned int advance_count() const { return this->m_advance_count; }
			inline const unsigned int to_reverse_count() const { return this->m_to_reverse_count; }

		};

	}

}