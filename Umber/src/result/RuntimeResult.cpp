#include "RuntimeResult.h"

namespace umber
{

	namespace result
	{
		RuntimeResult::RuntimeResult()
			: m_value(nullptr), m_error(nullptr), m_function_return_value(nullptr), m_loop_should_continue(false), m_loop_should_break(false)
		{
		}

		void RuntimeResult::reset()
		{
			this->m_value = nullptr;
			this->m_error = nullptr;
			this->m_function_return_value = nullptr;
			this->m_loop_should_continue = false;
			this->m_loop_should_break = false;
		}

		void RuntimeResult::success(Value* value)
		{
			this->reset();
			this->m_value = value;
		}

		void RuntimeResult::success_return(Value* value)
		{
			this->reset();
			this->m_function_return_value = value;
		}

		void RuntimeResult::success_continue()
		{
			this->reset();
			this->m_loop_should_continue = true;
		}

		void RuntimeResult::success_break()
		{
			this->reset();
			this->m_loop_should_break = true;
		}

		void RuntimeResult::failure(errors::RuntimeError* error)
		{
			this->reset();
			this->m_error = error;
		}

		bool RuntimeResult::should_return() const
		{
			return this->m_error != nullptr || this->m_function_return_value != nullptr || this->m_loop_should_continue || this->m_loop_should_break;
		}
	}

}