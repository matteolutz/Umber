#include "BuiltInFunction.h"

namespace umber
{

	namespace values
	{


		BuiltInFunction::BuiltInFunction(std::string name, std::vector<std::string> arg_names, execute_builtin_function executor)
			: BaseFunctionValue(name), m_arg_names(arg_names), m_executor(executor)
		{
		}

		result::RuntimeResult BuiltInFunction::execute(std::vector<std::shared_ptr<Value>> args)
		{
			auto res = result::RuntimeResult();

			std::shared_ptr<Context> exec_ctx = this->generate_new_context();

			res.register_res(this->check_and_populate_args(this->m_arg_names, args, exec_ctx));
			if (res.should_return())
			{
				return res;
			}

			res.success(res.register_res(m_executor(args, exec_ctx, this)));
			return res;
		}


	}

}