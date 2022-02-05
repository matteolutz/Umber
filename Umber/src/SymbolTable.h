#pragma once

#include "Value.h"

#include <map>

namespace umber
{

	class SymbolTable
	{
	public:
		struct symbol
		{
			Value* m_value;
			bool is_mutable;
		};

	private:
		std::map<std::string, symbol> m_symbols;
		SymbolTable* m_parent;

	public:
		SymbolTable(SymbolTable* parent = nullptr);

		bool exists(std::string& key) const;
		bool exists_rec(std::string& key) const;

		bool is_mutable(std::string& key) const;

		std::optional<SymbolTable::symbol> get(std::string& key) const;

	};

}