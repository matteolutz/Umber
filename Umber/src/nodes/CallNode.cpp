#include "CallNode.h"

namespace umber
{

	namespace nodes
	{



		CallNode::CallNode(std::shared_ptr<Node> node_to_call, std::vector<std::shared_ptr<Node>> arg_nodes)
			: Node(node_to_call->pos_start(), !arg_nodes.empty() ? arg_nodes.back()->pos_end() : node_to_call->pos_end(), NodeType::Call), m_node_to_call(std::move(node_to_call)), m_arg_nodes(arg_nodes)
		{
		}


	}

}