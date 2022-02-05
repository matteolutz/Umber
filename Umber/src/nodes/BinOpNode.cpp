#include "BinOpNode.h"

namespace umber
{

	namespace nodes
	{

		BinOpNode::BinOpNode(Node* left_node, Token op_token, Node* right_node)
			: Node(left_node->pos_start(), right_node->pos_end()), m_left_node(left_node), m_op_token(op_token), m_right_node(right_node)
		{
		}

		BinOpNode::~BinOpNode()
		{
			delete m_left_node, m_right_node;
		}

	}

}