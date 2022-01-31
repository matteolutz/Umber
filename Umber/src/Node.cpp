#include "Node.h"

namespace umber
{

	Node::Node(Position pos_start, Position pos_end)
		: m_pos_start(pos_start), m_pos_end(pos_end) { }

	std::string Node::as_string() const
	{
		return "{Node-as_string()-notYetImplemented}";
	}

	const Position& Node::pos_start() const
	{
		return this->m_pos_start;
	}

	const Position& Node::pos_end() const
	{
		return this->m_pos_end;
	}

}