#include "Value.h"

namespace umber
{


	Value::Value(ValueType type, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
		: m_type(type), m_pos_start(pos_start), m_pos_end(pos_end), m_context(context)
	{
	}

	Value::Value(ValueType type)
		: m_type(type)
	{
	}

	Value::~Value() {}

	std::unique_ptr<errors::RuntimeError> Value::illegal_operation(std::shared_ptr<Value> other) const
	{
		return std::make_unique<errors::RuntimeError>(this->m_pos_start, other != nullptr ? other->m_pos_end : this->m_pos_end, "Illegal operation!", this->m_context);
	}

}