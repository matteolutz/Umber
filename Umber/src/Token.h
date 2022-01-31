#pragma once

#include "Position.h"

#include <optional>

namespace umber
{

	enum TokenType
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

	static const std::string KEYWORDS[16] = {
		"var",
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
		Token(TokenType type, std::optional<std::string> value, Position pos_start, std::optional<Position> pos_end = std::nullopt);
		Token(TokenType type, Position pos_start, std::optional<Position> pos_end = std::nullopt);

		const TokenType& type() const;
		const std::optional<std::string>& value() const;

		const Position& pos_start() const;
		const Position& pos_end() const;

		bool operator==(const Token& other) const
		{
			return this->m_type == other.m_type && this->m_value == other.m_value;
		}
	};

}
