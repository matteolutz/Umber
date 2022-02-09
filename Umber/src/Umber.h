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

	void Test();

	void RunFile(std::string filepath);
	std::pair<std::optional<std::vector<Token>>, std::unique_ptr<Error>> RunText(std::string text);

}