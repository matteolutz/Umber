#pragma once

#include "../Node.h"
#include "../Token.h"

#include <optional>

namespace umber
{

	namespace nodes
	{

		class ForNode : public Node
		{

		private:
			Token m_var_name_token;
			Node* m_start_value_node, * m_end_value_node;
			Node* m_step_value_node;
			Node* m_body_node;
			bool m_should_return_null;

		public:
			ForNode(Token var_name_token, Node* start_value_node, Node* end_value_node, Node* step_value_node, Node* body_node, bool should_return_null);
			~ForNode();

		};

	}

}