#pragma once

#include "Position.h"
#include "Stringable.h"

#include <string>

namespace umber
{

	class Node : public Stringable
	{
	protected:
		const Position m_pos_start;
		const Position m_pos_end;

		Node(Position pos_start, Position pos_end);

	public:
		virtual std::string as_string() const override;

		const Position& pos_start() const;
		const Position& pos_end() const;
	};

}
