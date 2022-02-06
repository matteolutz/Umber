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
			std::shared_ptr<Node> m_left_node, m_right_node;
			Token m_op_token;

		public:
			BinOpNode(std::shared_ptr<Node> left_node, Token op_token, std::shared_ptr<Node> right_node);

		};

	}

}