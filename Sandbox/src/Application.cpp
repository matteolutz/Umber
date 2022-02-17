#include "Umber.h"

#include <stdio.h>
#include <string>

#include <iostream>

int run(const std::string &text)
{
    std::pair<std::shared_ptr<umber::Value>, std::shared_ptr<umber::Error>> result = umber::run_text(text);

    if (result.second != nullptr)
    {
        printf("\n%s\n", result.second->as_string().c_str());
        return -1;
    }

    if (result.first == nullptr)
    {
        printf("\nError, no value was returned!\n");
        return -1;
    }

    printf("%s\n\n", result.first->as_string().c_str());
    return 0;
}

int main(int argc, char *argv[])
{

    if (argc == 2)
    {
        std::string file_text;

        std::ifstream file_stream(argv[1]);

        while (std::getline(file_stream, file_text))
            ;

        int res = run(file_text);
        return res;
    }

    if (argc == 1)
    {
        while (true)
        {
            printf(">> ");
            std::string text_to_exec;
            std::cin >> text_to_exec;

            int res = run(text_to_exec);
            if (res != 0)
            {
                return res;
            }
        }

        return 0;
    }

    printf("Usage: %s [file_to_run]\n", argv[0]);
}