#pragma once

#include "Position.h"
#include "Stringable.h"

#include <string>
#include <iostream>
#include <format>

namespace umber
{
	class Error : public Stringable
	{
	protected:
		Position m_pos_start;
		Position m_pos_end;
		std::string m_error_name;
		std::string m_details;

	protected:
		Error(Position pos_start, Position pos_end, std::string error_name, std::string details);

	public:
		std::string as_string() const override;

	};
}
