#include "Umber.h"

#include <chrono>


const char* primes_test = R""""(
fun gen_primes_to (limit) {
	let mut count = 0;
	let mut primes = [];


	while count < limit {
		let mut is_prime = 1;

		for x = 2 to count {
			if math_mod(count, x) == 0 {
				is_prime = 0;
				break;
			};
		};

		if is_prime == 1 {
			primes = primes + count;
		};

		count = count + 1;
	};

	return primes;
};

print("starting...");
print(gen_primes_to(100));
)"""";

const char* test = R""""(

1+1+1;

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

		Parser p(tokens);
		result::ParseResult parse_res = p.parse();

		if (parse_res.has_error())
		{
			printf("%s\n", parse_res.error()->as_string().c_str());
			return;
		}
	
		std::shared_ptr<SymbolTable> global_symbol_table = std::make_shared<SymbolTable>();

		std::string print_arg_names[] = {"value"};
		std::string print_name = "print";
		std::shared_ptr<values::BuiltInFunction> print_test_function =
			std::make_shared<values::BuiltInFunction>(
				print_name,
				std::vector<std::string>(std::begin(print_arg_names), std::end(print_arg_names)),
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

		std::string math_mod_arg_names[] = { "value", "mod" };
		std::string math_mod_name = "math_mod";
		std::shared_ptr<values::BuiltInFunction> math_mod_test_function =
			std::make_shared<values::BuiltInFunction>(
				math_mod_name,
				std::vector<std::string>(std::begin(math_mod_arg_names), std::end(math_mod_arg_names)),
				[](std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx, values::BuiltInFunction* self) -> result::RuntimeResult
				{
					auto res = result::RuntimeResult();

					std::string value_name = "value";
					std::string mod_name = "mod";

					std::optional<SymbolTable::symbol> value = exec_ctx->symbol_table()->get(value_name);
					std::optional<SymbolTable::symbol> mod = exec_ctx->symbol_table()->get(mod_name);

					if (!value.has_value() || !mod.has_value())
					{
						res.failure(std::make_shared<errors::RuntimeError>(self->pos_start(), self->pos_end(), "error", exec_ctx));
						return res;
					}

					std::shared_ptr<values::NumberValue> number = std::dynamic_pointer_cast<values::NumberValue>(value.value().m_value);
					std::shared_ptr<values::NumberValue> mod_number = std::dynamic_pointer_cast<values::NumberValue>(mod.value().m_value);

					if (number == nullptr || mod_number == nullptr)
					{
						res.failure(std::make_shared<errors::RuntimeError>(self->pos_start(), self->pos_end(), "'value' must be a number!", exec_ctx));
						return res;
					}

					res.success(std::make_shared<values::NumberValue>(std::fmod(number->value(), mod_number->value())));
					return res;
				});

		global_symbol_table->declare(print_name, { print_test_function, false });
		global_symbol_table->declare(math_mod_name, { math_mod_test_function, false });

		std::shared_ptr<Context> global_context = std::make_shared<Context>("<main>", nullptr, global_symbol_table);

		result::RuntimeResult inrepreter_res = Interpreter::visit(parse_res.node(), global_context);

		if (inrepreter_res.has_error())
		{
			printf("Interpreter error:\n\n%s\n", inrepreter_res.error()->as_string().c_str());
			return;
		}

		auto end = time_now();

		printf("Took: %3.4fms\n", duration(end - begin) / 1000000.0);
	}

}