#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{
		class StringNode : public Node
		{
		private:
			Token m_token;

		public:
			StringNode(Token token);

			inline const Token& token() const { return this->m_token; }
		};
	}

}