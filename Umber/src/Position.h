#pragma once

#include <string>
#include <memory>
#include <optional>

namespace umber
{

	class Position
	{
	private:
		unsigned int m_index;
		unsigned int m_line;
		unsigned int m_col;

		std::string m_filename;
		std::string& m_filetext;

	public:
		Position(unsigned int index, unsigned int line, unsigned int col, std::string filename, std::string &filetext);
		Position(std::string filename, std::string &filetext);

		const unsigned int& index() const;
		const unsigned int& line() const;
		const unsigned int& col() const;

		const std::string& filename() const;
		const std::string& filetext() const;

		void advance(std::optional<char> current_char = std::nullopt);
		Position copy_advance(std::optional<char> current_char = std::nullopt);
	};

}