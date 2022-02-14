#include "DictValue.h"

namespace umber
{

	namespace values
	{
		DictValue::DictValue(std::map<std::string, std::shared_ptr<Value>> elements, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(ValueType::Dict, pos_start, pos_end, context), m_elements(elements)
		{
		}

		DictValue::DictValue(std::map<std::string, std::shared_ptr<Value>> elements)
			: Value(ValueType::Dict), m_elements(elements)
		{
		}

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> DictValue::access(std::shared_ptr<Value> accessor)
		{
			if (accessor->type() != ValueType::String)
			{
				return { nullptr, std::make_unique<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, "Dict accessor needs to be a string!", this->m_context) };
			}

			std::string key = std::static_pointer_cast<values::StringValue>(accessor)->value();

			if (this->m_elements.find(key) == this->m_elements.end())
			{
				return { nullptr, std::make_unique<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, utils::std_string_format("Can't access '%s' (key not present)!", key).c_str(), this->m_context) };
			}

			return { this->m_elements[key], nullptr};
		}

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> DictValue::set(std::shared_ptr<Value> accessor, std::shared_ptr<Value> value)
		{
			if (accessor->type() != ValueType::String)
			{
				return { nullptr, std::make_unique<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, "Dict accessor needs to be a string!", this->m_context) };
			}

			std::string key = std::static_pointer_cast<values::StringValue>(accessor)->value();

			this->m_elements[key] = value;
			return { this->m_elements[key], nullptr};
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