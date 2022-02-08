#include "Umber.h"

#include <chrono>


const char* test = R""""(
jl1jlkj
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

		// auto test_ptr = std::make_shared<std::string>(test);

		// test_ptr.get();

		Lexer l("<cin>", std::make_shared<std::string>(test));
		std::pair<std::optional<std::vector<Token>>, std::unique_ptr<Error>> lexer_res = l.make_tokens();

		if (lexer_res.second != nullptr)
		{
			printf("%s\n", lexer_res.second->as_string().c_str());
			return;
		}

		std::vector<Token> tokens = lexer_res.first.value();

		//printf("num tokens: %d\n", tokens.size());
		for (const Token& t : tokens)
		{
			printf("%s\n", t.as_string().c_str());
		}

		Parser p(tokens);
		result::ParseResult parse_res = p.parse();

		//printf("Parsing done\n");

		if (parse_res.has_error())
		{
			/*printf("Error: %s, from (%d, %d) to (%d, %d):\n", typeid(res.error()).name(), res.error()->pos_start().line(), res.error()->pos_start().col(), res.error()->pos_end().line(), res.error()->pos_end().col());
			printf("\t%s\n", res.error()->as_string().c_str());*/
			printf("%s\n", parse_res.error()->as_string().c_str());
			return;
		}
			
		printf("Parent node: %s\n", parse_res.node()->as_string().c_str());

	
		std::shared_ptr<SymbolTable> main_symbol_table = std::make_shared<SymbolTable>();
		std::shared_ptr<Context> main_context = std::make_shared<Context>("<main>", nullptr, main_symbol_table);

		result::RuntimeResult inrepreter_res = Interpreter::visit(parse_res.node(), main_context);
		//result::RuntimeResult inrepreter_res = Interpreter::visit(parse_res.node(), nullptr);

		printf("Interpreter visiting done!\n");
		if (inrepreter_res.has_error())
		{
			printf("Interpreter error:\n\n%s\n", inrepreter_res.error()->as_string().c_str());
			return;
		}

		printf("Interpreter done: %s\n", inrepreter_res.value()->as_string().c_str());

		auto end = time_now();

		printf("Took: %3.4fms\n", duration(end - begin) / 1000000.0);
	}

}