#include "Umber.h"

#include <iostream>
#include <chrono>

const char* lexer_test = R"""(
fun split_lines (content) {
    var lines = [];
    var current_line = "";

    for i = 0 to len(content) {
        var current_char = content[i];
        # print(current_char);
        if current_char == "\n" {
            list_append(lines, current_line);
            var current_line = "";
        } else {
            var current_line = current_line + content[i];
        };
    };
    list_append(lines, current_line);
    
    return lines;
};

file_read();
)""""";

int main()
{
	// umber::Run("C:\\Users\\Matteo\\Desktop\\test.txt");

	/*test();
	return -1;*/

	umber::Test();
	return 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::tuple<std::optional<std::vector<umber::Token>>, std::optional<umber::Error>> result = umber::RunText(lexer_test);

	auto end = std::chrono::high_resolution_clock::now();

	printf("Execution took: %f3.4ms\n\n", std::chrono::duration<float, std::milli>(end - start).count());

	if (std::get<1>(result).has_value())
	{
		printf("Failed: %s\n", std::get<1>(result).value().as_string().c_str());
		return -1;
	}

	printf("Success, length: %d\n", std::get<0>(result).value().size());
	for (const umber::Token& t : std::get<0>(result).value())
	{
		printf("%d: %s\n", t.type(), "TODO!");
	}

	return 0;

}