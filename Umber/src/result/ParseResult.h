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
			Error* m_error;
			Node* m_node;

			unsigned int m_last_registered_advance_count, m_advance_count, m_to_reverse_count;

		public:
			void cleanup();

			void register_advancement();

			Node* register_res(ParseResult res);
			Node* try_register_res(ParseResult res);

			void success(Node* node);

			void failure(Error* error);

			bool has_error() const;
			bool has_node() const;

			inline const Error* error() const { return this->m_error; }
			inline const Node* node() const { return this->m_node; }
			inline const unsigned int last_registered_advance_count() const { return this->m_last_registered_advance_count; }
			inline const unsigned int advance_count() const { return this->m_advance_count; }
			inline const unsigned int to_reverse_count() const { return this->m_to_reverse_count; }

		};

	}

}