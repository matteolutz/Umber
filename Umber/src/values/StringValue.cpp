#include "StringValue.h"

namespace umber
{

	namespace values
	{
		StringValue::StringValue(std::string value, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(ValueType::Dict, pos_start, pos_end, context), m_value(value)
		{
		}

		StringValue::StringValue(std::string value)
			: Value(ValueType::Dict), m_value(value)
		{
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> StringValue::added_to(std::shared_ptr<Value> other)
		{
			return { std::make_unique<StringValue>(this->m_value + other->as_string()), nullptr };
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> StringValue::comparison_eq(std::shared_ptr<Value> other)
		{
			std::shared_ptr<StringValue> other_string = std::dynamic_pointer_cast<StringValue>(other);
			if (other_string == nullptr)
			{
				return Value::comparison_eq(other);
			}

			return { std::make_unique<values::NumberValue>(this->m_value == other_string->m_value ? values::NumberValue::TRUE_VALUE : values::NumberValue::FALSE_VALUE), nullptr };
		}

		std::unique_ptr<Value> StringValue::copy() const
		{
			return std::make_unique<StringValue>(*this);
		}

	}

}