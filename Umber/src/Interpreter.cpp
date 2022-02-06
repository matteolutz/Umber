#include "Interpreter.h"

namespace umber
{

	result::RuntimeResult Interpreter::visit(std::shared_ptr<Node> node, std::shared_ptr<Context> context)
	{
		switch (node->node_type())
		{
		case NodeType::Number: return Interpreter::visit_number_node(std::static_pointer_cast<nodes::NumberNode>(node), context);
		case NodeType::String: return Interpreter::visit_string_node(std::static_pointer_cast<nodes::StringNode>(node), context);
		case NodeType::List: return Interpreter::visit_list_node(std::static_pointer_cast<nodes::ListNode>(node), context);

		case NodeType::VarAccess: return Interpreter::visit_var_access_node(std::static_pointer_cast<nodes::VarAccessNode>(node), context);
		case NodeType::VarAssign: return Interpreter::visit_var_assign_node(std::static_pointer_cast<nodes::VarAssignNode>(node), context);
		case NodeType::VarDeclaration: return Interpreter::visit_var_declaration_node(std::static_pointer_cast<nodes::VarDeclarationNode>(node), context);

		case NodeType::BinOp: return Interpreter::visit_bin_op_node(std::static_pointer_cast<nodes::BinOpNode>(node), context);
		case NodeType::UnaryOp: return Interpreter::visit_unary_op_node(std::static_pointer_cast<nodes::UnaryOpNode>(node), context);

		case NodeType::If: return Interpreter::visit_if_node(std::static_pointer_cast<nodes::IfNode>(node), context);
		case NodeType::For: return Interpreter::visit_for_node(std::static_pointer_cast<nodes::ForNode>(node), context);
		case NodeType::While: return Interpreter::visit_while_node(std::static_pointer_cast<nodes::WhileNode>(node), context);

		case NodeType::FunctionDef: return Interpreter::visit_function_def_node(std::static_pointer_cast<nodes::FunctionDefNode>(node), context);
		case NodeType::Call: return Interpreter::visit_call_node(std::static_pointer_cast<nodes::CallNode>(node), context);

		case NodeType::Return: return Interpreter::visit_return_node(std::static_pointer_cast<nodes::ReturnNode>(node), context);
		case NodeType::Continue: return Interpreter::visit_continue_node(std::static_pointer_cast<nodes::ContinueNode>(node), context);
		case NodeType::Break: return Interpreter::visit_break_node(std::static_pointer_cast<nodes::BreakNode>(node), context);

		default:
			throw std::invalid_argument("NoVisitFunctionDefined");
			break;
		}
	}

#pragma region Visit methods

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

	result::RuntimeResult Interpreter::visit_list_node(std::shared_ptr<nodes::ListNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();
		std::vector<std::shared_ptr<Value>> elements;

		for (const std::shared_ptr<Node>& n : node->elements())
		{
			elements.emplace_back(res.register_res(Interpreter::visit(n, context)));
			if (res.should_return())
			{
				return res;
			}
		}

		res.success(std::make_shared<values::ListValue>(elements, node->pos_start(), node->pos_end(), context));
		return res;
	}

	result::RuntimeResult Interpreter::visit_var_access_node(std::shared_ptr<nodes::VarAccessNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::string var_name = node->var_name_token().value().value_or("");
		if (var_name == "")
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable name expected!", context));
			return res;
		}

