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

			struct if_case
			{
				Node condition;
				Node statements;
				bool unknown; // TODO: change name
			};

		private:
			std::vector<if_case> m_cases;
			std::optional<if_case> m_else_case;

		public:
			IfNode(std::vector<if_case> cases, std::optional<if_case> m_else_case = std::nullopt);

		};

	}

}