#pragma once

#include "Value.h"
#include "Node.h"

#include "nodes/ReturnNode.h"
#include "nodes/ContinueNode.h"
#include "nodes/BreakNode.h"
#include "nodes/ListNode.h"
#include "nodes/UnaryOpNode.h"
#include "nodes/BinOpNode.h"
#include "nodes/VarDeclarationNode.h"
#include "nodes/CallNode.h"
#include "nodes/NumberNode.h"
#include "nodes/StringNode.h"
#include "nodes/VarAccessNode.h"
#include "nodes/VarAssignNode.h"
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"
#include "nodes/WhileNode.h"
#include "nodes/ListNode.h"
#include "nodes/FunctionDefNode.h"

#include "values/NumberValue.h"
#include "values/StringValue.h"
#include "values/ListValue.h"
#include "values/FunctionValue.h"

#include "Context.h"
#include "SymbolTable.h"

#include "utils/Utils.h"

#include <memory>
#include <stdexcept>
#include <iostream>

namespace umber
{
	
	class Interpreter
	{
	public:
		static result::RuntimeResult visit(std::shared_ptr<Node> node, std::shared_ptr<Context> context);

	private:
		static result::RuntimeResult visit_number_node(std::shared_ptr<nodes::NumberNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_string_node(std::shared_ptr<nodes::StringNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_list_node(std::shared_ptr<nodes::ListNode> node, std::shared_ptr<Context> context);

		static result::RuntimeResult visit_var_access_node(std::shared_ptr<nodes::VarAccessNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_var_assign_node(std::shared_ptr<nodes::VarAssignNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_var_declaration_node(std::shared_ptr<nodes::VarDeclarationNode> node, std::shared_ptr<Context> context);

		static result::RuntimeResult visit_bin_op_node(std::shared_ptr<nodes::BinOpNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_unary_op_node(std::shared_ptr<nodes::UnaryOpNode> node, std::shared_ptr<Context> context);

		static result::RuntimeResult visit_if_node(std::shared_ptr<nodes::IfNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_for_node(std::shared_ptr<nodes::ForNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_while_node(std::shared_ptr<nodes::WhileNode> node, std::shared_ptr<Context> context);

		static result::RuntimeResult visit_function_def_node(std::shared_ptr<nodes::FunctionDefNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_call_node(std::shared_ptr<nodes::CallNode> node, std::shared_ptr<Context> context);

		static result::RuntimeResult visit_return_node(std::shared_ptr<nodes::ReturnNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_continue_node(std::shared_ptr<nodes::ContinueNode> node, std::shared_ptr<Context> context);
		static result::RuntimeResult visit_break_node(std::shared_ptr<nodes::BreakNode> node, std::shared_ptr<Context> context);

	};

}