#include "Position.h"

namespace umber
{
	Position::Position(unsigned int index, unsigned int line, unsigned int col, std::string filename, std::shared_ptr<std::string> filetext)
		: m_index(index), m_line(line), m_col(col), m_filename(filename), m_filetext(filetext) {}

	Position::Position(std::string filename, std::shared_ptr<std::string> filetext)
		: m_index(0), m_line(0), m_col(0), m_filename(filename), m_filetext(filetext) { }

	Position::Position()
		: m_index(0), m_line(0), m_col(0), m_filename(""), m_filetext(std::make_shared<std::string>()) { }

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

}