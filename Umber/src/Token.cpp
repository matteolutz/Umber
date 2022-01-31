#include "Token.h"

namespace umber
{

	Token::Token(TokenType type, std::optional<std::string> value, Position pos_start, std::optional<Position> pos_end)
		: m_type(type), m_value(value), m_pos_start(pos_start), m_pos_end(pos_end.has_value() ? pos_end.value() : pos_start)
	{
	}

	Token::Token(TokenType type, Position pos_start, std::optional<Position> pos_end)
		: m_type(type), m_value(std::nullopt), m_pos_start(pos_start), m_pos_end(pos_end.has_value() ? pos_end.value() : pos_start)
	{
	}

	const TokenType& Token::type() const
	{
		return this->m_type;
	}

	const std::optional<std::string>& Token::value() const
	{
		return this->m_value;
	}

	const Position& Token::pos_start() const
	{
		return this->m_pos_start;
	}

	const Position& Token::pos_end() const
	{
		return this->m_pos_end;
	}


}