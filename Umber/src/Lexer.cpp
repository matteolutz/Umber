#include "Lexer.h"

namespace umber
{

	Lexer::Lexer(std::string filename, std::shared_ptr<std::string> filetext)
		: m_filename(filename), m_filetext(filetext),
		m_pos({ 0, 0, 0, filename, filetext })
	{
		m_current_char = this->m_pos.index() < this->m_filetext->length() ? std::make_optional<char>(this->m_filetext->at(this->m_pos.index())) : std::nullopt;
	}


	void Lexer::advance()
	{
		this->m_pos.advance(this->m_current_char.value());
		m_current_char = this->m_pos.index() < this->m_filetext->length() ? std::make_optional<char>(this->m_filetext->at(this->m_pos.index())) : std::nullopt;
	}

	std::pair<std::optional<std::vector<Token>>, std::unique_ptr<Error>> Lexer::make_tokens()
	{
		std::vector<Token> tokens;

		while (this->m_current_char.has_value())
		{
			char current = this->m_current_char.value();


			if (current == '\n' || current == '\t' || current == ' ')
			{
				this->advance();
			}
			else if (current == '#')
			{
				this->skip_comment();
			}
			else if (current == ';')
			{
				tokens.emplace_back(Token{ TokenType::Newline, this->m_pos });
				this->advance();
			}
			else if (isdigit(current))
			{
				tokens.emplace_back(this->make_number());
			}
			else if (isalpha(current))
			{
				tokens.emplace_back(this->make_identifier());
			}
			else if (current == '\"')
			{
				tokens.emplace_back(this->make_string());
			}
			else if (current == '+')
			{
				tokens.emplace_back(Token{ TokenType::Plus, this->m_pos });
				this->advance();
			}
			else if (current == '-')
			{
				tokens.emplace_back(this->make_minus_or_arrow());
			}
			else if (current == '*')
			{
				tokens.emplace_back(Token{ TokenType::Mult, this->m_pos });
				this->advance();
			}
			else if (current == '/')
			{
				tokens.emplace_back(Token{ TokenType::Div, this->m_pos });
				this->advance();
			}
			else if (current == '^')
			{
				tokens.emplace_back(Token{ TokenType::Pow, this->m_pos });
				this->advance();
			}
			else if (current == ':')
			{
				tokens.emplace_back(Token{ TokenType::Colon, this->m_pos });
				this->advance();
			}
			else if (current == '(')
			{
				tokens.emplace_back(Token{ TokenType::Lparen, this->m_pos });
				this->advance();
			}
			else if (current == ')')
			{
				tokens.emplace_back(Token{ TokenType::Rparen, this->m_pos });
				this->advance();
			}
			else if (current == '[')
			{
				tokens.emplace_back(Token{ TokenType::Lsquare, this->m_pos });
				this->advance();
			}
			else if (current == ']')
			{
				tokens.emplace_back(Token{ TokenType::Rsquare, this->m_pos });
				this->advance();
			}
			else if (current == '{')
			{
				tokens.emplace_back(Token{ TokenType::Lcurly, this->m_pos });
				this->advance();
			}
			else if (current == '}')
			{
				tokens.emplace_back(Token{ TokenType::Rcurly, this->m_pos });
				this->advance();
			}
			else if (current == '!')
			{
				std::pair<std::optional<Token>, std::unique_ptr<Error>> result = this->make_not_equals();

				if (result.second != nullptr)
				{
					return { std::nullopt, std::move(result.second) };
				}

				tokens.emplace_back(result.first.value());
			}
			else if (current == '=')
			{
				tokens.emplace_back(this->make_equals());
			}
			else if (current == '<')
			{
				tokens.emplace_back(this->make_less_than());
			}
			else if (current == '>')
			{
				tokens.emplace_back(this->make_greater_than());
			}
			else if (current == ',')
			{
				tokens.emplace_back(Token{ TokenType::Comma, this->m_pos });
				this->advance();
			}
			else if (current == '.')
			{
				tokens.emplace_back(Token{ TokenType::Accessor, this->m_pos });
				this->advance();
			}
			else
			{
				Position pos_start = this->m_pos;
				char err_char = current;
				this->advance();

				char err_details_buffer[3];
				sprintf_s(err_details_buffer, "'%c'", err_char);

				return { std::nullopt, std::make_unique<errors::IllegalCharacterError>(pos_start, this->m_pos, err_details_buffer) };
			}
		}

		tokens.emplace_back(Token{ TokenType::Eof, this->m_pos });
		return { tokens, nullptr };
	}

#pragma region Maker Functions

