#include "DictValue.h"

namespace umber
{

	namespace values
	{
		DictValue::DictValue(std::map<std::string, std::shared_ptr<Value>> elements, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(pos_start, pos_end, context), m_elements(elements)
		{
		}

		DictValue::DictValue(std::map<std::string, std::shared_ptr<Value>> elements)
			: Value(), m_elements(elements)
		{
		}

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> DictValue::access(Token accessor)
		{
			if (this->m_elements.find(accessor.value_or_zero()) == this->m_elements.end())
			{
				return { nullptr, std::make_unique<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, utils::std_string_format("Can't access '%s' (key not present)!", accessor.value_or_zero()).c_str(), this->m_context) };
			}

			return { this->m_elements[accessor.value_or_zero()], nullptr};
		}

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> DictValue::set(Token accessor, std::shared_ptr<Value> value)
		{
			if (!accessor.has_value())
			{
				return Value::set(accessor, value);
			}

			this->m_elements[accessor.value().value()] = value;
			return { this->m_elements[accessor.value().value()], nullptr};
		}

		std::string DictValue::as_string() const
		{
			std::string buffer = "{";
			for (const auto& [k, v] : this->m_elements)
			{
				buffer.append(utils::std_string_format("%s: %s, ", k.c_str(), v->as_string().c_str()));
			}
			buffer.push_back('}');
			return buffer;
		}

	}

}