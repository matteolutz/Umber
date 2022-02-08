#pragma once

#include "../Value.h"
#include "BaseFunctionValue.h"
#include "../Node.h"
#include "../result/RuntimeResult.h"
#include "../errors/RuntimeError.h"

#include "../Interpreter.h"

#include <vector>
#include <memory>

namespace umber
{

	namespace values
	{

		class FunctionValue : public BaseFunctionValue
		{
		private:
			std::shared_ptr<Node> m_body_node;
			std::vector<std::string> m_arg_names;
			bool m_should_auto_return;

		public:
			FunctionValue(std::string name, std::shared_ptr<Node> body_node, std::vector<std::string> arg_names, bool should_auto_return, Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			FunctionValue(std::string name, std::shared_ptr<Node> body_node, std::vector<std::string> arg_names, bool should_auto_return);

			result::RuntimeResult execute(std::vector<std::shared_ptr<Value>> args);

			inline std::unique_ptr<Value> copy() const override { return std::make_unique<FunctionValue>(*this); }

			inline std::string as_string() const override { return utils::std_string_format("<function: '%s'>", this->m_name.c_str()); }

			inline const std::shared_ptr<Node>& body_node() const { return this->m_body_node; }
			inline const std::vector<std::string>& arg_names() const { return this->m_arg_names; }
			inline const bool& should_auto_return() const { return this->m_should_auto_return; }

		};

	}

}