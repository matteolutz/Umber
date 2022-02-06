#pragma once

#include "Position.h"
#include "Context.fwd.h"

#include "errors/RuntimeError.h"
#include "result/RuntimeResult.h"

#include <optional>
#include <vector>

namespace umber
{

	class Value
	{
	private:
		Position m_pos_start, m_pos_end;
		std::shared_ptr<Context> m_context;

	protected:
		Value(Position pos_start, Position pos_end, std::shared_ptr<Context> context);

	public:
		virtual ~Value() = 0;

		std::unique_ptr<errors::RuntimeError> illegal_operation(std::shared_ptr<Value> other = nullptr) const;

		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> added_to(std::shared_ptr<Value> other) { return {nullptr, this->illegal_operation(other)}; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> subbed_by(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> multed_by(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> dived_by(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> powed_by(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }

		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_eq(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_ne(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_lt(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_gt(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_lte(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> comparison_gte(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }

		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> anded_by(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> ored_by(std::shared_ptr<Value> other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> notted() { return { nullptr, this->illegal_operation() }; }

		virtual inline result::RuntimeResult execute(std::vector<std::shared_ptr<Value>> args) {
			auto res = result::RuntimeResult();
			res.failure(this->illegal_operation());
			return res;
		}

		virtual inline bool is_true() { return false; }

		virtual inline std::pair<std::unique_ptr<Value>, std::unique_ptr<errors::RuntimeError>> subscribe(std::shared_ptr<Value> index) { return { nullptr, this->illegal_operation(index) }; }

		inline const Position& pos_start() const { return this->m_pos_start; }
		inline const Position& pos_end() const { return this->m_pos_end; }

		inline std::shared_ptr<Context>& context() { return this->m_context; }

	};

}