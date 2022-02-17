#pragma once

#include <stdio.h>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <optional>

#include "Position.h"
#include "Token.h"
#include "Lexer.h"
#include "Error.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "Interpreter.h"

#include "values/BuiltInFunction.h"

namespace umber
{

	std::pair<std::shared_ptr<Value>, std::shared_ptr<Error>> run_text(const std::string &text);

}