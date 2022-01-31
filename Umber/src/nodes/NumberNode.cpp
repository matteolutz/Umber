#include "NumberNode.h"

namespace umber
{

	namespace nodes
	{
		NumberNode::NumberNode(Token token)
			: Node(token.pos_start(), token.pos_end()), m_token(token) { }

	}

}