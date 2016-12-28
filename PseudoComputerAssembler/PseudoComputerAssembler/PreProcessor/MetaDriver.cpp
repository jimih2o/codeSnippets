#include "MetaDriver.hpp"

#include "Utility.hpp"

#include <cctype>

namespace PreProcess {
	using namespace Util ;
	void StripComments(std::string &fileStream)
	{
		std::string commentStripped = "" ;
		for (int i = 0; i < fileStream.size(); ++i) {
			if (fileStream[i] == ';') {
				// find end of line
				int len = 1 ;
				while (fileStream[i + len] != '\n') {
					++len ;
				}
				// increment location
				i += len ;

				commentStripped.push_back('\n') ;
			} else {
				commentStripped.push_back(fileStream[i]) ;
			}
		}
		fileStream = commentStripped ;
	}

	void HandleDefinitions(std::string &fileStream)
	{
		// Look for lets
		std::string nodefs = "" ;
		for (int i = 0; i < fileStream.size();) {
			std::string line = GetLine(fileStream, i) ;
			i += line.size() ;

			StripLeadingSpace(line) ;

			// LET_N_0
			if (line.size() >= 7) {
				// this was actually the "best" way beleive it or not 
				if (line[0] == 'L' && line[1] == 'E' && line[2] == 'T' && line[3] == ' ') {
					std::string symbol = GetSymbol(line, 4) ;
					std::string rep = GetSymbol(line, 5 + symbol.size()) ;

					i = fileStream.find(line) ;

					// search & replace
					SearchAndReplace(fileStream, symbol, rep) ;

					// remove line from fileStream 
					fileStream.erase(i, 5 + 2 * rep.size()) ;

					continue ;
				}
			}

			nodefs += line ;
		}

		fileStream = nodefs ;
	}

	void CaseDesensitize(std::string &fileStream)
	{
		for (char &c : fileStream) {
			c = std::toupper(c) ;
		}
	}

	static void StripAllSpaces(std::string &line)
	{
		int iBeg = 0 ;
		while (std::isspace(line[iBeg])) ++iBeg ;
		line = line.substr(iBeg, std::string::npos) ;
	}

	void StripEmptyLines(std::string &fileStream)
	{
		std::string noempty = "" ;
		for (int i = 0; i < fileStream.size();) {
			std::string line = GetLine(fileStream, i) ;
			i += line.size() ;

			StripAllSpaces(line) ;

			noempty += line ;
		}
		fileStream = noempty ;
	}
}