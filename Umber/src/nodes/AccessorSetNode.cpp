#include "AccessorSetNode.h"

namespace umber
{

	namespace nodes
	{


		AccessorSetNode::AccessorSetNode(std::shared_ptr<Node> accessed_node, std::shared_ptr<Node> accessor_node, std::shared_ptr<Node> set_node)
			: Node(accessed_node->pos_start(), set_node->pos_end(), NodeType::AccessorSet), m_accessed_node(accessed_node), m_accessor_node(accessor_node), m_set_node(set_node)
		{
		}

	}

}