#pragma once

#include <string>
#include <memory>
#include <stdexcept>

namespace umber
{

	namespace utils
	{

		template<typename ...Args>
		std::string string_format(const std::string& format, Args ...args);

	}

}