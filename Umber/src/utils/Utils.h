#pragma once

#include <stdio.h>
#include <cstdarg>

#include <string>
#include <memory>
#include <stdexcept>

namespace umber
{

	namespace utils
	{

		std::string std_string_format(const char* fmt, ...);

	}

}