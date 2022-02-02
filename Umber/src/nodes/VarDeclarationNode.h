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
			Node m_value_node;
			bool m_is_mutable;

		public:
			VarDeclarationNode(Token var_name_token, Node value_node, bool is_mutable);

		};

	}

}