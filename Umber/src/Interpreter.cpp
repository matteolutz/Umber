#include "Interpreter.h"

namespace umber
{

	result::RuntimeResult Interpreter::visit(std::shared_ptr<Node> node, std::shared_ptr<Context> context)
	{
		switch (node->node_type())
		{
		case NodeType::Number: return Interpreter::visit_number_node(std::make_shared<nodes::NumberNode>(dynamic_cast<nodes::NumberNode*>(node.get())), context);
		case NodeType::String: return Interpreter::visit_string_node(std::make_shared<nodes::StringNode>(dynamic_cast<nodes::StringNode*>(node.get())), context);
		case NodeType::List: return Interpreter::visit_list_node(std::make_shared<nodes::ListNode>(dynamic_cast<nodes::ListNode*>(node.get())), context);

		case NodeType::VarAccess: return Interpreter::visit_var_access_node(std::make_shared<nodes::VarAccessNode>(dynamic_cast<nodes::VarAccessNode*>(node.get())), context);
		case NodeType::VarAssign: return Interpreter::visit_var_assign_node(std::make_shared<nodes::VarAssignNode>(dynamic_cast<nodes::VarAssignNode*>(node.get())), context);
		case NodeType::VarDeclaration: return Interpreter::visit_var_declaration_node(std::make_shared<nodes::VarDeclarationNode>(dynamic_cast<nodes::VarDeclarationNode*>(node.get())), context);

		case NodeType::BinOp: return Interpreter::visit_bin_op_node(std::make_shared<nodes::BinOpNode>(dynamic_cast<nodes::BinOpNode*>(node.get())), context);
		case NodeType::UnaryOp: return Interpreter::visit_unary_op_node(std::make_shared<nodes::UnaryOpNode>(dynamic_cast<nodes::UnaryOpNode*>(node.get())), context);

		case NodeType::If: return Interpreter::visit_if_node(std::make_shared<nodes::IfNode>(dynamic_cast<nodes::IfNode*>(node.get())), context);
		case NodeType::For: return Interpreter::visit_for_node(std::make_shared<nodes::ForNode>(dynamic_cast<nodes::ForNode*>(node.get())), context);
		case NodeType::While: return Interpreter::visit_while_node(std::make_shared<nodes::WhileNode>(dynamic_cast<nodes::WhileNode*>(node.get())), context);

		case NodeType::FunctionDef: return Interpreter::visit_function_def_node(std::make_shared<nodes::FunctionDefNode>(dynamic_cast<nodes::FunctionDefNode*>(node.get())), context);
		case NodeType::Call: return Interpreter::visit_call_node(std::make_shared<nodes::CallNode>(dynamic_cast<nodes::CallNode*>(node.get())), context);

		case NodeType::Return: return Interpreter::visit_return_node(std::make_shared<nodes::ReturnNode>(dynamic_cast<nodes::ReturnNode*>(node.get())), context);
		case NodeType::Continue: return Interpreter::visit_continue_node(std::make_shared<nodes::ContinueNode>(dynamic_cast<nodes::ContinueNode*>(node.get())), context);
		case NodeType::Break: return Interpreter::visit_break_node(std::make_shared<nodes::BreakNode>(dynamic_cast<nodes::BreakNode*>(node.get())), context);

		default:
			throw std::invalid_argument("NoVisitFunctionDefined");
			break;
		}
	}

	result::RuntimeResult Interpreter::visit_number_node(std::shared_ptr<nodes::NumberNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();
		res.success(std::make_shared<values::NumberValue>(std::stof(node->token().value_or_zero()), node->pos_start(), node->pos_end(), context));
		return res;
	}

	result::RuntimeResult Interpreter::visit_string_node(std::shared_ptr<nodes::StringNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();
		res.success(std::make_shared<values::StringValue>(node->token().value().value_or(""), node->pos_start(), node->pos_end(), context));
		return res;
	}

}