#include "Parser.h"

namespace umber
{

	Parser::Parser(std::vector<Token> tokens)
		: m_tokens(tokens)
	{
		if (!this->m_tokens.empty())
		{
			this->m_current_token.emplace(this->m_tokens[0]);
		}
	}

	Token& Parser::advance()
	{
		this->m_token_index++;
		this->update_current_token();
		return this->m_current_token.value();
	}

	Token& Parser::reverse(unsigned int amount)
	{
		this->m_token_index -= amount;
		this->update_current_token();
		return this->m_current_token.value();
	}

	void Parser::update_current_token()
	{
		if (this->m_token_index >= 0 && this->m_token_index < this->m_tokens.size())
		{
			this->m_current_token.emplace(this->m_tokens[this->m_token_index]);
		}
	}

	result::ParseResult Parser::parse()
	{
		auto res = this->statements();

		if (!res.has_error() && this->m_current_token.value().type() != TokenType::Eof)
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected statements" });
		}

		return res;
	}

#pragma region Parser Functions

	result::ParseResult Parser::if_expr()
	{
		auto res = result::ParseResult();

		if (!this->m_current_token.value().matches(TokenType::Keyword, "if"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'if'!" });
			return res;
		}

		std::vector<nodes::IfNode::if_case> cases;
		Node* else_case = nullptr;

		res.register_advancement();
		this->advance();

#pragma region If statement
		Node* condition = res.register_res(this->expression());
		if (res.has_error())
		{
			return res;
		}

		if (this->m_current_token.value().type() == TokenType::Lcurly)
		{
			res.register_advancement();
			this->advance();

			Node* statements = res.register_res(this->statements());
			if (res.has_error())
			{
				return res;
			}

			if (this->m_current_token.value().type() != TokenType::Rcurly)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '}'!" });
				return res;
			}

			cases.emplace_back(nodes::IfNode::if_case{ condition, statements, true });

			res.register_advancement();
			this->advance();

		}
		else
		{
			if (!this->m_current_token.value().matches(TokenType::Keyword, "then"))
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'then'!" });
				return res;
			}

			res.register_advancement();
			this->advance();

			Node* expr = res.register_res(this->statement());
			if (res.has_error())
			{
				return res;
			}

			cases.emplace_back(nodes::IfNode::if_case{ condition, expr, false });

		}
#pragma endregion

#pragma region Elif statements
		while (this->m_current_token.value().matches(TokenType::Keyword, "elif"))
		{
			res.register_advancement();
			this->advance();


			Node* elif_condition = res.register_res(this->expression());
			if (res.has_error())
			{
				return res;
			}

			if (this->m_current_token.value().type() == TokenType::Lcurly)
			{
				res.register_advancement();
				this->advance();

				Node* statements = res.register_res(this->statements());
				if (res.has_error())
				{
					return res;
				}

				if (this->m_current_token.value().type() != TokenType::Rcurly)
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '}'!" });
					return res;
				}

				cases.emplace_back(nodes::IfNode::if_case{ condition, statements, true });

				res.register_advancement();
				this->advance();

			}
			else
			{
				if (!this->m_current_token.value().matches(TokenType::Keyword, "then"))
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'then'!" });
					return res;
				}

				res.register_advancement();
				this->advance();

				Node* expr = res.register_res(this->statement());
				if (res.has_error())
				{
					return res;
				}

				cases.emplace_back(nodes::IfNode::if_case{ condition, expr, false });
			}

		}
#pragma endregion

#pragma region Else statement
		if (this->m_current_token.value().matches(TokenType::Keyword, "else"))
		{

			res.register_advancement();
			this->advance();

			if (this->m_current_token.value().type() == TokenType::Lcurly)
			{
				res.register_advancement();
				this->advance();

				Node* statements = res.register_res(this->statements());
				if (res.has_error())
				{
					return res;
				}

				if (this->m_current_token.value().type() != TokenType::Rcurly)
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '}'!" });
					return res;
				}

				else_case = statements;

				res.register_advancement();
				this->advance();

			}
			else
			{
				if (!this->m_current_token.value().matches(TokenType::Keyword, "then"))
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'then'!" });
					return res;
				}

				res.register_advancement();
				this->advance();

				Node* expr = res.register_res(this->statement());
				if (res.has_error())
				{
					return res;
				}

				else_case = expr;
			}
		}
