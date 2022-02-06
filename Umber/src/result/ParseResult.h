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
			std::shared_ptr<Error> m_error;
			std::shared_ptr<Node> m_node;

			unsigned int m_last_registered_advance_count, m_advance_count, m_to_reverse_count;

		public:
			void register_advancement();

			std::shared_ptr<Node> register_res(ParseResult res);
			std::shared_ptr<Node> try_register_res(ParseResult res);

			void success(std::shared_ptr<Node> node);

			void failure(std::shared_ptr<Error> error);

			bool has_error() const;
			bool has_node() const;

			inline const std::shared_ptr<Error>& error() const { return this->m_error; }
			inline const std::shared_ptr<Node>& node() const { return this->m_node; }
			inline const unsigned int last_registered_advance_count() const { return this->m_last_registered_advance_count; }
			inline const unsigned int advance_count() const { return this->m_advance_count; }
			inline const unsigned int to_reverse_count() const { return this->m_to_reverse_count; }

		};

	}

}