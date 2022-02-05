#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class BinOpNode : public Node
		{
		private:
			Node* m_left_node, * m_right_node;
			Token m_op_token;

		public:
			BinOpNode(Node* left_node, Token op_token, Node* right_node);
			~BinOpNode();

		};

	}

}