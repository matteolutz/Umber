#pragma once

#include "Token.h"
#include "Node.h"
#include "result/ParseResult.h"
#include "errors/InvalidSyntaxError.h"

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
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"
#include "nodes/WhileNode.h"
#include "nodes/ListNode.h"
#include "nodes/FunctionDefNode.h"
#include "nodes/VarAssignNode.h"
#include "nodes/AccessorNode.h"
#include "nodes/DictNode.h"

#include <vector>
#include <map>
#include <optional>

namespace umber
{

	class Parser
	{
	private:
		std::vector<Token> m_tokens;
		unsigned int m_token_index = 0;
		std::optional<Token> m_current_token;

		enum class BinOpFunction
		{
			Comp,
			Arith,
			Term,
			Factor,
			Call
		};
		result::ParseResult eval_bin_op_func(BinOpFunction func);

	public:
		Parser(std::vector<Token> tokens);

		Token advance();
		Token reverse(unsigned int amount = 1);

		void update_current_token();

		result::ParseResult parse();

#pragma region Parser Functions
		result::ParseResult if_expr();

		result::ParseResult for_expr();
		result::ParseResult while_expr();

		result::ParseResult list_expr();
		result::ParseResult dict_expr();

		result::ParseResult function_def();

		result::ParseResult bin_operation(BinOpFunction function_a, std::vector<std::pair<TokenType, std::optional<std::string>>> ops, std::optional<BinOpFunction> function_b = std::nullopt);

		result::ParseResult statement();
		result::ParseResult statements();
		result::ParseResult expression();
		result::ParseResult comp_expression();
		result::ParseResult arith_expression();
		result::ParseResult term();
		result::ParseResult factor();
		result::ParseResult power();
		result::ParseResult call();
		result::ParseResult atom();
#pragma endregion

	};

}