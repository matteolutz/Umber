#include "FunctionValue.h"

namespace umber
{

	namespace values
	{

		

		FunctionValue::FunctionValue(std::string name, std::shared_ptr<Node> body_node, std::vector<std::string> arg_names, bool should_auto_return, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: BaseFunctionValue(name, pos_start, pos_end, context), m_body_node(body_node), m_arg_names(arg_names), m_should_auto_return(should_auto_return)
		{
		}

		FunctionValue::FunctionValue(std::string name, std::shared_ptr<Node> body_node, std::vector<std::string> arg_names, bool should_auto_return)
			: BaseFunctionValue(name), m_body_node(body_node), m_arg_names(arg_names), m_should_auto_return(should_auto_return)
		{
		}

		result::RuntimeResult FunctionValue::execute(std::vector<std::shared_ptr<Value>> args)
		{
			auto res = result::RuntimeResult();

			std::shared_ptr<Context> exec_ctx = this->generate_new_context();

			res.register_res(this->check_and_populate_args(this->m_arg_names, args, exec_ctx));
			if (res.should_return())
			{
				return res;
			}

			std::shared_ptr<Value> value = res.register_res(Interpreter::visit(this->m_body_node, exec_ctx));
			if (res.should_return() && res.function_return_value() == nullptr)
			{
				return res;
			}

			std::shared_ptr<Value> return_value = std::make_shared<NumberValue>(NumberValue::NULL_VALUE);
			if (this->m_should_auto_return)
			{
				return_value = value;
			}
			else if(res.function_return_value() != nullptr)
			{
				return_value = res.function_return_value();
			}
			
			res.success(return_value);
			return res;
		}

	}

}