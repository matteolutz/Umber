#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class VarAccessNode : public Node
		{
		private:
			Token m_var_name_token;

		public:
			VarAccessNode(Token var_name_token);

			inline const Token& var_name_token() const { return this->m_var_name_token; }

		};

	}

}