#include "AccessorNode.h"

namespace umber
{

	namespace nodes
	{


		AccessorNode::AccessorNode(std::shared_ptr<Node> node, std::shared_ptr<Node> access_node)
			: Node(node->pos_start(), access_node->pos_end(), NodeType::Accessor), m_node(node), m_accessor_node(access_node)
		{
		}

	}

}