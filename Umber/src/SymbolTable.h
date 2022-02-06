#pragma once

#include "Value.fwd.h"

#include <map>
#include <memory>
#include <string>
#include <optional>

namespace umber
{

	class SymbolTable
	{
	public:
		struct symbol
		{
			std::shared_ptr<Value> m_value;
			bool is_mutable;
		};

	private:
		std::map<std::string, symbol> m_symbols;
		std::shared_ptr<SymbolTable> m_parent;

	public:
		SymbolTable(std::shared_ptr<SymbolTable> parent = nullptr);

		bool exists(std::string& key) const;
		bool exists_rec(std::string& key) const;

		bool is_mutable(std::string& key) const;

		std::optional<SymbolTable::symbol> get(std::string& key) const;

	};

}