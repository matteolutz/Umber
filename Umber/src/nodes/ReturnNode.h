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
			Node* m_node_to_return;

		public:
			ReturnNode(Node* node_to_return, Position pos_start, Position pos_end);
			~ReturnNode();

		};

	}

}