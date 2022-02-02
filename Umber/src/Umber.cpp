#include "Umber.h"

namespace umber
{

	void RunFile(std::string path)
	{

		std::string code;
		std::ifstream file_stream(path);
		while (std::getline(file_stream, code));
		file_stream.close();

		// auto code_ptr = std::make_shared<std::string>(code);
		
		// printf("Code: %s, Pointer: %p", code.c_str(), code_ptr.get());
	}

	std::tuple<std::optional<std::vector<Token>>, std::optional<Error>> RunText(std::string text)
	{
		Lexer l("<cin>", text);

		return l.make_tokens();
	}

	void Test()
	{
		Lexer l("<cin>", "1+1");
		std::tuple<std::optional<std::vector<Token>>, std::optional<Error>> lexer_res = l.make_tokens();

		if (std::get<1>(lexer_res).has_value())
		{
			printf("Error");
			return;
		}

		std::vector<Token> tokens = std::get<0>(lexer_res).value();

		Parser p(tokens);
		result::ParseResult res = p.parse();

	}

}