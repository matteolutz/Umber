#include "ListNode.h"

namespace umber
{

	namespace nodes
	{

		ListNode::ListNode(std::vector<Node*> elements, Position pos_start, Position pos_end)
			: Node(pos_start, pos_end), m_elements(elements) { }

		ListNode::~ListNode()
		{
			for (Node* e : m_elements)
			{
				delete e;
			}
			m_elements.clear();
		}

	}

}