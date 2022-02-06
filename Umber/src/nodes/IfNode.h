#pragma once

#include "../Node.h"
#include "../Token.h"

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
				std::shared_ptr<Node> condition;
				std::shared_ptr<Node> statements;
				bool should_return_null;
			};

		private:
			std::vector<if_case> m_cases;
			std::shared_ptr<Node> m_else_case;

		public:
			IfNode(std::vector<if_case> cases, std::shared_ptr<Node> m_else_case = nullptr);

		};

	}

}