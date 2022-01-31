#pragma once

#include <string>

namespace umber
{

	class Stringable
	{
	public:
		inline virtual std::string as_string() const { return "Stringable-NotYetImplemented"; }
	};

}