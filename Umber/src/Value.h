#pragma once

#include "Position.h"
#include "Context.h"

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
		Context* m_context;

	protected:
		Value(Position pos_start, Position pos_end, Context* context);

	public:
		errors::RuntimeError* illegal_operation(Value* other = nullptr) const;

		virtual inline std::pair<Value*, errors::RuntimeError*> added_to(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> subbed_by(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> multed_by(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> dived_by(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> powed_by(Value* other) { return { nullptr, this->illegal_operation(other) }; }

		virtual inline std::pair<Value*, errors::RuntimeError*> comparison_eq(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> comparison_ne(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> comparison_lt(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> comparison_gt(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> comparison_lte(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> comparison_gte(Value* other) { return { nullptr, this->illegal_operation(other) }; }

		virtual inline std::pair<Value*, errors::RuntimeError*> anded_by(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> ored_by(Value* other) { return { nullptr, this->illegal_operation(other) }; }
		virtual inline std::pair<Value*, errors::RuntimeError*> notted() { return { nullptr, this->illegal_operation() }; }

		virtual inline result::RuntimeResult execute(std::vector<Value*> args) {
			auto res = result::RuntimeResult();
			res.failure(this->illegal_operation());
			return res;
		}

		virtual inline bool is_true() { return false; }

		virtual inline std::pair<Value*, errors::RuntimeError*> subscribe(Value* index) { return { nullptr, this->illegal_operation(index) }; }

		inline const Position& pos_start() const { return this->m_pos_start; }
		inline const Position& pos_end() const { return this->m_pos_end; }

		inline const Context* context() const { return this->m_context; }
		inline Context*& context() { return this->m_context; }

	};

}