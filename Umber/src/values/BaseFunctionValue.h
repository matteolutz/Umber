#pragma once

#include "../Value.h"
#include "../Context.h"
#include "../SymbolTable.h"
#include "../result/RuntimeResult.h"
#include "../errors/RuntimeError.h"

#include <memory>

namespace umber
{

	namespace values
	{

		class BaseFunctionValue : public Value
		{
		protected:
			std::string m_name;
			std::unique_ptr<Context> generate_new_context() const;

		public:
			BaseFunctionValue(Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			BaseFunctionValue(std::string name, Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			BaseFunctionValue(std::string name = "<anonymous>");
			virtual ~BaseFunctionValue() = 0;

			result::RuntimeResult check_args(std::vector<std::string>& arg_names, std::vector<std::shared_ptr<Value>>& args);
			void populate_args(std::vector<std::string>& arg_names, std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Context> exec_ctx);

			result::RuntimeResult check_and_populate_args(std::vector<std::string>& arg_names, std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Context> exec_ctx);

			virtual result::RuntimeResult execute(std::vector<std::shared_ptr<Value>> args) = 0;

			inline const std::string& name() const { return this->m_name; }
		};

	}

}