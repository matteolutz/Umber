#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class AccessorNode : public Node
		{
		private:
			std::shared_ptr<Node> m_node;
			Token m_accessor_token;

		public:
			AccessorNode(std::shared_ptr<Node> node, Token accessor_token);

			inline const std::shared_ptr<Node>& node() const { return this->m_node; }
			inline const Token& accessor_token() const { return this->m_accessor_token; }

		};

	}

}