#include "VarDeclarationNode.h"

namespace umber
{

	namespace nodes
	{

		VarDeclarationNode::VarDeclarationNode(Token var_name_token, std::shared_ptr<Node> value_node, bool is_mutable)
			: Node(var_name_token.pos_start(), value_node->pos_end()), m_var_name_token(var_name_token), m_value_node(value_node), m_is_mutable(is_mutable)
		{
		}

	}

}