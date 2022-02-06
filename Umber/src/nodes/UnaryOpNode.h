#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class UnaryOpNode : public Node
		{
		private:
			Token m_op_token;
			std::shared_ptr<Node> m_node;

		public:
			UnaryOpNode(Token op_token, std::shared_ptr<Node> node);

		};

	}

}