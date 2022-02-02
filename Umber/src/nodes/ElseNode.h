#pragma once

#include "../Node.h"

namespace umber
{

	namespace nodes
	{

		class ElseNode : public Node
		{

		private:
			Node m_statements;
			bool m_bool_return;

		public:
			ElseNode(Node statements, bool bool_return);

		};

	}

}