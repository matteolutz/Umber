#include "RuntimeError.h"

namespace umber
{

	namespace errors
	{

		RuntimeError::RuntimeError(Position pos_start, Position pos_end, std::string details, std::shared_ptr<Context> context)
			: Error(pos_start, pos_end, "RuntimeError", details), m_context(context)
		{
		}

	}

}