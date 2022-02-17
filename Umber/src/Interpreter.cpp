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
		case NodeType::Dict: return Interpreter::visit_dict_node(std::static_pointer_cast<nodes::DictNode>(node), context);

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

		case NodeType::Accessor: return Interpreter::visit_accessor_node(std::static_pointer_cast<nodes::AccessorNode>(node), context);
		case NodeType::AccessorSet: return Interpreter::visit_accessor_set_node(std::static_pointer_cast<nodes::AccessorSetNode>(node), context);

		case NodeType::Import: return Interpreter::visit_import_node(std::static_pointer_cast<nodes::ImportNode>(node), context);

		default:
			throw std::invalid_argument(utils::std_string_format("Not visit method defined for: %d!", node->node_type()).c_str());
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
		elements.reserve(node->elements().size());

		for (const std::shared_ptr<Node>& n : node->elements())
		{
			elements.push_back(res.register_res(Interpreter::visit(n, context)));
			if (res.should_return())
			{
				return res;
			}
		}

		res.success(std::make_shared<values::ListValue>(std::move(elements), node->pos_start(), node->pos_end(), context));
		return res;
	}

	result::RuntimeResult Interpreter::visit_dict_node(std::shared_ptr<nodes::DictNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::map<std::string, std::shared_ptr<Value>> elements;

		for (const auto& [name, node] : node->elements())
		{
			elements[name] = res.register_res(Interpreter::visit(node, context));
			if (res.should_return())
			{
				return res;
			}
		}

		res.success(std::make_shared<values::DictValue>(std::move(elements), node->pos_start(), node->pos_end(), context));
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
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable was either not declared in this scope or is immutable!", context));
			return res;
		}

		res.success(std::move(new_value));
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

		bool assigned = context->symbol_table()->declare(var_name, { new_value, is_mutable});

		if (!assigned)
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Variable was already declared in this scope!", context));
			return res;
		}

		res.success(std::move(new_value));
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

		std::pair<std::shared_ptr<Value>, std::shared_ptr<errors::RuntimeError>> bin_op_result =
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
			} if (node->op_token().type() == TokenType::Modulo)
			{
				return left->moduloed_by(right);
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
			if (node->op_token().type() == TokenType::And)
			{
				return left->anded_by(right);
			}
			if (node->op_token().type() == TokenType::Or)
			{
				return left->ored_by(right);
			}
			if (node->op_token().type() == TokenType::BitAnd)
			{
				return left->bit_anded_by(right);
			}
			if (node->op_token().type() == TokenType::BitOr)
			{
				return left->bit_ored_by(right);
			}
			return { nullptr, std::make_unique<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Unknown operation!", context) };
		}();

		if (bin_op_result.second != nullptr)
		{
			res.failure(std::move(bin_op_result.second));
			return res;
		}

		std::shared_ptr<Value> ret_value = std::move(bin_op_result.first);

		ret_value->pos_start() = node->pos_start();
		ret_value->pos_end() = node->pos_end();

		res.success(ret_value);
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

		std::pair<std::shared_ptr<Value>, std::shared_ptr<errors::RuntimeError>> unary_op_result =
			[&, node, context]() -> std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>>
		{
			if (node->op_token().type() == TokenType::Minus)
			{
				return number->multed_by(std::make_shared<values::NumberValue>(-1.0f));
			}
			if (node->op_token().type() == TokenType::Plus)
			{
				return { number->copy(), nullptr};
			}
			if (node->op_token().type() == TokenType::Not)
			{
				return number->notted();
			}
			return { nullptr, std::make_unique<errors::RuntimeError>(node->pos_start(), node->pos_end(), "Unknown operation!", context) };
		}();


		if (unary_op_result.second != nullptr)
		{
			res.failure(std::move(unary_op_result.second));
			return res;
		}

		std::shared_ptr<Value> ret_value = std::move(unary_op_result.first);

		ret_value->pos_start() = node->pos_start();
		ret_value->pos_end() = node->pos_end();

		res.success(ret_value);
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
				std::shared_ptr<Context> exec_ctx = std::make_shared<Context>("<if>", context, std::make_shared<SymbolTable>(context->symbol_table()));

				std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(ic.statements, exec_ctx));
				if (res.should_return())
				{
					return res;
				}

				if (ic.should_return_null)
				{
					res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
					return res;
				}

				res.success(std::move(body_value));
				return res;
			}
		}

		if (node->else_case().has_value())
		{
			std::shared_ptr<Context> exec_ctx = std::make_shared<Context>("<if-else>", context, std::make_shared<SymbolTable>(context->symbol_table()));

			std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(node->else_case().value().statements, exec_ctx));
			if (res.should_return())
			{
				return res;
			}

			if (node->else_case().value().should_return_null)
			{
				res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
				return res;
			}

			res.success(std::move(body_value));
			return res;
		}

		res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
		return res;
	}

	result::RuntimeResult Interpreter::visit_while_node(std::shared_ptr<nodes::WhileNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::vector<std::shared_ptr<Value>> elements;

		std::shared_ptr<Context> exec_ctx = std::make_shared<Context>("<while>", context, std::make_shared<SymbolTable>(context->symbol_table()));

		while (true)
		{
			exec_ctx->symbol_table()->clear();

			std::shared_ptr<Value> condition = res.register_res(Interpreter::visit(node->condition_node(), exec_ctx));
			if (res.should_return())
			{
				return res;
			}

			if (!condition->is_true())
			{
				break;
			}

			std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(node->body_node(), exec_ctx));

			if (res.should_return())
			{
				if (res.loop_should_continue()) continue;
				if (res.loop_should_break()) break;

				return res;
			}

			elements.push_back(std::move(body_value));
		}

		if (node->should_return_null())
		{
			res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
			return res;
		}

		res.success(std::make_shared<values::ListValue>(std::move(elements), node->pos_start(), node->pos_end(), context));
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

		float f_step_value;
		if (node->step_value_node() != nullptr)
		{
			std::shared_ptr<values::NumberValue> step_value = std::dynamic_pointer_cast<values::NumberValue>(res.register_res(Interpreter::visit(node->step_value_node(), context)));
			if (res.should_return() || step_value == nullptr)
			{
				return res;
			}

			f_step_value = step_value->value();
		}
		else
		{
			f_step_value = 1.0f;
		}

		float enumerator = start_value->value();
		std::shared_ptr<Context> exec_ctx = std::make_shared<Context>("<for>", context, std::make_shared<SymbolTable>(context->symbol_table()));

		if (exec_ctx->symbol_table()->exists_rec(var_name))
		{
			if (!exec_ctx->symbol_table()->assign(var_name, std::make_shared<values::NumberValue>(enumerator)))
			{
				res.failure(std::make_shared<errors::RuntimeError>(node->token().pos_start(), node->token().pos_end(), utils::std_string_format("Variable '%s' already was already declared as immutable!", var_name.c_str()), exec_ctx));
				return res;
			}
		}
		else
		{
			exec_ctx->symbol_table()->declare(var_name, { std::make_shared<values::NumberValue>(enumerator), false });
		}

		while (
			f_step_value >= 0.0f
			? enumerator < end_value->value()
			: enumerator > end_value->value()
			)
		{

			exec_ctx->symbol_table()->clear();
			exec_ctx->symbol_table()->set(var_name, { std::make_shared<values::NumberValue>(enumerator), false });

			std::shared_ptr<Value> body_value = res.register_res(Interpreter::visit(node->body_node(), exec_ctx));

			if (res.should_return())
			{
				if (res.loop_should_continue()) continue;
				if (res.loop_should_break()) break;

				return res;
			}

			elements.push_back(std::move(body_value));

			enumerator += f_step_value;
		}

		if (node->should_return_null())
		{
			res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
			return res;
		}

		res.success(std::make_shared<values::ListValue>(std::move(elements), node->pos_start(), node->pos_end(), context));
		return res;
	}

	result::RuntimeResult Interpreter::visit_function_def_node(std::shared_ptr<nodes::FunctionDefNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::optional<std::string> func_name = node->var_name_token().has_value() ? node->var_name_token().value().value() : std::nullopt;

		std::vector<std::string> arg_names;
		arg_names.reserve(node->arg_name_tokens().size());

		for (const Token& an : node->arg_name_tokens())
		{
			if (!an.value().has_value())
			{
				res.failure(std::make_shared<errors::RuntimeError>(an.pos_start(), an.pos_end(), "Variable name expected!", context));
				return res;
			}

			arg_names.push_back(an.value().value());
		}

		std::shared_ptr<values::FunctionValue> value = std::make_shared<values::FunctionValue>(func_name.value_or("<anonymous>"), node->body_node(), std::move(arg_names), node->should_auto_return(), node->pos_start(), node->pos_end(), context);

		if (node->var_name_token().has_value() && node->var_name_token().value().value().has_value())
		{
			std::string new_symbol_name = node->var_name_token().value().value().value();
			bool delcared = context->symbol_table()->declare(new_symbol_name, { value, false });

			if (!delcared)
			{
				res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), "A variable with that name was already declared in this scope!", context));
				return res;
			}
		}

		res.success(std::move(value));
		return res;
	}

	result::RuntimeResult Interpreter::visit_call_node(std::shared_ptr<nodes::CallNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::vector<std::shared_ptr<Value>> args;
		args.reserve(node->arg_nodes().size());

		std::shared_ptr<values::BaseFunctionValue> value_to_call = std::dynamic_pointer_cast<values::BaseFunctionValue>(res.register_res(Interpreter::visit(node->node_to_call(), context)));
		if (value_to_call == nullptr)
		{
			res.failure(std::make_shared<errors::RuntimeError>(node->pos_start(), node->pos_end(), utils::std_string_format("The value isn't callable!").c_str(), context));
			return res;
		}
		else if (res.should_return())
		{
			return res;
		}

		value_to_call->pos_start() = node->pos_start();
		value_to_call->pos_end() = node->pos_end();

		for (const std::shared_ptr<Node>& arg_node : node->arg_nodes())
		{
			args.push_back(res.register_res(Interpreter::visit(arg_node, context)));
			if (res.should_return())
			{
				return res;
			}
		}

		std::shared_ptr<Value> return_value = res.register_res(value_to_call->execute(std::move(args)));
		if (res.should_return())
		{
			return res;
		}

		return_value->pos_start() = node->pos_start();
		return_value->pos_end() = node->pos_end();
		return_value->context() = context;

		res.success(std::move(return_value));
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

		res.success_return(std::move(value_to_return));
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

	result::RuntimeResult Interpreter::visit_accessor_node(std::shared_ptr<nodes::AccessorNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::shared_ptr<Value> accessor_value = res.register_res(Interpreter::visit(node->accessor_node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::shared_ptr<Value> value_to_be_accessed = res.register_res(Interpreter::visit(node->node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::pair<std::shared_ptr<Value>, std::shared_ptr<errors::RuntimeError>> accessor_result = value_to_be_accessed->access(accessor_value);
		if (accessor_result.second != nullptr)
		{
			res.failure(std::move(accessor_result.second));
			return res;
		}

		std::shared_ptr<Value> ret_value = std::move(accessor_result.first);

		ret_value->pos_start() = node->pos_start();
		ret_value->pos_end() = node->pos_end();
		ret_value->context() = context;

		res.success(ret_value);
		return res;
	}

	result::RuntimeResult Interpreter::visit_accessor_set_node(std::shared_ptr<nodes::AccessorSetNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		std::shared_ptr<Value> accessor_value = res.register_res(Interpreter::visit(node->accessor_node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::shared_ptr<Value> value_to_be_accessed = res.register_res(Interpreter::visit(node->accessed_node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::shared_ptr<Value> value_to_be_set = res.register_res(Interpreter::visit(node->set_node(), context));
		if (res.should_return())
		{
			return res;
		}

		std::pair<std::shared_ptr<Value>, std::shared_ptr<errors::RuntimeError>> accessor_set_result = value_to_be_accessed->set(accessor_value, value_to_be_set);
		if (accessor_set_result.second != nullptr)
		{
			res.failure(std::move(accessor_set_result.second));
			return res;
		}

		std::shared_ptr<Value> ret_value = std::move(accessor_set_result.first);

		ret_value->pos_start() = node->pos_start();
		ret_value->pos_end() = node->pos_end();
		ret_value->context() = context;

		res.success(ret_value);
		return res;
	}

	result::RuntimeResult Interpreter::visit_import_node(std::shared_ptr<nodes::ImportNode> node, std::shared_ptr<Context> context)
	{
		auto res = result::RuntimeResult();

		// TOOD import 

		res.success(std::make_shared<values::NumberValue>(values::NumberValue::NULL_VALUE));
		return res;
	}

#pragma endregion

}