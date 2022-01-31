#pragma once

#include "../Node.h"

namespace umber
{

	namespace nodes
	{

		class WhileNode : public Node
		{

		private:
			Node m_condition_node;
			Node m_body_node;
			bool m_should_return_null;

		public:
			WhileNode(Node condition_node, Node body_node, bool should_return_null);

		};

	}

}