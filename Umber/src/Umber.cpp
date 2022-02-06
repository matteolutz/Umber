#include "Umber.h"

#include <chrono>


const char* test = R""""(
while 1 == 1 {
	print("Hello World");
	break;
};

print("Done, exiting...");
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

	std::pair<std::optional<std::vector<Token>>, std::unique_ptr<Error>> RunText(std::string text)
	{
		Lexer l("<cin>", std::make_shared<std::string>(text));

		return l.make_tokens();
	}

	void Test()
	{
		auto begin = time_now();

		auto test_ptr = std::make_shared<std::string>(test);

		test_ptr.get();

		Lexer l("<cin>", test_ptr);
		std::pair<std::optional<std::vector<Token>>, std::unique_ptr<Error>> lexer_res = l.make_tokens();

		if (lexer_res.second != nullptr)
		{
			printf("Error");
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
			printf("Error: %s, from (%d, %d) to (%d, %d):\n", typeid(res.error()).name(), res.error()->pos_start().line(), res.error()->pos_start().col(), res.error()->pos_end().line(), res.error()->pos_end().col());
			printf("\t%s\n", res.error()->as_string().c_str());
			return;
		}

		auto end = time_now();

		printf("Took: %3.4fms\n", duration(end - begin) / 1000000.0);
			
		printf("Parent node: %s\n", res.node()->as_string().c_str());

	
		//std::shared_ptr<SymbolTable> main_symbol_table = std::make_shared<SymbolTable>();
		//std::shared_ptr<Context> main_context = std::make_shared<Context>("<main>", nullptr, main_symbol_table);

		//Interpreter::visit(res.node(), main_context);
		Interpreter::visit(res.node(), nullptr);
	}

}