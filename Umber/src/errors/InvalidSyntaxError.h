#pragma once

#include "../Error.h"
#include "../Position.h"

#include <string>

namespace umber
{

	namespace errors
	{

		class InvalidSyntaxError : public Error
		{
		public:
			InvalidSyntaxError(Position pos_start, Position pos_end, std::string details);
		};

	}
	
}