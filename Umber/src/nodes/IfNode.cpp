#include "IfNode.h"

namespace umber
{

	namespace nodes
	{

		IfNode::IfNode(std::vector<if_case> cases, std::optional<if_else_case> else_case)
			: Node(cases[0].condition->pos_start(), m_else_case != std::nullopt ? m_else_case.value().statements->pos_end() : cases.back().condition->pos_end(), NodeType::If), m_cases(cases), m_else_case(else_case) { }


	}

}