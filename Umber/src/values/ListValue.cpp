#include "ListValue.h"

namespace umber
{

	namespace values
	{
		ListValue::ListValue(std::vector<std::shared_ptr<Value>> elements, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(pos_start, pos_end, context), m_elements(elements)
		{
		}

		ListValue::ListValue(std::vector<std::shared_ptr<Value>> elements)
			: Value(), m_elements(elements)
		{
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> ListValue::added_to(std::shared_ptr<Value> other)
		{
			std::unique_ptr<ListValue> new_list = std::make_unique<ListValue>(*this);
			new_list->m_elements.emplace_back(other);

			return { std::move(new_list), nullptr };
		}

		std::string ListValue::as_string() const
		{
			std::string buffer = "[";
			for (const std::shared_ptr<Value>& v : this->m_elements)
			{
				buffer.append(utils::std_string_format("%s, ", v->as_string().c_str()));
			}
			buffer.push_back(']');
			return buffer;
		}

	}

}