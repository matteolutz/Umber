#include "IllegalCharacterError.h"

namespace umber
{

	namespace errors
	{

		IllegalCharacterError::IllegalCharacterError(Position pos_start, Position pos_end, std::string details)
			: Error(pos_start, pos_end, "IllegalCharacterError", details)
		{}

	}

}