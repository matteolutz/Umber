#pragma once

#include "../Node.h"
#include "../Token.h"

#include <string>

namespace umber
{

	namespace nodes
	{

		class NumberNode : public Node
		{
		private:
			Token m_token;

		public:
			NumberNode(Token token);

			inline const Token& token() const { return this->m_token; }
		};

	}

}