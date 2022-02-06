#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class VarDeclarationNode : public Node
		{
		private:
			Token m_var_name_token;
			std::shared_ptr<Node> m_value_node;
			bool m_is_mutable;

		public:
			VarDeclarationNode(Token var_name_token, std::shared_ptr<Node> value_node, bool is_mutable);

			inline const Token& var_name_token() const { return this->m_var_name_token; }
			inline const std::shared_ptr<Node>& value_node() const { return this->m_value_node; }
			inline const bool& is_mutable() const { return this->m_is_mutable; }

		};

	}

}