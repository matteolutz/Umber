#pragma once

#include "Position.h"

#include <string>

namespace umber
{

	class Node
	{
	protected:
		Position m_pos_start;
		Position m_pos_end;

		Node(Position pos_start, Position pos_end);

	public:
		virtual std::string as_string() const;

		const Position& pos_start() const;
		const Position& pos_end() const;
	};

}
