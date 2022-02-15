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

	bool Token::matches(const TokenType& type, const std::optional<std::string>& value) const
	{
		return this->m_type == type && this->m_value == value;
	}

	std::string Token::as_string() const
	{
		return utils::std_string_format("[%d: %s]", this->m_type, this->m_value.value_or("NULL").c_str());
	}

}