#include "BinOpNode.h"

namespace umber
{

	namespace nodes
	{

		BinOpNode::BinOpNode(std::shared_ptr<Node> left_node, Token op_token, std::shared_ptr<Node> right_node)
			: Node(left_node->pos_start(), right_node->pos_end(), NodeType::BinOp), m_left_node(std::move(left_node)), m_op_token(op_token), m_right_node(std::move(right_node))
		{
		}

	}

}