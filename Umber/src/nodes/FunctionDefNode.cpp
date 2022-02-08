#include "FunctionDefNode.h"

namespace umber
{

	namespace nodes
	{



		FunctionDefNode::FunctionDefNode(std::optional<Token> var_name_token, std::vector<Token> arg_name_tokens, std::shared_ptr<Node> body_node, bool should_auto_return)
			: Node(
				var_name_token.has_value() ?
				var_name_token.value().pos_start()
				: (
					!arg_name_tokens.empty() ? arg_name_tokens[0].pos_start() : body_node->pos_start()
					),
				body_node->pos_end(),
				NodeType::FunctionDef
			), m_var_name_token(var_name_token), m_arg_name_tokens(arg_name_tokens), m_body_node(std::move(body_node)), m_should_auto_return(should_auto_return) { }

	}

}