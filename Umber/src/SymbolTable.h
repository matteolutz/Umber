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

		bool exists(const std::string& key) const;
		bool exists_rec(const std::string& key) const;

		bool is_mutable(const std::string& key) const;

		std::optional<SymbolTable::symbol> get(const std::string& key) const;
		
		bool assign(const std::string& key, std::shared_ptr<Value> value);
		bool declare(const std::string& key, symbol new_symbol);
		void set(const std::string& key, symbol new_symbol);

		inline void clear() { this->m_symbols.clear(); }

		inline const std::shared_ptr<SymbolTable>& parent() const { return this->m_parent; }

	};

}