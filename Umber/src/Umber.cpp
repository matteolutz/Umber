#include "Umber.h"

#include <chrono>


const char* test = R""""(
while 1 < 1 and 1 > 1 {
	print("Hello World!");
}
)"""";

namespace umber
{

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define time_now() std::chrono::high_resolution_clock::now()

	void RunFile(std::string path)
	{

		std::string code;
		std::ifstream file_stream(path);
		while (std::getline(file_stream, code));
		file_stream.close();

		// auto code_ptr = std::make_shared<std::string>(code);
		
		// printf("Code: %s, Pointer: %p", code.c_str(), code_ptr.get());
	}

	std::pair<std::optional<std::vector<Token>>, Error*> RunText(std::string text)
	{
		Lexer l("<cin>", text);

		return l.make_tokens();
	}

	void Test()
	{
		auto begin = time_now();

		Lexer l("<cin>", test);
		std::pair<std::optional<std::vector<Token>>, Error*> lexer_res = l.make_tokens();

		if (lexer_res.second != nullptr)
		{
			printf("Error");
			delete lexer_res.second;
			return;
		}

		std::vector<Token> tokens = lexer_res.first.value();

		//printf("num tokens: %d\n", tokens.size());
		for (const Token& t : tokens)
		{
			printf("[%d: %s]\n", t.type(), t.value().value_or("NULL").c_str());
		}

		Parser p(tokens);
		result::ParseResult res = p.parse();

		//printf("Parsing done\n");

		if (res.has_error())
		{
			printf("Error: %s, from (%d, %d) to (%d, %d)\n", typeid(res.error()).name(), res.error()->pos_start().line(), res.error()->pos_start().col(), res.error()->pos_end().line(), res.error()->pos_end().col());
			return;
		}

		auto end = time_now();

		printf("Took: %3.4fms\n", duration(end - begin) / 1000000.0);
			
		printf("Parent node: %s\n", res.node()->as_string());

		res.cleanup();
	}

}