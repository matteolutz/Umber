#pragma once

#include "../Node.h"

namespace umber
{

	namespace nodes
	{

		class WhileNode : public Node
		{

		private:
			std::shared_ptr<Node> m_condition_node;
			std::shared_ptr<Node> m_body_node;
			bool m_should_return_null;

		public:
			WhileNode(std::shared_ptr<Node> condition_node, std::shared_ptr<Node> body_node, bool should_return_null);

			inline const std::shared_ptr<Node>& condition_node() const { return this->m_condition_node; }
			inline const std::shared_ptr<Node>& body_node() const { return this->m_body_node; }
			inline const bool& should_return_null() const { return this->m_should_return_null; }

		};

	}

}