#include "Position.h"

namespace umber
{
	Position::Position(unsigned int index, unsigned int line, unsigned int col, std::string filename, std::string &filetext)
		: m_index(index), m_line(line), m_col(col), m_filename(filename), m_filetext(filetext) {}

	Position::Position(std::string filename, std::string &filetext)
		: m_index(0), m_line(0), m_col(0), m_filename(filename), m_filetext(filetext) { }

	void Position::advance(std::optional<char> current_char)
	{
		this->m_index++;
		this->m_col++;

		if (current_char.has_value() && current_char.value() == '\n')
		{
			this->m_line++;
			this->m_col = 0;
		}
	}

	Position Position::copy_advance(std::optional<char> current_char)
	{
		Position copy = *this;
		copy.advance(current_char);
		return copy;
	}

	const unsigned int& Position::index() const
	{
		return m_index;
	}


	const unsigned int& Position::line() const
	{
		return m_line;
	}


	const unsigned int& Position::col() const
	{
		return m_col;
	}

	const std::string& Position::filename() const
	{
		return this->m_filename;
	}


	const std::string& Position::filetext() const
	{
		return this->m_filetext;
	}

}