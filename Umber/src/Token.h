#pragma once

#include "Position.h"

#include <optional>
#include <string>

namespace umber
{

	enum class TokenType
	{
		Int,
		Float,
		String,
		Identifier,
		Keyword,
		Plus,
		Minus,
		Mult,
		Div,
		Pow,
		Eq,
		Colon,
		Lparen,
		Rparen,
		Lsquare,
		Rsquare,
		Lcurly,
		Rcurly,
		Ee,
		Ne,
		Lt,
		Gt,
		Lte,
		Gte,
		Comma,
		Arrow,
		Newline,
		Eof
	};

	static const std::string KEYWORDS[17] = 
	{
		"let",
		"mut",
		"and",
		"or",
		"not",
		"if",
		"elif",
		"else",
		"for",
		"to",
		"step",
		"while",
		"fun",
		"then",
		"return",
		"continue",
		"break"
	};

	class Token
	{

	private:
		TokenType m_type;
		std::optional<std::string> m_value;
		Position m_pos_start;
		Position m_pos_end;

	public:
		Token(TokenType type, std::optional<std::string> value, Position pos_start = Position{}, std::optional<Position> pos_end = std::nullopt);
		Token(TokenType type, Position pos_start = Position{}, std::optional<Position> pos_end = std::nullopt);

		inline const TokenType& type() const { return this->m_type; }
		inline const std::optional<std::string>& value() const { return this->m_value; }
		inline const std::string value_or_zero() const { return this->m_value.value_or("0"); }

		inline const Position& pos_start() const { return this->m_pos_start; }
		inline const Position& pos_end() const { return this->m_pos_end; }

		bool matches(TokenType type, std::optional<std::string> value) const;

		bool has_value() const;

		bool operator==(const Token& other) const
		{
			return this->matches(other.m_type, other.m_value);
		}
	};

}
