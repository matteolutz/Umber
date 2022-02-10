#pragma once

#include "../Value.h"
#include "../Position.h"

#include <vector>

namespace umber
{

	namespace values
	{

		class ListValue : public Value
		{
		private:
			std::vector<std::shared_ptr<Value>> m_elements;

		public:
			ListValue(std::vector<std::shared_ptr<Value>> elements, Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			ListValue(std::vector<std::shared_ptr<Value>> elements);

			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> added_to(std::shared_ptr<Value> other) override;

			inline const std::vector<std::shared_ptr<Value>>& elements() const { return this->m_elements; }

			inline std::unique_ptr<Value> copy() const override { return std::make_unique<ListValue>(*this); }
			
			std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> access(std::string accessor) override;

			std::string as_string() const override;

		};

	}

}