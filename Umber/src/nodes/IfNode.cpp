#include "IfNode.h"

namespace umber
{

	namespace nodes
	{

		IfNode::IfNode(std::vector<if_case> cases, std::shared_ptr<Node> m_else_case)
			: Node(cases[0].condition->pos_start(), m_else_case != nullptr ? m_else_case->pos_end() : cases.back().condition->pos_end()) { }


	}

}