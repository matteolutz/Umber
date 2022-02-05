#include "Value.h"

namespace umber
{



	Value::Value(Position pos_start, Position pos_end, Context* context)
		: m_pos_start(pos_start), m_pos_end(pos_end), m_context(context)
	{
	}

	errors::RuntimeError* Value::illegal_operation(Value* other) const
	{
		return new errors::RuntimeError{
			this->m_pos_start, other != nullptr ? other->m_pos_end : this->m_pos_end, "Illegal operation!", this->m_context
		};
	}

}