		std::optional<SymbolTable::symbol> value = context->symbol_table()->get(var_name);
		if (!value.has_value())
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), utils::string_format("'%s' is not declared in this scope!", var_name.c_str()), context));
			return res;
		}
		
		res.success(value.value().m_value);
		return res;
	}

	result::RuntimeResult Interpreter::visit_var_assign_node(std::shared_ptr<nodes::VarAssignNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::string var_name = node->var_name_token().value().value_or("");
		if (var_name == "")
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable name expected!", context));
			return res;
		}

		std::shared_ptr<Value> new_value = res.register_res(Interpreter::visit(node->value_node(), context));

		if (res.should_return())
		{
			return res;
		}

		bool assigned = context->symbol_table()->assign(var_name, new_value);
		if (!assigned)
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable already exists or is immutable!", context));
			return res;
		}

		res.success(new_value);
		return res;
	}

	result::RuntimeResult Interpreter::visit_var_declaration_node(std::shared_ptr<nodes::VarDeclarationNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::string var_name = node->var_name_token().value().value_or("");
		bool is_mutable = node->is_mutable();

		if (var_name == "")
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable name expected!", context));
			return res;
		}

		std::shared_ptr<Value> new_value = res.register_res(Interpreter::visit(node->value_node(), context));

		if (res.should_return())
		{
			return res;
		}

		bool assigned = context->symbol_table()->declare(var_name, { new_value, is_mutable });
		if (!assigned)
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable already exists!", context));
			return res;
		}

		res.success(new_value);
		return res;
	}

	result::RuntimeResult Interpreter::visit_bin_op_node(std::shared_ptr<nodes::BinOpNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::shared_ptr<Value> left = res.register_res(Interpreter::visit(node->left_node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::shared_ptr<Value> right = res.register_res(Interpreter::visit(node->right_node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> result =
			[&, node, context]() -> std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>>
		{
			if (node->op_token().type() == TokenType::Plus)
			{
				return left->added_to(right);
			}
			if (node->op_token().type() == TokenType::Minus)
			{
				return left->subbed_by(right);
			}
			if (node->op_token().type() == TokenType::Mult)
			{
				return left->multed_by(right);
			}
			if (node->op_token().type() == TokenType::Div)
			{
				return left->dived_by(right);
			}
			if (node->op_token().type() == TokenType::Pow)
			{
				return left->powed_by(right);
			}
			if (node->op_token().type() == TokenType::Ee)
			{
				return left->comparison_eq(right);
			}
			if (node->op_token().type() == TokenType::Ne)
			{
				return left->comparison_ne(right);
			}
			if (node->op_token().type() == TokenType::Gt)
			{
				return left->comparison_gt(right);
			}
			if (node->op_token().type() == TokenType::Lt)
			{
				return left->comparison_lt(right);
			}
			if (node->op_token().type() == TokenType::Gte)
			{
				return left->comparison_gte(right);
			}
			if (node->op_token().type() == TokenType::Lte)
			{
				return left->comparison_lte(right);
			}
			if (node->op_token().matches(TokenType::Keyword, "and"))
			{
				return left->anded_by(right);
			}
			if (node->op_token().matches(TokenType::Keyword, "or"))
			{
				return left->ored_by(right);
			}
			return { nullptr, std::make_unique<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Unknown operation!", context) };
		}();

		if (result.second != nullptr)
		{
			res.failure(std::make_shared<errors::RuntimeError>(result.second));
			return res;
		}

		result.first->pos_start() = node->pos_start();
		result.first->pos_end() = node->pos_end();
		
		res.success(std::make_shared<Value>(result.first));
		return res;
	}

	result::RuntimeResult Interpreter::visit_unary_op_node(std::shared_ptr<nodes::UnaryOpNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::shared_ptr<Value> number = res.register_res(Interpreter::visit(node->node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> result =
			[&, node, context]() -> std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>>
		{
			if (node->op_token().type() == TokenType::Minus)
			{
				return number->multed_by(std::make_shared<values::NumberValue>(-1));
			}
			if (node->op_token().matches(TokenType::Keyword, "not"))
			{
				return number->notted();
			}
			return { nullptr, std::make_unique<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Unknown operation!", context) };
		}();

		if (result.second != nullptr)
		{
			res.failure(std::make_shared<errors::RuntimeError>(result.second));
			return res;
		}

		result.first->pos_start() = node->pos_start();
		result.first->pos_end() = node->pos_end();

		res.success(std::make_shared<Value>(result.first));
		return res;
	}

#pragma endregion

}