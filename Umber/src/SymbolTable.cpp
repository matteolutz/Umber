#include "SymbolTable.h"

namespace umber
{



	SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent)
		: m_parent(parent) { }

	bool SymbolTable::exists(const std::string& key) const
	{
		return this->m_symbols.count(key);
	}

	bool SymbolTable::exists_rec(const std::string& key) const
	{
		if (this->exists(key)) return true;

		if (this->m_parent == nullptr) return false;

		return this->m_parent->exists_rec(key);

	}

	bool SymbolTable::is_mutable(const std::string& key) const
	{
		std::optional<symbol> s = this->get(key);
		return s.has_value() ? s.value().is_mutable : false;
	}

	std::optional<SymbolTable::symbol> SymbolTable::get(const std::string& key) const
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

	bool SymbolTable::assign(const std::string& key, std::shared_ptr<Value> value)
	{

		if (!this->exists(key))
		{
			if (this->m_parent != nullptr) return this->m_parent->assign(key, value);
			return false;
		}

		if (!this->get(key).value().is_mutable) return false;

		this->set(key, { value, true });
		return true;

	}

	bool SymbolTable::declare(const std::string& key, symbol new_symbol)
	{
		// std::pair<bool, std::shared_ptr<SymbolTable>> exists = this->exists_rec(key);

		if (this->exists_rec(key))
		{
			return false;
		}
		
		// if (exists.first) return false;

		this->set(key, new_symbol);
	}

	void SymbolTable::set(const std::string& key, symbol new_symbol)
	{
		this->m_symbols[key] = new_symbol;
	}

}