#pragma endregion

		res.success(new nodes::IfNode{ cases, else_case });
		return res;
	}

	result::ParseResult Parser::for_expr()
	{
		auto res = result::ParseResult();

		if (!this->m_current_token.value().matches(TokenType::Keyword, "for"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'for'!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		if (this->m_current_token.value().type() != TokenType::Identifier)
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected identifier!" });
			return res;
		}

		Token var_name_token = this->m_current_token.value();
		res.register_advancement();
		this->advance();

		if (this->m_current_token.value().type() != TokenType::Eq)
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '='!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		Node* start_value = res.register_res(this->expression());
		if (res.has_error())
		{
			return res;
		}

		if (!this->m_current_token.value().matches(TokenType::Keyword, "to"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'to'!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		Node* end_value = res.register_res(this->expression());
		if (res.has_error())
		{
			return res;
		}

		Node* step_value = nullptr;
		if (this->m_current_token.value().matches(TokenType::Keyword, "step"))
		{
			res.register_advancement();
			this->advance();

			step_value = res.register_res(this->expression());
			if (res.has_error())
			{
				return res;
			}
		}

		if (this->m_current_token.value().type() == TokenType::Lcurly)
		{
			res.register_advancement();
			this->advance();

			Node* for_node_body = res.register_res(this->statements());
			if (res.has_error())
			{
				return res;
			}

			if (this->m_current_token.value().type() != TokenType::Rcurly)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '}'" });
				return res;
			}

			res.register_advancement();
			this->advance();

			res.success(new nodes::ForNode{ var_name_token, start_value, end_value, step_value, for_node_body, true });
			return res;
		}

		if (!this->m_current_token.value().matches(TokenType::Keyword, "then"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'then'" });
			return res;
		}

		res.register_advancement();
		this->advance();

		Node* for_node_body = res.register_res(this->statement());
		if (res.has_error())
		{
			return res;
		}

		res.success(new nodes::ForNode{ var_name_token, start_value, end_value, step_value, for_node_body, false });
		return res;
	}

	result::ParseResult Parser::while_expr()
	{
		auto res = result::ParseResult();

		if (!this->m_current_token.value().matches(TokenType::Keyword, "while"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'while'" });
			return res;
		}

		res.register_advancement();
		this->advance();

		Node* condition = res.register_res(this->expression());
		if (res.has_error())
		{
			return res;
		}

		if (this->m_current_token.value().type() == TokenType::Lcurly)
		{

			res.register_advancement();
			this->advance();

			Node* while_node_body = res.register_res(this->statements());
			if (res.has_error())
			{
				return res;
			}

			if (this->m_current_token.value().type() != TokenType::Rcurly)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '}'" });
				return res;
			}

			res.register_advancement();
			this->advance();

			res.success(new nodes::WhileNode{ condition, while_node_body, true });
			return res;
		}

		if (!this->m_current_token.value().matches(TokenType::Keyword, "then"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'then'" });
			return res;
		}

		res.register_advancement();
		this->advance();

		Node* while_node_body = res.register_res(this->statement());
		if (res.has_error())
		{
			return res;
		}

		res.success(new nodes::WhileNode{ condition, while_node_body, false });
		return res;
	}

	result::ParseResult Parser::list_expr()
	{
		auto res = result::ParseResult();

		if (this->m_current_token.value().type() != TokenType::Lsquare)
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '['!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		std::vector<Node*> element_nodes;
		Position pos_start = this->m_current_token.value().pos_start();

		if (this->m_current_token.value().type() == TokenType::Rsquare)
		{
			res.register_advancement();
			this->advance();
		}
		else
		{
			Node* new_element_node = res.register_res(this->expression());
			if (res.has_error())
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected expression!" });
				return res;
			}

			element_nodes.emplace_back(new_element_node);

			while (this->m_current_token.value().type() == TokenType::Comma)
			{
				res.register_advancement();
				this->advance();

				Node* new_element_node = res.register_res(this->expression());
				if (res.has_error())
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected expression!" });
					return res;
				}

				element_nodes.emplace_back(new_element_node);

			}

			if (this->m_current_token.value().type() != TokenType::Rsquare)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected ',' or ']'!" });
				return res;
			}

			res.register_advancement();
			this->advance();

		}

		res.success(new nodes::ListNode{ element_nodes, pos_start, this->m_current_token.value().pos_end() });
		return res;
	}

	result::ParseResult Parser::function_def()
	{
		auto res = result::ParseResult();

		if (!this->m_current_token.value().matches(TokenType::Keyword, "fun"))
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'fun'!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		std::optional<Token> var_name_token = std::nullopt;

		if (this->m_current_token.value().type() == TokenType::Identifier)
		{
			var_name_token.emplace(this->m_current_token.value());

			res.register_advancement();
			this->advance();

			if (this->m_current_token.value().type() != TokenType::Lparen)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '('!" });
				return res;
			}

		}
		else
		{
			if (this->m_current_token.value().type() != TokenType::Lparen)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected identifier or '('!" });
				return res;
			}
		}

		res.register_advancement();
		this->advance();

		std::vector<Token> arg_name_tokens;
		if (this->m_current_token.value().type() == TokenType::Identifier)
		{
			arg_name_tokens.emplace_back(this->m_current_token.value());
			res.register_advancement();
			this->advance();

			while (this->m_current_token.value().type() == TokenType::Comma)
			{
				res.register_advancement();
				this->advance();

				if (this->m_current_token.value().type() != TokenType::Identifier)
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected identifier!" });
					return res;
				}

				arg_name_tokens.emplace_back(this->m_current_token.value());
				res.register_advancement();
				this->advance();
			}

			if (this->m_current_token.value().type() != TokenType::Rparen)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected ')'!" });
				return res;
			}

		}
		else
		{
			if (this->m_current_token.value().type() != TokenType::Rparen)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected identifier or ')'!" });
				return res;
			}
		}

		res.register_advancement();
		this->advance();

		if (this->m_current_token.value().type() == TokenType::Arrow)
		{
			res.register_advancement();
			this->advance();

			Node* node_to_return = res.register_res(this->expression());
			if (res.has_error())
			{
				return res;
			}

			res.success(new nodes::FunctionDefNode{ var_name_token, arg_name_tokens, node_to_return, true });
			return res;
		}

		if (this->m_current_token.value().type() != TokenType::Lcurly)
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '{'!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		Node* function_body = res.register_res(this->statements());
		if (res.has_error())
		{
			return res;
		}

		if (this->m_current_token.value().type() != TokenType::Rcurly)
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '}'!" });
			return res;
		}

		res.register_advancement();
		this->advance();

		res.success(new nodes::FunctionDefNode{ var_name_token, arg_name_tokens, function_body, false });
		return res;
	}

	result::ParseResult Parser::eval_bin_op_func(BinOpFunction func)
	{
		switch (func)
		{
		case BinOpFunction::Arith:
			return this->arith_expression();
		case BinOpFunction::Comp:
			return this->comp_expression();
		case BinOpFunction::Term:
			return this->term();
		case BinOpFunction::Factor:
			return this->factor();
		case BinOpFunction::Call:
			return this->call();
		};
	}
	result::ParseResult Parser::bin_operation(BinOpFunction function_a, std::vector<std::pair<TokenType, std::optional<std::string>>> ops, std::optional<BinOpFunction> function_b)
	{
		auto res = result::ParseResult();

		Node* left = res.register_res(this->eval_bin_op_func(function_a));

		if (res.has_error())
		{
			return res;
		}

		while (true)
		{

			bool break_while = false;
			for (int i = 0; i < ops.size(); i++)
			{
				if (
					(ops[i].first != this->m_current_token.value().type())
					|| (ops[i].second.has_value() != this->m_current_token.has_value())
					|| (ops[i].second.has_value() && (ops[i].second.value() != this->m_current_token.value().value()))
					)
				{
					break_while = break_while || true;
					break;
				}
			}

			if (break_while) break;

			Token op_token = this->m_current_token.value();

			res.register_advancement();
			this->advance();

			// Node* right = res.register_res(function_b.has_value() ? this->eval_bin_op_func(function_b.value()) : this->eval_bin_op_func(function_a));
			Node* right = res.register_res(this->eval_bin_op_func(function_b.value_or(function_a)));

			if (res.has_error())
			{
				return res;
			}

			res.success(new nodes::BinOpNode{ left, op_token, right });
			return res;
		}

		res.success(left);
		return res;

	}

	result::ParseResult Parser::statements()
	{
		auto res = result::ParseResult();

		std::vector<Node*> statements;
		Position pos_start = this->m_current_token.value().pos_start();

		while (this->m_current_token.value().type() == TokenType::Newline)
		{
			res.register_advancement();
			this->advance();
		}

		Node* statement = res.register_res(this->statement());
		if (res.has_error())
		{
			return res;
		}
		statements.emplace_back(statement);

		bool more_statements = true;

		while (true)
		{
			unsigned int new_line_count = 0;
			while (this->m_current_token.value().type() == TokenType::Newline)
			{
				res.register_advancement();
				this->advance();
				new_line_count++;
			}
			if (new_line_count == 0)
			{
				more_statements = false;
			}

			if (!more_statements)
			{
				break;
			}

			Node* statement = res.try_register_res(this->statement());
			if (statement == nullptr)
			{
				this->reverse(res.to_reverse_count());
				more_statements = false;
				continue;
			}

			statements.emplace_back(statement);
		}

		res.success(new nodes::ListNode{ statements, pos_start, this->m_current_token.value().pos_start() });
		return res;

	}

	result::ParseResult Parser::statement()
	{
		auto res = result::ParseResult();
		Position pos_start = this->m_current_token.value().pos_start();

		if (this->m_current_token.value().matches(TokenType::Keyword, "return"))
		{
			res.register_advancement();
			this->advance();

			Node* expr = res.try_register_res(this->expression());
			if (expr == nullptr)
			{
				this->reverse(res.to_reverse_count());
			}

			res.success(new nodes::ReturnNode{ expr, pos_start, this->m_current_token.value().pos_start() });
			return res;
		}

		if (this->m_current_token.value().matches(TokenType::Keyword, "continue"))
		{
			res.register_advancement();
			this->advance();

			res.success(new nodes::ContinueNode{ pos_start, this->m_current_token.value().pos_start() });
			return res;
		}

		if (this->m_current_token.value().matches(TokenType::Keyword, "break"))
		{
			res.register_advancement();
			this->advance();

			res.success(new nodes::BreakNode{ pos_start, this->m_current_token.value().pos_start() });
			return res;
		}

		Node* expr = res.register_res(this->expression());
		if (res.has_error())
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'return', 'continue', 'break' or expression!" });
			return res;
		}

		res.success(expr);
		return res;
	}

	result::ParseResult Parser::expression()
	{
		auto res = result::ParseResult();

		if (this->m_current_token.value().matches(TokenType::Keyword, "let"))
		{
			res.register_advancement();
			this->advance();

			bool is_mutable = false;
			if (this->m_current_token.value().matches(TokenType::Keyword, "mut"))
			{
				is_mutable = true;
				res.register_advancement();
				this->advance();
			}

			if (this->m_current_token.value().type() != TokenType::Identifier)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected 'mut' or identifier!" });
				return res;
			}

			Token var_name_token = this->m_current_token.value();
			res.register_advancement();
			this->advance();

			if (this->m_current_token.value().type() != TokenType::Eq)
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected '='!" });
				return res;
			}

			res.register_advancement();
			this->advance();

			Node* expr = res.register_res(this->expression());
			if (res.has_error())
			{
				return res;
			}

			res.success(new nodes::VarDeclarationNode{ var_name_token, expr, is_mutable });
			return res;

		}

		Node* node = res.register_res(this->bin_operation(BinOpFunction::Comp, {
			{ TokenType::Keyword, "and"},
			{ TokenType::Keyword, "or"},
			}));

		if (res.has_error())
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected comp expression!" });
			return res;
		}

		res.success(node);
		return res;
	}

	result::ParseResult Parser::comp_expression()
	{
		auto res = result::ParseResult();

		if (this->m_current_token.value().matches(TokenType::Keyword, "not"))
		{
			Token op_token = this->m_current_token.value();

			res.register_advancement();
			this->advance();

			Node* node = res.register_res(this->comp_expression());
			if (res.has_error())
			{
				return res;
			}

			res.success(new nodes::UnaryOpNode{ op_token, node });
			return res;
		}

		Node* node = res.register_res(this->bin_operation(BinOpFunction::Arith, {
			{ TokenType::Ee, std::nullopt },
			{ TokenType::Ne, std::nullopt },
			{ TokenType::Lt, std::nullopt },
			{ TokenType::Gt, std::nullopt },
			{ TokenType::Lte, std::nullopt },
			{ TokenType::Gte, std::nullopt }
			}));

		if (res.has_error())
		{
			res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_start(), "Expected arith expression!" });
			return res;
		}

		res.success(node);
		return res;
	}

	result::ParseResult Parser::arith_expression()
	{
		return this->bin_operation(BinOpFunction::Term, {
			{ TokenType::Plus, std::nullopt },
			{ TokenType::Minus, std::nullopt }
			});
	}

	result::ParseResult Parser::term()
	{
		return this->bin_operation(BinOpFunction::Factor, {
			{ TokenType::Mult, std::nullopt },
			{ TokenType::Div, std::nullopt }
			});
	}

	result::ParseResult Parser::factor()
	{
		auto res = result::ParseResult();
		Token& token = this->m_current_token.value();

		if (token.type() == TokenType::Plus || token.type() == TokenType::Minus)
		{
			res.register_advancement();
			this->advance();

			Node* factor = res.register_res(this->factor());
			if (res.has_error())
			{
				return res;
			}

			res.success(new nodes::UnaryOpNode{ token, factor });
			return res;
		}

		return this->power();
	}

	result::ParseResult Parser::power()
	{
		return this->bin_operation(BinOpFunction::Call, {
			{ TokenType::Pow, std::nullopt }
			}, BinOpFunction::Factor);
	}

	result::ParseResult Parser::call()
	{
		auto res = result::ParseResult();

		Node* atom = res.register_res(this->atom());
		if (res.has_error())
		{
			return res;
		}

		if (this->m_current_token.value().type() == TokenType::Lparen)
		{
			res.register_advancement();
			this->advance();

			std::vector<Node*> arg_nodes;

			if (this->m_current_token.value().type() == TokenType::Rparen)
			{
				res.register_advancement();
				this->advance();
			}
			else
			{
				Node* new_arg = res.register_res(this->expression());

				if (res.has_error())
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected expression!" });
					return res;
				}

				arg_nodes.emplace_back(new_arg);

				while (this->m_current_token.value().type() == TokenType::Comma)
				{
					res.register_advancement();
					this->advance();

					Node* new_arg = res.register_res(this->expression());
					if (res.has_error())
					{
						res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected expression!" });
						return res;
					}
					arg_nodes.emplace_back(new_arg);
				}

				if (this->m_current_token.value().type() != TokenType::Rparen)
				{
					res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected ',' or ')'!" });
					return res;
				}

				res.register_advancement();
				this->advance();

			}

			res.success(new nodes::CallNode{ atom, arg_nodes });
			return res;

		}

		res.success(atom);
		return res;

	}

	result::ParseResult Parser::atom()
	{
		auto res = result::ParseResult();
		Token& token = this->m_current_token.value();

		if (token.type() == TokenType::Int || token.type() == TokenType::Float)
		{
			res.register_advancement();
			this->advance();

			res.success(new nodes::NumberNode{ token });
			return res;
		}

		if (token.type() == TokenType::String)
		{
			res.register_advancement();
			this->advance();

			res.success(new nodes::StringNode{ token });
			return res;
		}

		if (token.type() == TokenType::Identifier)
		{
			res.register_advancement();
			this->advance();

			res.success(new nodes::VarAccessNode{ token });
			return res;
		}

		if (token.type() == TokenType::Lparen)
		{
			res.register_advancement();
			this->advance();

			Node* expr = res.register_res(this->expression());

			if (res.has_error())
			{
				return res;
			}

			if (this->m_current_token.value().type() == TokenType::Rparen)
			{
				res.register_advancement();
				this->advance();

				res.success(expr);
				return res;
			}
			else
			{
				res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected ')'!" });
				return res;
			}
		}

		if (token.type() == TokenType::Lsquare)
		{
			Node* list_expr = res.register_res(this->list_expr());

			if (res.has_error())
			{
				return res;
			}

			res.success(list_expr);
			return res;
		}

		// TODO: dict expr

		if (token.matches(TokenType::Keyword, "if"))
		{
			Node* if_expr = res.register_res(this->if_expr());

			if (res.has_error())
			{
				return res;
			}

			res.success(if_expr);
			return res;
		}

		if (token.matches(TokenType::Keyword, "for"))
		{
			Node* for_expr = res.register_res(this->for_expr());

			if (res.has_error())
			{
				return res;
			}

			res.success(for_expr);
			return res;
		}

		if (token.matches(TokenType::Keyword, "while"))
		{
			Node* while_expr = res.register_res(this->while_expr());

			if (res.has_error())
			{
				return res;
			}

			res.success(while_expr);
			return res;
		}

		if (token.matches(TokenType::Keyword, "fun"))
		{
			Node* func_def = res.register_res(this->function_def());

			if (res.has_error())
			{
				return res;
			}

			res.success(func_def);
			return res;
		}

		res.failure(new errors::InvalidSyntaxError{ this->m_current_token.value().pos_start(), this->m_current_token.value().pos_end(), "Expected!" });
		return res;
	}

#pragma endregion
}