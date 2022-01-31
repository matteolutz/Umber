#include "ExpectedCharError.h"

namespace umber
{

	namespace errors
	{

		ExpectedCharError::ExpectedCharError(Position pos_start, Position pos_end, std::string details)
			: Error(pos_start, pos_end, "ExpectedCharError", details) { }

	}

}