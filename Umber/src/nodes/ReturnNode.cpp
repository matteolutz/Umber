#include "ReturnNode.h"

namespace umber
{

	namespace nodes
	{

		ReturnNode::ReturnNode(Node* node_to_return, Position pos_start, Position pos_end)
			: Node(pos_start, pos_end), m_node_to_return(node_to_return) { }

		ReturnNode::~ReturnNode()
		{
			delete m_node_to_return;
		}

	}

}