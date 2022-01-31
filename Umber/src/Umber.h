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

namespace umber
{

	void Test();

	void RunFile(std::string filepath);
	std::tuple<std::optional<std::vector<Token>>, std::optional<Error>> RunText(std::string text);

}