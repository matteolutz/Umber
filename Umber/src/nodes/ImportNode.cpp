#include "ImportNode.h"

namespace umber
{

	namespace nodes
	{

		ImportNode::ImportNode(Token import_name, Position pos_start, Position pos_end)
			: Node(pos_start, pos_end, NodeType::Import), m_import_name(import_name)
		{
		}

	}

}