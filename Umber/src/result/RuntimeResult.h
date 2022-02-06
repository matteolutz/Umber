#pragma once

#include "../Value.fwd.h"
#include "../errors/RuntimeError.h"

namespace umber
{

	namespace result
	{

		class RuntimeResult
		{
		private:
			std::shared_ptr<Value> m_value;
			std::shared_ptr<errors::RuntimeError> m_error;
			
			std::shared_ptr<Value> m_function_return_value;
			bool m_loop_should_continue, m_loop_should_break;

		private:
			void reset();

		public:
			RuntimeResult();
			
			void success(std::shared_ptr<Value> value);
			void success_return(std::shared_ptr<Value> value);
			void success_continue();
			void success_break();

			void failure(std::shared_ptr<errors::RuntimeError> error);

			bool should_return() const;

			inline const std::shared_ptr<Value>& value() const { return this->m_value; }
			inline const std::shared_ptr<errors::RuntimeError>& error() const { return this->m_error; }
			
			inline const std::shared_ptr<Value>& function_return_value() const { return this->m_function_return_value; }

			inline const bool& loop_should_continue() const { return this->m_loop_should_continue; }
			inline const bool& loop_should_break() const { return this->m_loop_should_break; }

		};

	}

}