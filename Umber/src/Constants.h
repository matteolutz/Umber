#pragma once

#include <vector>
#include <map>

namespace umber
{

	namespace constants
	{

		static const std::map<char, char> ESCAPED_CHARACTERS = {
			{ 'n', '\n' },
			{ 't', '\t' }
		};

	}

}