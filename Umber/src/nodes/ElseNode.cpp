#include "ElseNode.h"


namespace umber
{

	namespace nodes
	{
		ElseNode::ElseNode(Node statements, bool bool_return)
			: Node(statements.pos_start(), statements.pos_end()), m_statements(statements), m_bool_return(bool_return) { }
	}

}