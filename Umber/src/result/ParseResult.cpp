#include "ParseResult.h"

namespace umber
{

	namespace result
	{
		void ParseResult::cleanup()
		{
			delete m_error, m_node;
		}

		void ParseResult::register_advancement()
		{
			this->m_last_registered_advance_count = 1;
			this->m_advance_count++;
		}

		Node* ParseResult::register_res(ParseResult res)
		{
			this->m_last_registered_advance_count = res.m_last_registered_advance_count;
			this->m_advance_count += res.m_advance_count;
			if (res.has_error())
			{

				this->m_error = res.m_error;

				return nullptr;
			}
			if (!res.has_node())
			{
				return nullptr;
			}
			return res.m_node;
		}

		Node* ParseResult::try_register_res(ParseResult res)
		{
			if (res.has_error())
			{
				this->m_to_reverse_count = res.m_advance_count;
				return nullptr;
			}
			return this->register_res(res);
		}

		void ParseResult::success(Node* node)
		{
			this->m_node = node;
		}

		void ParseResult::failure(Error* error)
		{
			if (this->m_error == nullptr || this->m_advance_count == 0)
			{
				this->m_error = error;
			}
		}

		bool ParseResult::has_error() const
		{
			return this->m_error != nullptr;
		}

		bool ParseResult::has_node() const
		{
			return this->m_node != nullptr;
		}

	}

}