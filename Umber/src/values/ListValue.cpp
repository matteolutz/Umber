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

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> ListValue::access(Token accessor)
		{
			if (accessor.type() != TokenType::Int)
			{
				return { nullptr, std::make_unique<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, "List accessor needs to be an int!", this->m_context) };
			}

			int index = std::stoi(accessor.value_or_zero());
			if (index < 0 || index >= this->m_elements.size())
			{
				return { nullptr, std::make_unique<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, utils::std_string_format("Index %d is out of range!", index).c_str(), this->m_context)};
			}

			return { this->m_elements[index], nullptr};
		}

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> ListValue::set(Token accessor, std::shared_ptr<Value> value)
		{
			if (accessor.type() != TokenType::Int)
			{
				return { nullptr, Value::illegal_operation() };
			}
			
			int index = std::stoi(accessor.value_or_zero());
			if (index < 0 || index >= this->m_elements.size())
			{
				return Value::access(accessor);
			}

			this->m_elements[index] = value;

			return { this->m_elements[index], nullptr };
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