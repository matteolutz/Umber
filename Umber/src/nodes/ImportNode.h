#pragma once

#include "../Node.h"
#include "../Token.h"
#include "../Position.h"

namespace umber
{

	namespace nodes
	{

		class ImportNode : public Node
		{
		private:
			Token m_import_name;

		public:
			ImportNode(Token import_name, Position pos_start, Position pos_end);

			inline const Token& import_name() const { return this->m_import_name; }

		};

	}

}