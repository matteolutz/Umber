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
			std::vector<Node*> m_elements;

		public:
			ListNode(std::vector<Node*> elements, Position pos_start, Position pos_end);
			~ListNode();

		};

	}

}