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

	umber::Test();
	return 0;

}