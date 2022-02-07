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

		std::unique_ptr<Value> ListValue::copy()
		{
			return std::make_unique<ListValue>(*this);
		}

		std::string ListValue::as_string() const
		{
			std::string buffer = "[";
			for (const std::shared_ptr<Value>& v : this->m_elements)
			{
				buffer.append(v->as_string());
			}
			buffer.push_back(']');
			return buffer;
		}

	}

}