#include "AccessorNode.h"

namespace umber
{

	namespace nodes
	{


		AccessorNode::AccessorNode(std::shared_ptr<Node> node, Token accessor_token)
			: Node(node->pos_start(), accessor_token.pos_end(), NodeType::Accessor), m_node(node), m_accessor_token(accessor_token)
		{
		}

	}

}