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

namespace umber
{

	void Test();

	void RunFile(std::string filepath);
	std::pair<std::optional<std::vector<Token>>, Error*> RunText(std::string text);

}