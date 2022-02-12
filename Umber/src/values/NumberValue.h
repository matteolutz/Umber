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
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> moduloed_by(std::shared_ptr<Value> other) override;
			
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_eq(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_gt(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_lt(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_gte(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_lte(std::shared_ptr<Value> other) override;

			inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> anded_by(std::shared_ptr<Value> other) override { return { std::make_unique<NumberValue>(this->is_true() && other->is_true() ? TRUE_VALUE : FALSE_VALUE), nullptr }; }
			inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> ored_by(std::shared_ptr<Value> other) override { return { std::make_unique<NumberValue>(this->is_true() || other->is_true() ? TRUE_VALUE : FALSE_VALUE), nullptr }; }
			inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> notted() override { return { std::make_unique<NumberValue>(!this->is_true() ? TRUE_VALUE : FALSE_VALUE), nullptr}; }

			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> bit_anded_by(std::shared_ptr<Value> other) override;
			std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> bit_ored_by(std::shared_ptr<Value> other) override;

			inline std::unique_ptr<Value> copy() const override { return std::make_unique<NumberValue>(*this); };

			inline bool is_true() override { return this->m_value != FALSE_VALUE; }

			inline std::string as_string() const override { return utils::std_string_format("%3.4f", this->m_value); }

		public:
			inline static const float NULL_VALUE = 0.0f;
			inline static const float FALSE_VALUE = 0.0f;
			inline static const float TRUE_VALUE = 1.0f;

		};

	}

}