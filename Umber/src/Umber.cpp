#include "Umber.h"

namespace umber
{

	void Test()
	{
		printf("Umber-Lang test was successful!\n");
	}

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

}