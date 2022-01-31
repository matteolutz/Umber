#pragma once

#include "../Error.h"

namespace umber
{

	namespace errors
	{

		class ExpectedCharError : public Error
		{

		public:
			ExpectedCharError(Position pos_start, Position pos_end, std::string details);

		};

	}
}

