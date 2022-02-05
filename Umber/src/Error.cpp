#include "Error.h"

namespace umber
{

	Error::Error(Position pos_start, Position pos_end, std::string error_name, std::string details)
		: m_pos_start(pos_start), m_pos_end(pos_end), m_error_name(error_name), m_details(details)
	{ }

	Error::~Error() {}

	std::string Error::as_string() const
	{
		/*char buffer[200];
		sprintf_s(buffer, "%s: %s\n  File '%s', line %d", this->m_error_name.c_str(), this->m_details.c_str(), this->m_pos_start.filename().c_str(), this->m_pos_start.line() + 1);
		return buffer;*/
		return this->m_details;
	}

}