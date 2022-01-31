#include "ListNode.h"

namespace umber
{

	namespace nodes
	{

		ListNode::ListNode(std::vector<Token> elements, Position pos_start, Position pos_end)
			: Node(pos_start, pos_end), m_elements(elements) { }

	}

}