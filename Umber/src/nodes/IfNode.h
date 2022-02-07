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

			struct if_else_case
			{
				std::shared_ptr<Node> statements;
				bool should_return_null;
			};

		private:
			std::vector<if_case> m_cases;
			std::optional<if_else_case> m_else_case;

		public:
			IfNode(std::vector<if_case> cases, std::optional<if_else_case> else_case = std::nullopt);

			inline const std::vector<if_case>& cases() const { return this->m_cases; }
			inline const std::optional<if_else_case>& else_case() const { return this->m_else_case; }

		};

	}

}