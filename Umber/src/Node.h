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
		Accessor,
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

		inline const Position& pos_start() const { return this->m_pos_start; }
		inline const Position& pos_end() const { return this->m_pos_end; }

		inline const NodeType& node_type() const { return this->m_node_type; }
	};

}
