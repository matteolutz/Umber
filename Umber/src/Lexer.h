#pragma once

#include "Position.h"
#include "Token.h"
#include "Error.h"
#include "errors/IllegalCharacterError.h"
#include "errors/ExpectedCharError.h"
#include "Constants.h"

#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <map>
#include <ctype.h>

namespace umber
{
	class Lexer
	{
	private:
		std::string m_filename;
		std::shared_ptr<std::string> m_filetext;

		Position m_pos;
		std::optional<char> m_current_char;

	private:
		void skip_comment();
		Token make_number();
		Token make_identifier();
		Token make_string();
		Token make_minus_or_arrow();

		std::pair<std::optional<Token>, Error*> make_not_equals();
		Token make_equals();
		Token make_less_than();
		Token make_greater_than();

	public:
		Lexer(std::string filename, std::shared_ptr<std::string> filetext);

		void advance();
		std::pair<std::optional<std::vector<Token>>, Error*> make_tokens();

	};
}