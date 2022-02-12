#include "WhileNode.h"

namespace umber
{

	namespace nodes
	{



		WhileNode::WhileNode(std::shared_ptr<Node> condition_node, std::shared_ptr<Node> body_node, bool should_return_null)
			: Node(condition_node->pos_start(), body_node->pos_end(), NodeType::While), m_condition_node(condition_node), m_body_node(body_node), m_should_return_null(should_return_null) { }

	}

}