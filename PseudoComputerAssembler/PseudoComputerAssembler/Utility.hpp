#pragma once

#include <string>
#include <cctype>
#include <algorithm>

namespace Util {
	static std::string GetLine(std::string const &stream, int iPos)
	{
		std::string line = "" ;
		while (iPos < stream.size()) {
			line.push_back(stream[iPos]) ;
			if (stream[iPos] == '\n') return line ;
			++iPos ;
		}
		return line ;
	}

	static void StripLeadingSpace(std::string &line)
	{
		int iBeg = 0 ;
		while (std::isspace(line[iBeg]) && line[iBeg] != '\n') ++iBeg ;
		line = line.substr(iBeg, std::string::npos) ;
	}

	static std::string GetSymbol(std::string const &line, int iPos)
	{
		std::string sym = "" ;
		while (!std::isspace(line[iPos]) && iPos < line.size()) sym.push_back(line[iPos++]) ;
		return sym ;
	}

	static int SearchAndReplace(std::string &fileStream, std::string const &phrase, std::string const &what)
	{
		int i = 0 ;
		int ret = 0 ;
		while ((i = fileStream.find(phrase)) != std::string::npos) {
			fileStream.replace(i, phrase.size(), what) ;
			ret = i ;
		}
		return ret ;
	}

	static bool IsNumber(std::string const & s)
	{
		return !s.empty() && 
			std::find_if(
			s.begin(), 
			s.end(), 
			[](char c) { 
				return !std::isdigit(c); 
		}) == s.end() ;
	} 
}