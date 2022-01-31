#include "ParseResult.h"

namespace umber
{

	namespace result
	{

		ParseResult::ParseResult() {}

		void ParseResult::register_advancement()
		{
			this->m_last_registered_advance_count = 1;
			this->m_advance_count++;
		}

		std::optional<Node> ParseResult::register_res(ParseResult& res)
		{
			this->m_last_registered_advance_count = res.m_last_registered_advance_count;
			this->m_advance_count += res.m_advance_count;
			if (res.has_error())
			{
				this->m_error.emplace(res.m_error.value());
			}
			return res.m_node.value();
		}

		std::optional<Node> ParseResult::try_register_res(ParseResult& res)
		{
			if (res.has_error())
			{
				this->m_to_reverse_count = res.m_advance_count;
				return std::nullopt;
			}
			return this->register_res(res);
		}

		void ParseResult::success(Node node)
		{
			this->m_node.emplace(node);
		}

		ParseResult ParseResult::copy_success(Node node) const
		{
			ParseResult res = *this;
			res.success(node);
			return res;
		}

		void ParseResult::failure(Error error)
		{
			if (!this->m_error.has_value() || this->m_advance_count == 0)
			{
				this->m_error.emplace(error);
			}
		}

		ParseResult ParseResult::copy_error(Error error)
		{
			ParseResult res = *this;
			res.failure(error);
			return res;
		}

		bool ParseResult::has_error() const
		{
			return this->m_error.has_value();
		}

		bool ParseResult::has_node() const
		{
			return this->m_node.has_value();
		}

	}

}