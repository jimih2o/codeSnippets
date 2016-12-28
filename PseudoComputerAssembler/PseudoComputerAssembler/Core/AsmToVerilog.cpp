#include "AsmToVerilog.hpp"

#include "Instruction.hpp"
#include "Utility.hpp"

#include <string>
#include <cctype>
#include <cstring>

namespace Core {
	void AsmAdjustGOTOs(std::string &fileStream)
	{
		std::string processed = fileStream ;
		while (!processed.empty()) {
			std::string line = Util::GetLine(processed, 0) ;
			processed.erase(0, line.size()) ;

			if (line.find(gInstructions[GOTO].mName) != std::string::npos) {
				// get tag sym
				std::string sym = Util::GetSymbol(line, 5) ;
				if (Util::IsNumber(sym)) continue ;

				// find tag
				int ramLocation = 0 ;
				int s = 0 ;
				for (;ramLocation < (64*1024);++ramLocation) {
					//          ASSERT(ramLocation < (64*1024) - 1, "Tag not found!") ;

					std::string nline = Util::GetLine(fileStream, s) ;

					if (nline.find(sym) == 0) {
						fileStream.replace(s, sym.size() + 1, "NOP") ;
						break ;
					}

					s += nline.size() ;
				}

				Util::SearchAndReplace(fileStream, sym, std::to_string(ramLocation)) ;
			}
		}
	}

	std::string AsmLineToROMAssign(std::string const &line, int lineNumber)
	{
		// format:
		// rom[__LINE__] = (INST << 11) | (PARAMS) ;

		std::string romLine = "rom[ " + std::to_string(lineNumber) + " ] = " ;
		std::string instruction = "" ;

		// assumes preprocessed
		int end_inst = 0 ;
		while (!std::isspace(line[end_inst])) ++end_inst ;
		instruction = line.substr(0, end_inst) ;
		Instruction const &instInfo = InstructionFromString(instruction) ;
		ERROR_IF(&instInfo == &BAD_INSTRUCTION, "Could not assemble code!") ;

		unsigned inst = instInfo.mHexCode << 11 ;

		if (end_inst < line.size()) {
			std::string param = Util::GetSymbol(line, end_inst + 1) ;
			int value = 0 ;
			if (param[0] == 'H') {
				std::sscanf(param.c_str(), "H%X", &value) ;
			} else if (param[0] == 'D') {
				std::sscanf(param.c_str(), "D%i", &value) ;
			} else {
				std::sscanf(param.c_str(), "%i", &value) ;
			}
			inst |= (*(unsigned short *)&value & 0x07FF) ;
		}

		romLine += std::to_string(inst) + ";\n" ;
		return romLine ;
	}

	void AsmToRomAssign(std::string &fileStream)
	{
		std::string processed = fileStream ;
		fileStream = "" ;
		int lineNumber = 0 ;
		while (!processed.empty()) {
			std::string line = Util::GetLine(processed, 0) ;
			processed.erase(0, line.size()) ;
			fileStream += AsmLineToROMAssign(line, lineNumber) ;
			++lineNumber ;
		}
	}
}