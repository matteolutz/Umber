#include "InvalidSyntaxError.h"

namespace umber
{

	namespace errors
	{

		InvalidSyntaxError::InvalidSyntaxError(Position pos_start, Position pos_end, std::string details)
			: Error(pos_start, pos_end, "InvalidSyntaxError", details) { }

	}

}