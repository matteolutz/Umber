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
			Node* m_value_node;

		public:
			VarAssignNode(Token var_name_token, Node* value_node);
			~VarAssignNode();

		};

	}

}