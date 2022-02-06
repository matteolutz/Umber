#pragma once

#include "../Value.h"
#include "../Position.h"

#include <vector>

namespace umber
{

	namespace values
	{

		class ListValue : public Value
		{
		private:
			std::vector<std::shared_ptr<Value>> m_elements;

		public:
			ListValue(std::vector<std::shared_ptr<Value>> elements, Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			ListValue(std::vector<std::shared_ptr<Value>> elements);

			inline const std::vector<std::shared_ptr<Value>>& elements() const { return this->m_elements; }

		};

	}

}