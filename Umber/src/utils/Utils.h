#pragma once

#include "../Position.h"

#include <stdio.h>
#include <cstdarg>

#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <sstream>

namespace umber
{

	namespace utils
	{

		std::string std_string_format(const char* fmt, ...);
		std::vector<std::string> std_string_split(char delimiter, std::string& from);
		std::string umber_error_description(std::string& filetext, Position pos_start, Position pos_end);

	}

}