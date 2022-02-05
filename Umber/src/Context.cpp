#include "Context.h"

namespace umber
{



	Context::Context(std::string display_name, Context* parent, SymbolTable symbol_table)
		: m_display_name(display_name), m_parent(parent), m_symbol_table(symbol_table)
	{
	}

	Context::Context(std::string display_name, SymbolTable symbol_table)
		: m_display_name(display_name), m_parent(nullptr), m_symbol_table(symbol_table)
	{
	}

}