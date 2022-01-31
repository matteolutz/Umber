#include "CallNode.h"

namespace umber
{

	namespace nodes
	{

		

		CallNode::CallNode(Node node_to_call, std::vector<Node> arg_nodes)
			: Node(node_to_call.pos_start(), !arg_nodes.empty() ? arg_nodes.back().pos_end() : node_to_call.pos_end()), m_node_to_call(node_to_call), m_arg_nodes(arg_nodes) { }

	}

}