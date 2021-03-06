#pragma once

#include "../Value.h"
#include "NumberValue.h"
#include "StringValue.h"
#include "../Position.h"
#include "../Token.h"

#include <map>

namespace umber
{

	namespace values
	{

		class DictValue : public Value
		{
		private:
			std::map<std::string, std::shared_ptr<Value>> m_elements;

		public:
			DictValue(std::map<std::string, std::shared_ptr<Value>>, Position pos_start, Position pos_end, std::shared_ptr<Context> context);
			DictValue(std::map<std::string, std::shared_ptr<Value>>);

			inline const std::map<std::string, std::shared_ptr<Value>>& elements() const { return this->m_elements; }

			inline std::unique_ptr<Value> copy() const override { return std::make_unique<DictValue>(*this); }

			std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> access(std::shared_ptr<Value> accessor) override;
			std::pair<std::shared_ptr<Value>, std::unique_ptr<errors::RuntimeError>> set(std::shared_ptr<Value> accessor, std::shared_ptr<Value> value) override;

			std::string as_string() const override;

		};

	}

}