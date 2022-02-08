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
			std::shared_ptr<Node> m_body_node;
			bool m_should_auto_return;

		public:
			FunctionDefNode(std::optional<Token> var_name_token, std::vector<Token> arg_name_tokens, std::shared_ptr<Node> body_node, bool should_auto_return);

			inline const std::optional<Token>& var_name_token() const { return this->m_var_name_token; }
			inline const std::vector<Token>& arg_name_tokens() const { return this->m_arg_name_tokens; }
			inline const std::shared_ptr<Node>& body_node() const { return this->m_body_node; }
			inline const bool& should_auto_return() const { return this->m_should_auto_return; }

		};

	}

}