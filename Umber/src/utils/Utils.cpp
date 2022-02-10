#include "Utils.h"

namespace umber
{

	namespace utils
	{

		std::string std_string_format(const char* fmt, ...)
		{
			char buf[256];

			va_list args;
			va_start(args, fmt);
			const auto r = std::vsnprintf(buf, sizeof buf, fmt, args);
			va_end(args);

			if (r < 0)
				// conversion failed
				return {};

			const size_t len = r;
			if (len < sizeof buf)
				// we fit in the buffer
				return { buf, len };

#if __cplusplus >= 201703L
			// C++17: Create a string and write to its underlying array
			std::string s(len, '\0');
			va_start(args, fmt);
			std::vsnprintf(s.data(), len + 1, fmt, args);
			va_end(args);

			return s;
#else
			// C++11 or C++14: We need to allocate scratch memory
			auto vbuf = std::unique_ptr<char[]>(new char[len + 1]);
			va_start(args, fmt);
			std::vsnprintf(vbuf.get(), len + 1, fmt, args);
			va_end(args);

			return { vbuf.get(), len };
#endif
		}

		std::vector<std::string> std_string_split(char delimiter, std::string& from)
		{
			std::istringstream stream(from);

			std::vector<std::string> lines;
			std::string current_line;

			while (std::getline(stream, current_line))
			{
				lines.emplace_back(current_line);
			}

			return lines;
		}

		std::string umber_error_description(std::string& filetext, Position pos_start, Position pos_end)
		{
			std::string buf;
			std::vector<std::string> lines = std_string_split('\n', filetext);

			for (int i = pos_start.line(); i <= pos_end.line(); i++)
			{

				if (i >= lines.size()) break;

				int col_start = i == pos_start.line() ? pos_start.col() : 0;
				int col_end = i == pos_end.line() ? pos_end.col() : lines[i].length();

				buf.append(std_string_format("%d.\t%s\n", i + 1, lines[i].c_str()));

				buf.append(std_string_format("%d.\t", i + 1));
				for (int i = 0; i < col_start; i++) buf.push_back(' ');

				for (int i = col_start; i < col_end; i++) buf.push_back('~');

				buf.append("\n\n");
			}

			return buf;
		}

	}

}