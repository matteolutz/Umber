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

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> ListValue::access(std::string accessor)
		{
			/*if (std::find(accessor.begin(), accessor.end(), [](const char& c) { return std::isdigit(c); }) == accessor.end())
			{
				return { nullptr, Value::illegal_operation() };
			}*/

			if (accessor == "l") return { std::make_shared<values::NumberValue>(this->m_elements.size()), nullptr };

			int index = std::stoi(accessor);
			if (index < 0 || index >= this->m_elements.size())
			{
				return Value::access(accessor);
			}

			return { this->m_elements[index]->copy(), nullptr};
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