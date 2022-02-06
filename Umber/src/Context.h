#pragma once

#include "Position.h"
#include "SymbolTable.fwd.h"

#include <string>
#include <optional>
#include <memory>

namespace umber
{

	class Context
	{
	private:
		std::string m_display_name;

		std::shared_ptr<Context> m_parent;
		std::optional<Position> m_parent_entry_pos;

		std::shared_ptr<SymbolTable> m_symbol_table;

	public:
		Context(std::string display_name, std::shared_ptr<Context> parent, std::shared_ptr<SymbolTable> symbol_table);
		Context(std::string display_name, std::shared_ptr<SymbolTable> symbol_table);

		inline const std::string& display_name() const { return this->m_display_name; }
		inline const std::shared_ptr<Context>& parent() const { return this->m_parent; }
		inline const std::optional<Position>& parent_entry_pos() const { return this->m_parent_entry_pos; }
		inline const std::shared_ptr<SymbolTable>& symbol_table() const { return this->m_symbol_table; }
		

	};

}