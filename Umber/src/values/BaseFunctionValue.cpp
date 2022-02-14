#include "BaseFunctionValue.h"

namespace umber
{

	namespace values
	{
		BaseFunctionValue::BaseFunctionValue(Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(ValueType::Function, pos_start, pos_end, context), m_name("<anonymous>")
		{
		}

		BaseFunctionValue::BaseFunctionValue(std::string name, Position pos_start, Position pos_end, std::shared_ptr<Context> context)
			: Value(ValueType::Function, pos_start, pos_end, context), m_name(name)
		{
		}

		BaseFunctionValue::BaseFunctionValue(std::string name)
			: Value(ValueType::Function), m_name(name)
		{
		}

		BaseFunctionValue::~BaseFunctionValue() {}

		std::unique_ptr<Context> BaseFunctionValue::generate_new_context() const
		{
			return this->m_context != nullptr
				? std::make_unique<Context>(this->m_name, this->m_context, std::make_shared<SymbolTable>(this->m_context->symbol_table()))
				: std::make_unique<Context>(this->m_name, std::make_shared<SymbolTable>());
		}

		result::RuntimeResult BaseFunctionValue::check_args(std::vector<std::string>& arg_names, std::vector<std::shared_ptr<Value>>& args)
		{
			auto res = result::RuntimeResult();

			if (args.size() > arg_names.size())
			{
				res.failure(std::make_shared<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, utils::std_string_format("%d too many args passed to '%s'!", args.size() - arg_names.size(), this->m_name.c_str()), this->m_context));
				return res;
			}

			if (args.size() < arg_names.size())
			{
				res.failure(std::make_shared<errors::RuntimeError>(this->m_pos_start, this->m_pos_end, utils::std_string_format("%d too few args passed to '%s'!", arg_names.size() - args.size(), this->m_name.c_str()), this->m_context));
				return res;
			}

			return res;
		}

		void BaseFunctionValue::populate_args(std::vector<std::string>& arg_names, std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Context> exec_ctx)
		{
			if (arg_names.size() != args.size()) return;

			for (int i = 0; i < arg_names.size(); i++)
			{
				std::string arg_name = arg_names[i];

				std::shared_ptr<Value> arg_value = args[i]->copy();
				arg_value->context() = exec_ctx;

				exec_ctx->symbol_table()->set(arg_name, { arg_value, false });
			}
		}

		result::RuntimeResult BaseFunctionValue::check_and_populate_args(std::vector<std::string>& arg_names, std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Context> exec_ctx)
		{
			auto res = result::RuntimeResult();

			res.register_res(this->check_args(arg_names, args));
			if (res.should_return())
			{
				return res;
			}

			this->populate_args(arg_names, args, exec_ctx);

			return res;
		}

	}

}