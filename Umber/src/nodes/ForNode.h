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
			std::shared_ptr<Node> m_start_value_node, m_end_value_node;
			std::shared_ptr<Node> m_step_value_node;
			std::shared_ptr<Node> m_body_node;
			bool m_should_return_null;

		public:
			ForNode(Token var_name_token, std::shared_ptr<Node> start_value_node, std::shared_ptr<Node> end_value_node, std::shared_ptr<Node> step_value_node, std::shared_ptr<Node> body_node, bool should_return_null);

			inline const Token& token() const { return this->m_var_name_token; }
			inline const std::shared_ptr<Node>& start_value_node() { return this->m_start_value_node; }
			inline const std::shared_ptr<Node>& end_value_node() { return this->m_end_value_node; }
			inline const std::shared_ptr<Node>& step_value_node() { return this->m_step_value_node; }
			inline const std::shared_ptr<Node>& body_node() { return this->m_body_node; }
			inline const bool& should_return_null() { return this->m_should_return_null; }

		};

	}

}