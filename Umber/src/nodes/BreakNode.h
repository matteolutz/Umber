#pragma once

#include "../Node.h"
#include "../Position.h"

namespace umber
{

	namespace nodes
	{

		class BreakNode : public Node
		{
		public:
			BreakNode(Position pos_start, Position pos_end);
		};

	}

}