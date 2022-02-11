#include "DictNode.h"

namespace umber
{

	namespace nodes
	{

		DictNode::DictNode(std::map<std::string, std::shared_ptr<Node>> elements, Position pos_start, Position pos_end)
			: Node(pos_start, pos_end, NodeType::Dict), m_elements(elements)
		{
		}
	}

}