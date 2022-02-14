#pragma once

#include "../Node.h"
#include "../Token.h"

namespace umber
{

	namespace nodes
	{

		class AccessorNode : public Node
		{
		private:
			std::shared_ptr<Node> m_node;
			std::shared_ptr<Node> m_accessor_node;

		public:
			AccessorNode(std::shared_ptr<Node> node, std::shared_ptr<Node> accessor_node);

			inline const std::shared_ptr<Node>& node() const { return this->m_node; }
			inline const std::shared_ptr<Node>& accessor_node() const { return this->m_accessor_node; }

		};

	}

}