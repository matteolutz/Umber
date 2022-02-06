#pragma once

#include "Position.h"

#include <string>

namespace umber
{

	enum class NodeType
	{
		BinOp,
		Break,
		Call,
		Continue,
		For,
		FunctionDef,
		If,
		List,
		Number,
		Return,
		String,
		UnaryOp,
		VarAccess,
		VarAssign,
		VarDeclaration,
		While
	};

	class Node
	{
	private:
		NodeType m_node_type;

	protected:
		Position m_pos_start;
		Position m_pos_end;

		Node(Position pos_start, Position pos_end, NodeType node_type);

	public:
		virtual ~Node() = 0;

		virtual std::string as_string() const;

		const Position& pos_start() const;
		const Position& pos_end() const;

		const NodeType& node_type() const;
	};

}
