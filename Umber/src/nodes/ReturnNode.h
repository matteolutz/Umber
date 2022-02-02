#pragma once

#include "../Node.h"
#include "../Position.h"

namespace umber
{
	
	namespace nodes
	{

		class ReturnNode : public Node
		{
		private:
			std::optional<Node> m_node_to_return;

		public:
			ReturnNode(std::optional<Node> node_to_return, Position pos_start, Position pos_end);
		};

	}

}