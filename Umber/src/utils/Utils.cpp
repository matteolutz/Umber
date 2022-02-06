#include "Utils.h"

namespace umber
{

	namespace utils
	{

		template<typename ...Args>
		std::string string_format(const std::string& format, Args ...args)
		{
			int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
			if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
			auto size = static_cast<size_t>(size_s);
			auto buf = std::make_unique<char[]>(size);
			std::snprintf(buf.get(), size, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size - 1);
		}

	}

}