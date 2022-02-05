#include "SymbolTable.h"

namespace umber
{



	SymbolTable::SymbolTable(SymbolTable* parent)
		: m_parent(parent) { }

	bool SymbolTable::exists(std::string& key) const
	{
		return this->m_symbols.count(key);
	}

	bool SymbolTable::exists_rec(std::string& key) const
	{
		if (this->exists(key)) return true;

		if (this->m_parent == nullptr) return false;

		return this->m_parent->exists_rec(key);

	}

	bool SymbolTable::is_mutable(std::string& key) const
	{
		std::optional<symbol> s = this->get(key);
		return s.has_value() ? s.value().is_mutable : false;
	}

	std::optional<SymbolTable::symbol> SymbolTable::get(std::string& key) const
	{
		auto it = this->m_symbols.find(key);
		
		if (it == this->m_symbols.end() && this->m_parent != nullptr)
		{
			return this->m_parent->get(key);
		}
		
		return it->second;
	}

}