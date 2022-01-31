#pragma once

#include "Token.h"

#include <vector>
#include <optional>

namespace umber
{

	class Parser
	{
	private:
		std::vector<Token> m_tokens;
		unsigned int m_token_index = 0;
		std::optional<Token> m_current_token;

	public:
		Parser(std::vector<Token> tokens);

		Token& advance();
		Token& reverse();

		void update_current_token();

	};

}