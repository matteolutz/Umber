#pragma once

#include "../Node.h"
#include "../Position.h"
#include "../Token.h"

#include <map>
#include <string>

namespace umber
{

	namespace nodes
	{

		class DictNode : public Node
		{
		private:
			std::map<std::string, std::shared_ptr<Node>> m_elements;

		public:
			DictNode(std::map<std::string, std::shared_ptr<Node>> elements, Position pos_start, Position pos_end);

			inline const std::map<std::string, std::shared_ptr<Node>>& elements() const { return this->m_elements; }

		};

	}

}