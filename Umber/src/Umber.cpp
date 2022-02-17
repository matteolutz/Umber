#include "Umber.h"

#include <chrono>

const char *test = R""""(

# O(n^2)
fun bubble_sort_arr (arr, len) {
	let mut new_arr = arr;
	let mut swapped = 1;

	while swapped {

		swapped = 0;

		for i = 0 to len - 1 {
			if new_arr[i] > new_arr[i + 1] {

				let cache = new_arr[i];
				new_arr[i] = new_arr[i + 1];
				new_arr[i + 1] = cache;

				swapped  = 1;
			};
		};
	};

	return new_arr;
};

# O(n^2)
fun selection_sort_arr (arr, len) {

	let mut new_arr = arr;

	for i = 0 to len {
		let mut lowest_value_index = i;

		for j = i + 1 to len {
			if new_arr[j] < new_arr[lowest_value_index] {
				lowest_value_index = j;
			};
		};

		let cache = new_arr[i];
		new_arr[i] = new_arr[lowest_value_index];
		new_arr[lowest_value_index] = cache;
	};

	return new_arr;
};

# O(n log2n)
fun quick_sort_arr (arr, len) {

	if len < 2 {
		return arr;
	};

	let mut low = let mut same = let mut high = [];
	let mut low_count = let mut high_count = 0;

	let mut pivot = arr[randint(0, len - 1)];

	for i = 0 to len {
	
		if arr[i] < pivot {
			low = low + arr[i];
			low_count = low_count + 1;
		} elif arr[i] == pivot {
			same = same + arr[i];
		} elif arr[i] > pivot {
			high = high + arr[i];
			high_count = high_count + 1;
		};
	};

	return quick_sort_arr(low, low_count) + same + quick_sort_arr(high, high_count);

};

fun gen_arr (min, max) {

	let mut arr = [];

	for i = min to max {
		arr = arr + i;
	};
	
	return arr;

};

let test_arr = [43895, 3, 5, 38902456, 324783, 27819047921241, 124, 1, 34264324, 23452343456464, 21312, 123124544666364, 312312, 4334657, 12];
let test_arr_len = 15;

# print(bubble_sort_arr(test_arr, test_arr_len));
# print(selection_sort_arr(test_arr, test_arr_len));
print(quick_sort_arr(test_arr, test_arr_len));


)"""";

const char *testx = R""""(

fun gen_arr (min, max) {

	let mut arr = [];

	for i = min to max {
		arr = arr + i;
	};
	
	return arr;

};

print(gen_arr(0, 100));

)"""";

namespace umber
{

	std::pair<std::shared_ptr<Value>, std::shared_ptr<Error>> umber::run_text(const std::string &text)
	{

		Lexer l("<cin>", std::make_shared<std::string>(text));
		std::pair<std::optional<std::vector<Token>>, std::unique_ptr<Error>> lexer_res = l.make_tokens();

		if (lexer_res.second != nullptr)
		{
			return {nullptr, nullptr};
		}

		std::vector<Token> tokens = lexer_res.first.value();

		Parser p(tokens);
		result::ParseResult parse_res = p.parse();

		if (parse_res.has_error())
		{
			return {nullptr, parse_res.error()};
		}

		std::shared_ptr<SymbolTable> global_symbol_table = std::make_shared<SymbolTable>();

		std::string print_arg_names[] = {"value"};
		std::shared_ptr<values::BuiltInFunction> print_test_function =
			std::make_shared<values::BuiltInFunction>(
				"print",
				std::vector<std::string>(std::begin(print_arg_names), std::end(print_arg_names)),
				[](std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx, values::BuiltInFunction *self) -> result::RuntimeResult
				{
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

		std::string rand_int_arg_names[] = {"from", "to"};
		std::shared_ptr<values::BuiltInFunction> rand_int_test_function =
			std::make_shared<values::BuiltInFunction>(
				"randint",
				std::vector<std::string>(std::begin(rand_int_arg_names), std::end(rand_int_arg_names)),
				[](std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx, values::BuiltInFunction *self) -> result::RuntimeResult
				{
					auto res = result::RuntimeResult();

					std::optional<SymbolTable::symbol> from = exec_ctx->symbol_table()->get("from");
					std::optional<SymbolTable::symbol> to = exec_ctx->symbol_table()->get("to");

					if (!from.has_value() || !to.has_value())
					{
						res.failure(std::make_shared<errors::RuntimeError>(self->pos_start(), self->pos_end(), "error", exec_ctx));
						return res;
					}

					std::shared_ptr<values::NumberValue> from_number = std::dynamic_pointer_cast<values::NumberValue>(from.value().m_value);
					std::shared_ptr<values::NumberValue> to_number = std::dynamic_pointer_cast<values::NumberValue>(to.value().m_value);

					if (from_number == nullptr || to_number == nullptr)
					{
						res.failure(std::make_shared<errors::RuntimeError>(self->pos_start(), self->pos_end(), "'from' and 'to' must be a number!", exec_ctx));
						return res;
					}

					int rand_int = from_number->value() + (std::rand() % static_cast<int>(to_number->value() - from_number->value() + 1));
					res.success(std::make_shared<values::NumberValue>((float)rand_int));
					return res;
				});

		global_symbol_table->declare("print", {print_test_function, false});
		global_symbol_table->declare("randint", {rand_int_test_function, false});

		global_symbol_table->declare("true", {std::make_shared<values::NumberValue>(values::NumberValue::TRUE_VALUE), false});
		global_symbol_table->declare("false", {std::make_shared<values::NumberValue>(values::NumberValue::FALSE_VALUE), false});

		std::shared_ptr<Context> global_context = std::make_shared<Context>("<main>", nullptr, global_symbol_table);

		result::RuntimeResult interpreter_res = Interpreter::visit(parse_res.node(), global_context);

		if (interpreter_res.has_error())
		{
			return {nullptr, interpreter_res.error()};
		}

		return {interpreter_res.value(), nullptr};
	}

}