#include "Umber.h"

#include <chrono>


const char* test = R""""(
fun gen_primes_to(limit) {
	let mut count = 0;
	let mut primes = [];

	return primes;
};

gen_primes_to(10);
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

	
		std::shared_ptr<SymbolTable> global_symbol_table = std::make_shared<SymbolTable>();

		std::string arg_names[] = {"value"};
		std::string print_name = "print";
		std::shared_ptr<values::BuiltInFunction> print_test_function =
			std::make_shared<values::BuiltInFunction>(
				"print",
				std::vector<std::string>(std::begin(arg_names), std::end(arg_names)),
				[](std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx, values::BuiltInFunction* self) -> result::RuntimeResult {
					auto res = result::RuntimeResult();

					std::string value_name = "value";
					std::optional<SymbolTable::symbol> s = exec_ctx->symbol_table()->get(value_name);

					if (!s.has_value())
					{
						res.failure(std::make_shared<errors::RuntimeError>(self->pos_start(), self->pos_end(), "error", exec_ctx));
						return res;
					}

					printf("%s\n", s.value().m_value->as_string().c_str());
					res.success(s.value().m_value);

					return res;
				});

		global_symbol_table->declare(print_name, { print_test_function, false });

		std::shared_ptr<Context> global_context = std::make_shared<Context>("<main>", nullptr, global_symbol_table);

		result::RuntimeResult inrepreter_res = Interpreter::visit(parse_res.node(), global_context);
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