	void Lexer::skip_comment()
	{
		this->advance();

		while (this->m_current_char.has_value() && this->m_current_char.value() != '\n')
		{
			this->advance();
		}

		this->advance();
	}

	Token Lexer::make_number()
	{
		std::string num_string;
		unsigned int dot_count = 0;
		Position pos_start = this->m_pos;

		while (
			this->m_current_char.has_value() &&
			(
				isdigit(this->m_current_char.value())
				||
				this->m_current_char.value() == '.'
				)
			)
		{
			if (this->m_current_char.value() == '.')
			{
				if (dot_count == 1)
				{
					break;
				}

				dot_count++;
				num_string.push_back('.');
			}
			else
			{
				num_string.push_back(this->m_current_char.value());
			}
			this->advance();
		}

		return Token{ dot_count == 0 ? TokenType::Int : TokenType::Float, num_string, pos_start, this->m_pos };

	}

	Token Lexer::make_identifier()
	{
		std::string id_string;
		Position pos_start = this->m_pos;

		while (
			this->m_current_char.has_value() &&
			(
				isdigit(this->m_current_char.value()) || isalpha(this->m_current_char.value())
				|| this->m_current_char.value() == '_'
				)
			)
		{
			id_string.push_back(this->m_current_char.value());
			this->advance();
		}

		TokenType id_token_type = std::find(std::begin(KEYWORDS), std::end(KEYWORDS), id_string) != std::end(KEYWORDS) ? TokenType::Keyword : TokenType::Identifier;
		return Token{ id_token_type, id_string, pos_start, this->m_pos };

	}

	Token Lexer::make_string()
	{
		std::string new_string;
		Position pos_start = this->m_pos;
		bool escape_character = false;

		this->advance();

		while (this->m_current_char.has_value() && (this->m_current_char.value() != '\"' || escape_character))
		{
			if (escape_character)
			{
				std::map<char, char>::const_iterator escaped_character_pos = constants::ESCAPED_CHARACTERS.find(this->m_current_char.value());
				char replace_char = this->m_current_char.value();

				if (escaped_character_pos != constants::ESCAPED_CHARACTERS.end())
				{
					replace_char = escaped_character_pos->second;
				}

				new_string.push_back(replace_char);
			}
			else
			{
				if (this->m_current_char.value() == '\\')
				{
					escape_character = true;
				}
				else
				{
					new_string.push_back(this->m_current_char.value());
				}
			}

			this->advance();
			escape_character = false;
		}

		this->advance();
		return Token{ TokenType::String, new_string, pos_start, this->m_pos };
	}

	Token Lexer::make_minus_or_arrow()
	{
		TokenType r_token_type = TokenType::Minus;
		Position pos_start = this->m_pos;
		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '>')
		{
			this->advance();
			r_token_type = TokenType::Arrow;
		}

		return Token{ r_token_type, pos_start, this->m_pos };
	}

	std::pair<std::optional<Token>, std::unique_ptr<Error>> Lexer::make_not_equals()
	{
		Position pos_start = this->m_pos;
		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '=')
		{
			this->advance();
			return { Token{ TokenType::Ne, pos_start, this->m_pos }, nullptr };
		}

		this->advance();
		return { std::nullopt, std::make_unique<errors::ExpectedCharError>(pos_start, this->m_pos, "'=' (after '!')") };

	}

	Token Lexer::make_equals()
	{
		TokenType r_token_type = TokenType::Eq;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '=')
		{
			this->advance();
			r_token_type = TokenType::Ee;
		}

		return Token{ r_token_type, pos_start, this->m_pos };
	}

	Token Lexer::make_less_than()
	{
		TokenType r_token_type = TokenType::Lt;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '=')
		{
			this->advance();
			r_token_type = TokenType::Lte;
		}

		return Token{ r_token_type, pos_start, this->m_pos };
	}

	Token Lexer::make_greater_than()
	{
		TokenType r_token_type = TokenType::Gt;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '=')
		{
			this->advance();
			r_token_type = TokenType::Gte;
		}

		return Token{ r_token_type, pos_start, this->m_pos };
	}

#pragma endregion

}