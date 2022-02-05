#include "ForNode.h"

namespace umber
{

	namespace nodes
	{



		ForNode::ForNode(
			Token var_name_token,
			Node* start_value_node, Node* end_value_node, Node* step_value_node,
			Node* body_node, bool should_return_null)
			: Node(var_name_token.pos_start(), body_node->pos_end()),
			m_var_name_token(var_name_token),
			m_start_value_node(start_value_node), m_end_value_node(end_value_node), m_step_value_node(step_value_node),
			m_body_node(body_node), m_should_return_null(should_return_null)
		{
		}

		ForNode::~ForNode()
		{
			delete m_start_value_node, m_end_value_node, m_step_value_node, m_body_node;
		}

	}

}