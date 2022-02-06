#pragma once

#include "../Value.h"
#include "../Position.h"

namespace umber
{

	namespace values
	{

		class NumberValue : public Value
		{
		private:
			float m_value;

		public:
			NumberValue(float value, Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			NumberValue(float value);

			inline const float& value() const { return this->m_value; }

			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> added_to(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> subbed_by(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> multed_by(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> dived_by(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> powed_by(std::shared_ptr<Value> other) override;

		};

	}

}