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

		std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> DictValue::access(std::string accessor)
		{
			if (this->m_elements.find(accessor) == this->m_elements.end())
			{
				return Value::access(accessor);
			}

			return { this->m_elements[accessor]->copy(), nullptr};
		}

		std::string DictValue::as_string() const
		{
			std::string buffer = "{\n";
			for (const auto& [k, v] : this->m_elements)
			{
				buffer.append(utils::std_string_format("\t%s: %s\n", k.c_str(), v->as_string().c_str()));
			}
			buffer.push_back('}');
			return buffer;
		}

	}

}