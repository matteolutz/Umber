#include "IfNode.h"

namespace umber
{

	namespace nodes
	{

		IfNode::IfNode(std::vector<if_case> cases, std::optional<if_case> m_else_case)
			: Node(cases[0].condition.pos_start(), m_else_case.has_value() ? m_else_case.value().condition.pos_end() : cases.back().condition.pos_end()) { }

	}

}