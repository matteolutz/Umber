#pragma once

#include "../Value.h"
#include "../errors/RuntimeError.h"

namespace umber
{

	namespace result
	{

		class RuntimeResult
		{
		private:
			Value* m_value;
			errors::RuntimeError* m_error;
			
			Value* m_function_return_value;
			bool m_loop_should_continue, m_loop_should_break;

		private:
			void reset();

		public:
			RuntimeResult();
			
			void success(Value* value);
			void success_return(Value* value);
			void success_continue();
			void success_break();

			void failure(errors::RuntimeError* error);

			bool should_return() const;

			inline const Value* value() const { return this->m_value; }
			inline const errors::RuntimeError* error() const { return this->m_error; }
			
			inline const Value* function_return_value() const { return this->m_function_return_value; }

			inline const bool& loop_should_continue() const { return this->m_loop_should_continue; }
			inline const bool& loop_should_break() const { return this->m_loop_should_break; }

		};

	}

}