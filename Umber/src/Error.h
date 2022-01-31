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
		const Position m_pos_start;
		const Position m_pos_end;
		const std::string m_error_name;
		const std::string m_details;

	protected:
		Error(Position pos_start, Position pos_end, std::string error_name, std::string details);

	public:
		std::string as_string() const override;

	};
}
