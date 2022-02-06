#pragma once

#include "../Error.h"
#include "../Context.fwd.h"

namespace umber
{

	namespace errors
	{

		class RuntimeError : public Error
		{
		private:
			std::shared_ptr<Context> m_context;

		public:
			RuntimeError(Position pos_start, Position pos_end, std::string details, std::shared_ptr<Context> context);

			inline const std::shared_ptr<Context>& context() const { return this->m_context; }

		};

	}
}

