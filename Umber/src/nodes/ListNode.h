#pragma once

#include "../Node.h"
#include "../Position.h"
#include "../Token.h"

#include <vector>

namespace umber
{

	namespace nodes
	{

		class ListNode : public Node
		{
		private:
			std::vector<Token> m_elements;

		public:
			ListNode(std::vector<Token> elements, Position pos_start, Position pos_end);

		};

	}

}