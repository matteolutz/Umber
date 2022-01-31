#pragma once

#include "../Error.h"

#include <string>

namespace umber
{
	namespace errors
	{
		class IllegalCharacterError : public Error
		{
		public:
			IllegalCharacterError(Position pos_start, Position pos_end, std::string details);
		};
	}
}

