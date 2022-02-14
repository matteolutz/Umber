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
				tokens.emplace_back(TokenType::Newline, this->m_pos);
				this->advance();
			}
			else if (isdigit(current))
			{
				tokens.push_back(this->make_number());
			}
			else if (isalpha(current))
			{
				tokens.push_back(this->make_identifier());
			}
			else if (current == '"')
			{
				tokens.push_back(this->make_string());
			}
			else if (current == '+')
			{
				tokens.emplace_back(TokenType::Plus, this->m_pos);
				this->advance();
			}
			else if (current == '-')
			{
				tokens.push_back(this->make_minus_or_arrow());
			}
			else if (current == '*')
			{
				tokens.emplace_back(TokenType::Mult, this->m_pos);
				this->advance();
			}
			else if (current == '/')
			{
				tokens.emplace_back(TokenType::Div, this->m_pos);
				this->advance();
			}
			else if (current == '^')
			{
				tokens.emplace_back(TokenType::Pow, this->m_pos);
				this->advance();
			}
			else if (current == '%')
			{
				tokens.emplace_back(TokenType::Modulo, this->m_pos);
				this->advance();
			}
			else if (current == ':')
			{
				// tokens.push_back(this->make_colon_or_accessor());
				tokens.emplace_back(TokenType::Colon, this->m_pos);
				this->advance();
			}
			else if (current == '(')
			{
				tokens.emplace_back(TokenType::Lparen, this->m_pos);
				this->advance();
			}
			else if (current == ')')
			{
				tokens.emplace_back(TokenType::Rparen, this->m_pos);
				this->advance();
			}
			else if (current == '[')
			{
				tokens.emplace_back(TokenType::Lsquare, this->m_pos);
				this->advance();
			}
			else if (current == ']')
			{
				tokens.emplace_back(TokenType::Rsquare, this->m_pos);
				this->advance();
			}
			else if (current == '{')
			{
				tokens.emplace_back(TokenType::Lcurly, this->m_pos);
				this->advance();
			}
			else if (current == '}')
			{
				tokens.emplace_back(TokenType::Rcurly, this->m_pos);
				this->advance();
			}
			else if (current == '!')
			{
				tokens.push_back(this->make_not_equals());
			}
			else if (current == '=')
			{
				tokens.push_back(this->make_equals());
			}
			else if (current == '<')
			{
				tokens.push_back(this->make_less_than());
			}
			else if (current == '>')
			{
				tokens.push_back(this->make_greater_than());
			}
			else if (current == '&')
			{
				tokens.push_back(this->make_and());
			}
			else if (current == '|')
			{
				tokens.push_back(this->make_or());
			}
			else if (current == ',')
			{
				tokens.emplace_back(TokenType::Comma, this->m_pos);
				this->advance();
			}
			else if (current == '.')
			{
				tokens.emplace_back(TokenType::Accessor, this->m_pos);
				this->advance();
			}
			else
			{
				Position pos_start = this->m_pos;
				char err_char = current;
				this->advance();

				return { std::nullopt, std::make_unique<errors::IllegalCharacterError>(pos_start, this->m_pos, utils::std_string_format("'%c'", err_char).c_str()) };
			}
		}

		tokens.emplace_back(TokenType::Eof, this->m_pos);
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

		return { dot_count == 0 ? TokenType::Int : TokenType::Float, num_string, pos_start, this->m_pos };

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
		return { id_token_type, id_string, pos_start, this->m_pos };

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
				auto escaped_character_pos = constants::ESCAPED_CHARACTERS.find(this->m_current_char.value());
				char replace_char = this->m_current_char.value();

				if (escaped_character_pos != constants::ESCAPED_CHARACTERS.end())
				{
					replace_char = escaped_character_pos->second;
				}

				new_string.push_back(replace_char);

				escape_character = false;
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
		}

		this->advance();
		return { TokenType::String, new_string, pos_start, this->m_pos };
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

		return { r_token_type, pos_start, this->m_pos };
	}

	Token Lexer::make_colon_or_accessor()
	{
		TokenType r_token_type = TokenType::Colon;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char == ':')
		{
			this->advance();
			r_token_type = TokenType::Accessor;
		}

		return { r_token_type, pos_start, this->m_pos };
	}

	Token Lexer::make_not_equals()
	{
		TokenType r_token_type = TokenType::Not;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '=')
		{
			this->advance();
			r_token_type = TokenType::Ne;
		}

		return { r_token_type, pos_start, this->m_pos };

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

		return { r_token_type, pos_start, this->m_pos };
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

		return { r_token_type, pos_start, this->m_pos };
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

		return { r_token_type, pos_start, this->m_pos };
	}

	Token Lexer::make_and()
	{
		TokenType r_token_type = TokenType::BitAnd;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '&')
		{
			this->advance();
			r_token_type = TokenType::And;
		}

		return { r_token_type, pos_start, this->m_pos };
	}

	Token Lexer::make_or()
	{
		TokenType r_token_type = TokenType::BitOr;
		Position pos_start = this->m_pos;

		this->advance();

		if (this->m_current_char.has_value() && this->m_current_char.value() == '|')
		{
			this->advance();
			r_token_type = TokenType::Or;
		}

		return { r_token_type, pos_start, this->m_pos };
	}

#pragma endregion

}