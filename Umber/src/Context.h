#pragma once

#include "Position.h"
#include "SymbolTable.h"

#include <string>
#include <optional>

namespace umber
{

	class Context
	{
	private:
		std::string m_display_name;

		Context* m_parent;
		std::optional<Position> m_parent_entry_pos;

		SymbolTable m_symbol_table;

	public:
		Context(std::string display_name, Context* parent, SymbolTable symbol_table);
		Context(std::string display_name, SymbolTable symbol_table);

		inline const std::string& display_name() const { return this->m_display_name; }
		inline const Context* parent() const { return this->m_parent; }
		inline const std::optional<Position>& parent_entry_pos() const { return this->m_parent_entry_pos; }
		inline const SymbolTable& symbol_table() const { return this->m_symbol_table; }

	};

}