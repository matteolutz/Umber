#pragma once

#include "../Node.h"
#include "../Token.h"

#include <optional>
#include <vector>

namespace umber
{

	namespace nodes
	{

		class FunctionDefNode : public Node
		{
		private:
			std::optional<Token> m_var_name_token;
			std::vector<Token> m_arg_name_tokens;
			Node m_body_node;
			bool m_should_return_null;

		public:
			FunctionDefNode(std::optional<Token> var_name_token, std::vector<Token> arg_name_tokens, Node body_node, bool should_return_null);
		};

	}

}