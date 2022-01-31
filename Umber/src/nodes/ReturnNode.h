#pragma once

#include "../Node.h"
#include "../Position.h"

namespace umber
{
	
	namespace nodes
	{

		class ReturnNode : public Node
		{
		public:
			ReturnNode(Position pos_start, Position pos_end);
		};

	}

}