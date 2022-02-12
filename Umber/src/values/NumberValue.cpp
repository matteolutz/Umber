#include "NumberValue.h"

namespace umber
{

	namespace values
	{
		NumberValue::NumberValue(float value, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(pos_start, pos_end, context), m_value(value)
		{
		}

		NumberValue::NumberValue(float value)
			: Value(), m_value(value)
		{ }

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::added_to(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value + other_number->m_value), nullptr };
			}
			return Value::added_to(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::subbed_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value - other_number->m_value), nullptr };
			}
			return Value::subbed_by(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::multed_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value * other_number->m_value), nullptr };
			}
			return Value::multed_by(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::dived_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				if (other_number->m_value == 0.0f)
				{
					return { nullptr, std::make_unique<errors::RuntimeError>(other_number->m_pos_start, other_number->m_pos_end, "Divison by 0!", this->m_context) };
				}

				return { std::make_unique<NumberValue>(this->m_value / other_number->m_value), nullptr };
			}
			return Value::dived_by(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::powed_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>((float)pow((double)this->m_value, (double)other_number->m_value)), nullptr };
			}
			return Value::powed_by(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::moduloed_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(std::fmod(this->m_value, other_number->m_value)), nullptr };
			}
			return Value::moduloed_by(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::comparison_eq(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value == other_number->m_value ? TRUE_VALUE : FALSE_VALUE), nullptr };
			}
			return Value::comparison_eq(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::comparison_gt(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value > other_number->m_value ? TRUE_VALUE : FALSE_VALUE), nullptr };
			}
			return Value::comparison_gt(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::comparison_lt(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value < other_number->m_value ? TRUE_VALUE : FALSE_VALUE), nullptr };
			}
			return Value::comparison_lt(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::comparison_gte(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value >= other_number->m_value ? TRUE_VALUE : FALSE_VALUE), nullptr };
			}
			return Value::comparison_gte(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::comparison_lte(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number != nullptr)
			{
				return { std::make_unique<NumberValue>(this->m_value <= other_number->m_value ? TRUE_VALUE : FALSE_VALUE), nullptr };
			}
			return Value::comparison_lte(other);
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::bit_anded_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number == nullptr || std::fmod(this->m_value, 1) != 0 || std::fmod(other_number->m_value, 1) != 0)
			{
				return Value::bit_anded_by(other);
			}
			return { std::make_unique<NumberValue>((float)((int)this->m_value & (int)other_number->m_value)), nullptr };
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> NumberValue::bit_ored_by(std::shared_ptr<Value> other)
		{
			std::shared_ptr<NumberValue> other_number = std::dynamic_pointer_cast<NumberValue>(other);
			if (other_number == nullptr || std::fmod(this->m_value, 1) != 0 || std::fmod(other_number->m_value, 1) != 0)
			{
				return Value::bit_anded_by(other);
			}
			return { std::make_unique<NumberValue>((float)((int)this->m_value | (int)other_number->m_value)), nullptr };
		}

	}

}