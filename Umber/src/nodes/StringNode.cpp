#include "StringNode.h"

namespace umber
{

	namespace nodes
	{

		StringNode::StringNode(Token token)
			: Node(token.pos_start(), token.pos_end(), NodeType::String), m_token(token) { }

	}

}