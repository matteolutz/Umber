#pragma once

#include "Position.h"
#include "utils/Utils.h"

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
		Modulo,
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
		And,
		BitAnd,
		Or,
		BitOr,
		Not,
		Comma,
		Arrow,
		Accessor,
		Newline,
		Eof,
	};

	static const std::string KEYWORDS[15] = 
	{
		"let",
		"mut",
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
		"break",
		"import"
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

		inline const Position& pos_start() const { return this->m_pos_start; }
		inline const Position& pos_end() const { return this->m_pos_end; }

		bool matches(const TokenType& type, const std::optional<std::string>& value) const;

		inline int get_as_int() const { return std::stoi(this->value_or_zero()); }
		inline float get_as_float() const { return std::stof(this->value_or_zero()); }
		inline const std::string value_or_zero() const { return this->m_value.value_or("0"); }

		bool operator==(const Token& other) const
		{
			return this->matches(other.m_type, other.m_value);
		}

		virtual std::string as_string() const;

	};

}
