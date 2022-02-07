#pragma once

#include "../Value.h"
#include "../Position.h"
#include "../utils/Utils.h"

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

			std::unique_ptr<Value> copy() override;

			inline std::string as_string() const override { return utils::std_string_format("%3.4f", this->m_value); }

		public:
			inline static const float NULL_VALUE = 0.0f;
			inline static const float FALSE_VALUE = 0.0f;
			inline static const float TRUE_VALUE = 1.0f;

		};

	}

}