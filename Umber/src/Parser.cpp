#include "Parser.h"

namespace umber
{



	Parser::Parser(std::vector<Token> tokens)
		: m_tokens(tokens)
	{
		if (!this->m_tokens.empty())
		{
			this->m_current_token.emplace(this->m_tokens[0]);
		}
	}

	Token& Parser::advance()
	{
		this->m_token_index++;
		this->update_current_token();
		return this->m_current_token.value();
	}

	Token& Parser::reverse()
	{
		this->m_token_index--;
		this->update_current_token();
		return this->m_current_token.value();
	}

	void Parser::update_current_token()
	{
		if (this->m_token_index >= 0 && this->m_token_index < this->m_tokens.size())
		{
			this->m_current_token.emplace(this->m_tokens[this->m_token_index]);
		}
	}

}