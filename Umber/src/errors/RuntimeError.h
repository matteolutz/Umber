#pragma once

#include "../Error.h"
#include "../Context.h"

namespace umber
{

	namespace errors
	{

		class RuntimeError : public Error
		{
		private:
			Context* m_context;

		public:
			RuntimeError(Position pos_start, Position pos_end, std::string details, Context* context);

			inline const Context* context() const { return this->m_context; }

		};

	}
}

