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
			throw std::invalid_argument(utils::std_string_format("Not visit method defined for: %s!", node->node_type()).c_str());
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
			
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), utils::std_string_format("'%s' is not declared in this scope!", var_name.c_str()), context));
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

		std::pair<std::unique_ptr<Value>, std::shared_ptr<errors::RuntimeError>> result =
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
			res.failure(result.second);
			return res;
		}

		result.first->pos_start() = node->pos_start();
		result.first->pos_end() = node->pos_end();
		
		res.success(result.first->copy());
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

		std::pair<std::unique_ptr<Value>, std::shared_ptr<errors::RuntimeError>> result =
			[&, node, context]() -> std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>>
		{
			if (node->op_token().type() == TokenType::Minus)
			{
				return number->multed_by(std::make_shared<values::NumberValue>(-1.0f));
			}
			if (node->op_token().matches(TokenType::Keyword, "not"))
			{
				return number->notted();
			}
			return { nullptr, std::make_unique<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Unknown operation!", context) };
		}();


		if (result.second != nullptr)
		{
			res.failure(result.second);
			return res;
		}

		result.first->pos_start() = node->pos_start();
		result.first->pos_end() = node->pos_end();

		res.success(result.first->copy());
		return res;
	}

	result::RuntimeResult Interpreter::visit_if_node(std::shared_ptr<nodes::IfNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		for (const nodes::IfNode::if_case& ic : node->cases())
		{
			std::shared_ptr<Value> condition_value = res.register_res(Interpreter::visit(ic.condition, context));
			if (res.should_return())
			{
				return res;
			}

			if (condition_value->is_true())
			{
				if (ic.should_return_null)
				{
					res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
					return res;
				}

				std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(ic.statements, context));
				if (!res.should_return())
				{
					res.success(body_value);
				}

				return res;
			}
		}

		if (node->else_case().has_value())
		{
			if (node->else_case().value().should_return_null)
			{
				res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
				return res;
			}

			std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(node->else_case().value().statements, context));
			if (!res.should_return())
			{
				res.success(body_value);
			}

			return res;
		}

		res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
		return res;
	}

	result::RuntimeResult Interpreter::visit_while_node(std::shared_ptr<nodes::WhileNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::vector<std::shared_ptr<Value>> elements;

		while (true)
		{
			std::shared_ptr<Value> condition = res.register_res(Interpreter::visit(node->condition_node(), context));
			if (res.should_return())
			{
				return res;
			}

			if (!condition->is_true())
			{
				break;
			}

			std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(node->body_node(), context));

			if (res.should_return())
			{
				if (res.loop_should_continue()) continue;
				if (res.loop_should_break()) break;

				return res;
			}

			elements.emplace_back(body_value);
		}

		if (node->should_return_null())
		{
			res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
			return res;
		}

		res.success(std::make_shared<values::ListValue>(elements, node->pos_start(), node->pos_end(), context));
		return res;
	}

	result::RuntimeResult Interpreter::visit_for_node(std::shared_ptr<nodes::ForNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::string var_name = node->token().value().value_or("");
		if (var_name == "")
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->token().pos_start(), node->token().pos_end(), "Variable name expected!", context));
			return res;
		}

		std::vector<std::shared_ptr<Value>> elements;

		std::shared_ptr<values::NumberValue> start_value = std::dynamic_pointer_cast<values::NumberValue>(res.register_res(Interpreter::visit(node->start_value_node(), context)));
		if (res.should_return() || start_value == nullptr)
		{
			return res;
		}

		std::shared_ptr<values::NumberValue> end_value = std::dynamic_pointer_cast<values::NumberValue>(res.register_res(Interpreter::visit(node->end_value_node(), context)));
		if (res.should_return() || start_value == nullptr)
		{
			return res;
		}

		std::shared_ptr<values::NumberValue> step_value;
		if (node->step_value_node() != nullptr)
		{
			step_value = std::dynamic_pointer_cast<values::NumberValue>(res.register_res(Interpreter::visit(node->step_value_node(), context)));
			if (res.should_return() || step_value == nullptr)
			{
				return res;
			}
		}
		else
		{
			step_value = std::make_shared<values::NumberValue>(1.0f);
		}

		float enumerator = step_value->value();
		
		auto condition = [start_value, end_value, enumerator]() -> bool
		{
			if (start_value->value() >= 0)
			{
				return enumerator < end_value->value();
			}
			else
			{
				return enumerator > end_value->value();
			}
		};

		if (context->symbol_table()->exists_rec(var_name).first)
		{
			if (!context->symbol_table()->is_mutable(var_name))
			{
				res.failure(std::make_shared<errors::RuntimeError>(node->token().pos_start(), node->token().pos_end(), utils::std_string_format("Variable '%s' already exists and is immutable!", var_name.c_str()), context));
				return res;
			}
		}

		while (condition())
		{
			context->symbol_table()->set(var_name, { std::make_shared<values::NumberValue>(enumerator), false });
			enumerator += (float)step_value->value();

			std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(node->body_node(), context));

			if (res.should_return())
			{
				if (res.loop_should_continue()) continue;
				if (res.loop_should_break()) break;

				return res;
			}

			elements.emplace_back(body_value);

		}

		if (node->should_return_null())
		{
			res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
			return res;
		}

		res.success(std::make_shared<values::ListValue>(elements, node->pos_start(), node->pos_end(), context));
		return res;
	}

	result::RuntimeResult Interpreter::visit_function_def_node(std::shared_ptr<nodes::FunctionDefNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::optional<std::string> func_name = node->var_name_token().has_value() ? node->var_name_token().value().value() : std::nullopt;

		std::vector<std::string> arg_names;
		for (const Token& an : node->arg_name_tokens())
		{
			if (!an.has_value())
			{
				res.failure(std::make_shared<errors::RuntimeError>(an.pos_start(), an.pos_end(), "Variable name expected!", context));
				return res;
			}

			arg_names.emplace_back(an.value().value());
		}

		// TODO: function def
		res.success(std::make_shared<values::NumberValue>(0.0f));
		return res;
	}

	result::RuntimeResult Interpreter::visit_call_node(std::shared_ptr<nodes::CallNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		// TODO: call node
		res.success(std::make_shared<values::NumberValue>(0.0f));
		return res;
	}

	result::RuntimeResult Interpreter::visit_return_node(std::shared_ptr<nodes::ReturnNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::shared_ptr<Value> value_to_return;
		if (node->node_to_return() != nullptr)
		{
			value_to_return = res.register_res(Interpreter::visit(node->node_to_return(), context));
			if (res.should_return())
			{
				return res;
			}
		}
		else
		{
			value_to_return = std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE);
		}

		res.success_return(value_to_return);
		return res;
	}

	result::RuntimeResult Interpreter::visit_continue_node(std::shared_ptr<nodes::ContinueNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();
		res.success_continue();
		return res;
	}

	result::RuntimeResult Interpreter::visit_break_node(std::shared_ptr<nodes::BreakNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();
		res.success_break();
		return res;
	}


#pragma endregion

}