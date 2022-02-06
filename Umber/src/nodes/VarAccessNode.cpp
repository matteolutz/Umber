#include "VarAccessNode.h"

namespace umber
{

	namespace nodes
	{

		VarAccessNode::VarAccessNode(Token var_name_token)
			: Node(var_name_token.pos_start(), var_name_token.pos_end(), NodeType::VarAccess), m_var_name_token(var_name_token) { }

	}

}