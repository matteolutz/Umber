#pragma once

#include "../Node.h"
#include "../Position.h"

#include <vector>

namespace umber
{

	namespace nodes
	{

		class ListNode : public Node
		{
		private:
			std::vector<std::shared_ptr<Node>> m_elements;

		public:
			ListNode(std::vector<std::shared_ptr<Node>> elements, Position pos_start, Position pos_end);

			inline const std::vector<std::shared_ptr<Node>>& elements() const { return this->m_elements; }

		};

	}

}