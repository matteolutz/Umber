#include "VarAssignNode.h"

namespace umber
{

	namespace nodes
	{



		VarAssignNode::VarAssignNode(Token var_name_token, Node* value_node)
			: Node(var_name_token.pos_start(), value_node->pos_end()), m_var_name_token(var_name_token), m_value_node(value_node) { }

		VarAssignNode::~VarAssignNode()
		{
			delete m_value_node;
		}

	}

}