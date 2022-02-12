#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class AccessorSetNode : public Node
		{
		private:
			std::shared_ptr<Node> m_accessed_node;
			Token m_accessor_token;
			std::shared_ptr<Node> m_set_node;

		public:
			AccessorSetNode(std::shared_ptr<Node> accessed_node, Token accessor_token, std::shared_ptr<Node> set_node);

			inline const std::shared_ptr<Node>& accessed_node() const { return this->m_accessed_node; }
			inline const Token& accessor_token() const { return this->m_accessor_token; }
			inline const std::shared_ptr<Node>& set_node() const { return this->m_set_node; }

		};

	}

}