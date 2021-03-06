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
			std::shared_ptr<Node> m_node_to_call;
			std::vector<std::shared_ptr<Node>> m_arg_nodes;

		public:
			CallNode(std::shared_ptr<Node> node_to_call, std::vector<std::shared_ptr<Node>> arg_nodes);

			inline const std::shared_ptr<Node>& node_to_call() const { return this->m_node_to_call; }
			inline const std::vector<std::shared_ptr<Node>>& arg_nodes() const { return this->m_arg_nodes; }

		};

	}

}