#pragma once

#include "BaseFunctionValue.h"
#include "NumberValue.h"

#include <vector>

namespace umber
{

	namespace values
	{

		class BuiltInFunction : public BaseFunctionValue
		{
		public:
			typedef result::RuntimeResult(*execute_builtin_function)(std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx, BuiltInFunction* self);

		private:
			std::vector<std::string> m_arg_names;
			execute_builtin_function m_executor;

		public:
			BuiltInFunction(std::string name, std::vector<std::string> arg_names, execute_builtin_function executor);

			result::RuntimeResult execute(std::vector<std::shared_ptr<Value>> args) override;

			inline std::unique_ptr<Value> copy() const override { return std::make_unique<BuiltInFunction>(*this); }
			inline std::string as_string() const override { return utils::std_string_format("<built-in function: '%s'>", this->m_name.c_str()); }

			inline const std::vector<std::string>& arg_names() const { return this->m_arg_names; }

		};

	}

}