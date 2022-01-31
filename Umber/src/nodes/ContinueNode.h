#pragma once

#include "../Node.h"
#include "../Position.h"

namespace umber
{

	namespace nodes
	{

		class ContinueNode : public Node
		{
		public:
			ContinueNode(Position pos_start, Position pos_end);
		};

	}

}