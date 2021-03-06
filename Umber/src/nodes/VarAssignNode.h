#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class VarAssignNode : public Node
		{
		private:
			Token m_var_name_token;
			std::shared_ptr<Node> m_value_node;

		public:
			VarAssignNode(Token var_name_token, std::shared_ptr<Node> value_node);

			inline const Token& var_name_token() const { return this->m_var_name_token; }
			inline const std::shared_ptr<Node>& value_node() const { return this->m_value_node; }

		};

	}

}