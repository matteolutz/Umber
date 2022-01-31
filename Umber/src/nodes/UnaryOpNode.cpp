#include "UnaryOpNode.h"

namespace umber
{

	namespace nodes
	{



		UnaryOpNode::UnaryOpNode(Token op_token, Node node)
			: Node(op_token.pos_start(), node.pos_end()), m_op_token(op_token), m_node(node) { }

	}

}