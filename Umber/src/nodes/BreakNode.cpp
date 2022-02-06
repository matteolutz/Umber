#include "BreakNode.h"

namespace umber
{

	namespace nodes
	{


		BreakNode::BreakNode(Position pos_start, Position pos_end)
			: Node(pos_start, pos_end, NodeType::Break) { }

	}

}