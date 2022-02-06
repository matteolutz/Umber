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
		std::shared_ptr<std::string> m_filetext;

	public:
		Position(unsigned int index, unsigned int line, unsigned int col, std::string filename, std::shared_ptr<std::string> filetext);
		Position(std::string filename, std::shared_ptr<std::string> filetext);
		Position();

		inline const unsigned int& index() const { return this->m_index; }
		inline const unsigned int& line() const { return this->m_line; }
		inline const unsigned int& col() const { return this->m_col; }

		inline const std::string& filename() const { return this->m_filename; }
		const std::shared_ptr<std::string>& filetext() const { return this->m_filetext; }

		void advance(std::optional<char> current_char = std::nullopt);
	};

}