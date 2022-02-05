#include "WhileNode.h"

namespace umber
{

	namespace nodes
	{



		WhileNode::WhileNode(Node* condition_node, Node* body_node, bool should_return_null)
			: Node(condition_node->pos_start(), body_node->pos_end()), m_condition_node(condition_node), m_body_node(body_node), m_should_return_null(should_return_null) { }

		WhileNode::~WhileNode()
		{
			delete m_condition_node, m_body_node;
		}

	}

}