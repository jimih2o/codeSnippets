#pragma once

#include "Defs.hpp"
#include <string>

#include "Defs.hpp"

namespace Core {
	std::string AsmLineToROMAssign(std::string const &line, int lineNumber) ;
	void AsmAdjustGOTOs(std::string &fileStream) ;
	void AsmToRomAssign(std::string &fileStream) ;
}
