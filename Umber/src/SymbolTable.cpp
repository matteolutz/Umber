#include "SymbolTable.h"

namespace umber
{



	SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent)
		: m_parent(parent) { }

	bool SymbolTable::exists(std::string& key) const
	{
		return this->m_symbols.count(key);
	}

	std::pair<bool, std::shared_ptr<SymbolTable>> SymbolTable::exists_rec(std::string& key) const
	{
		if (this->exists(key)) return { true, std::make_shared<SymbolTable>(*this) };

		if (this->m_parent == nullptr) return { false, nullptr };

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
		else if (it != this->m_symbols.end())
		{
			return it->second;
		}
		
		return std::nullopt;
	}

	bool SymbolTable::assign(std::string& key, std::shared_ptr<Value> value)
	{
		std::pair<bool, std::shared_ptr<SymbolTable>> exists = this->exists_rec(key);
		if (exists.first)
		{
			if (!exists.second->get(key).value().is_mutable)
			{
				return false;
			}

			exists.second->m_symbols[key] = { value, false };
			return true;
		}
		
		return false;

	}

	bool SymbolTable::declare(std::string& key, symbol new_symbol)
	{
		std::pair<bool, std::shared_ptr<SymbolTable>> exists = this->exists_rec(key);

		if (exists.first) return false;

		this->set(key, new_symbol);
	}

	void SymbolTable::set(std::string& key, symbol new_symbol)
	{
		this->m_symbols[key] = new_symbol;
	}

}