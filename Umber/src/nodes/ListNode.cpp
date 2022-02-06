#include "ListNode.h"

namespace umber
{

	namespace nodes
	{

		ListNode::ListNode(std::vector<std::shared_ptr<Node>> elements, Position pos_start, Position pos_end)
			: Node(pos_start, pos_end, NodeType::List), m_elements(elements) { }
	}

}