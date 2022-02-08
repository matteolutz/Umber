#pragma once

#include "Position.h"
#include "utils/Utils.h"

#include <string>

namespace umber
{
	class Error
	{
	protected:
		Position m_pos_start, m_pos_end;
		std::string m_error_name, m_details;

	protected:
		Error(Position pos_start, Position pos_end, std::string error_name, std::string details);

	public:
		virtual ~Error() = 0;

		virtual std::string as_string() const;

		inline const std::string& name() const { return this->m_error_name; }

		inline const Position& pos_start() const { return this->m_pos_start; }
		inline const Position& pos_end() const { return this->m_pos_end; }

	};
}
