#pragma once

#include "../Node.h"
#include "../Token.h"

#include "ElseNode.h"

#include <vector>

namespace umber
{

	namespace nodes
	{

		class IfNode : public Node
		{
		public:
			struct if_case
			{
				Node condition;
				Node statements;
				bool should_return_null;
			};

		private:
			std::vector<if_case> m_cases;
			std::optional<Node> m_else_case;

		public:
			IfNode(std::vector<if_case> cases, std::optional<Node> m_else_case = std::nullopt);

		};

	}

}