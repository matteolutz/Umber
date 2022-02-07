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
			std::shared_ptr<Node> m_node_to_return;

		public:
			ReturnNode(std::shared_ptr<Node> node_to_return, Position pos_start, Position pos_end);

			inline const std::shared_ptr<Node>& node_to_return() const { return this->m_node_to_return; }

		};

	}

}