#include "Node.h"

namespace umber
{

	Node::Node(Position pos_start, Position pos_end, NodeType node_type)
		: m_pos_start(pos_start), m_pos_end(pos_end), m_node_type(node_type) { }

	Node::~Node() {}

	std::string Node::as_string() const
	{
		return "NotYetImplemented";
	}

}