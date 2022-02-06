#include "VarAssignNode.h"

namespace umber
{

	namespace nodes
	{



		VarAssignNode::VarAssignNode(Token var_name_token, std::shared_ptr<Node> value_node)
			: Node(var_name_token.pos_start(), value_node->pos_end(), NodeType::VarAssign), m_var_name_token(var_name_token), m_value_node(value_node) { }

	}

}