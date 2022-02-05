#pragma once

#include "../Node.h"

#include <vector>

namespace umber
{

	namespace nodes
	{

		class CallNode : public Node
		{

		private:
			Node* m_node_to_call;
			std::vector<Node*> m_arg_nodes;

		public:
			CallNode(Node* node_to_call, std::vector<Node*> arg_nodes);
			~CallNode();

		};

	}

}