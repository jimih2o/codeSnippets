/* Start Header -------------------------------------------------------
File Name: FileStream.cpp
Purpose: Contains definition for FileStream
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "FileStream.hpp"

#include <fstream>
#include <sstream>

void FileStream::Parse(const string &file) {
	std::ifstream f(file) ;
	string line ;
	while (std::getline(f, line)) {
		buffer.push_back(line) ;
	}

	bool in_block = false ;
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		if (!in_block) {
			if (it->length()) {
				elements.push_back(MakePair(*it, it)) ;
				in_block = true ;
			}
		} else if (it->find("}") != string::npos) in_block = false ;
	}
}

void FileStream::ReadSet(const string &block, uint32 element, float32 &u, float32 &v) {
	Array<string>::iterator location = buffer.end() ;
	for (auto pair : elements) {
		if (pair.first == block) {
			location = pair.second ;
			break ;
		}
	}
	
	int32 offset = -2 ; // block ident, open {
	for (;location != buffer.end() && location->find("}") == string::npos; ++location) {
		if (uint32(++offset) == element) {
			string::size_type n = location->find_first_of(",") ;
			string sub_str = location->substr(0, n), rest = location->substr(n + 1, string::npos) ;
			std::stringstream ss ;
			ss << sub_str ;
			ss >> u ;
			
			n = rest.find_first_of(",") ;
			sub_str = rest.substr(0, n) ;
			ss.clear() ;
			ss << sub_str ;
			ss >> v ;
			break ;
		}
	}
}

void FileStream::ReadSet(const string &block, uint32 element, uint32 &i1, uint32 &i2, uint32 &i3) {
	Array<string>::iterator location = buffer.end() ;
	for (auto pair : elements) {
		if (pair.first == block) {
			location = pair.second ;
			break ;
		}
	}
	
	int32 offset = -2 ; // block ident, open {
	for (;location != buffer.end() && location->find("}") == string::npos; ++location) {
		if (uint32(++offset) == element) {
			string::size_type n = location->find_first_of(",") ;
			string sub_str = location->substr(0, n), rest = location->substr(n + 1, string::npos) ;
			std::stringstream ss ;
			ss << sub_str ;
			ss >> i1 ;
			
			n = rest.find_first_of(",") ;
			sub_str = rest.substr(0, n) ;
			ss.clear() ;
			ss << sub_str ;
			ss >> i2 ;

			rest = rest.substr(n + 1, string::npos) ;
			
			n = rest.find_first_of(",") ;
			sub_str = rest.substr(0, n) ;
			ss.clear() ;
			ss << sub_str ;
			ss >> i3 ;
			break ;
		}
	}
}
void FileStream::ReadSet(const string &block, uint32 element, float32 &x, float32 &y, float32 &z, float32 &w) {
	Array<string>::iterator location = buffer.end() ;
	for (auto pair : elements) {
		if (pair.first == block) {
			location = pair.second ;
			break ;
		}
	}
	
	int32 offset = -2 ; // block ident, open {
	for (;location != buffer.end() && location->find("}") == string::npos; ++location) {
		if (uint32(++offset) == element) {
			string::size_type n = location->find_first_of(",") ;
			string sub_str = location->substr(0, n), rest = location->substr(n + 1, string::npos) ;
			std::stringstream ss ;
			ss << sub_str ;
			ss >> x ;
			
			n = rest.find_first_of(",") ;
			sub_str = rest.substr(0, n) ;
			ss.clear() ;
			ss << sub_str ;
			ss >> y ;

			rest = rest.substr(n + 1, string::npos) ;
			
			n = rest.find_first_of(",") ;
			sub_str = rest.substr(0, n) ;
			ss.clear() ;
			ss << sub_str ;
			ss >> z ;

			rest = rest.substr(n + 1, string::npos) ;
			
			n = rest.find_first_of(",") ;
			sub_str = rest.substr(0, n) ;
			ss.clear() ;
			ss << sub_str ;
			ss >> w ;
			break ;
		}
	}
}	

void FileStream::GetVarBlock(const string &block, const string &var_name, float32 &store) {
	Array<string>::iterator location = buffer.end() ;
	for (auto pair : elements) {
		if (pair.first == block) {
			location = pair.second ;
			break ;
		}
	}

	for (;location != buffer.end() && location->find("}") == string::npos; ++location) {
		if (uint32 loc = location->find(var_name) != string::npos) {
			std::stringstream ss ;
			ss << location->substr(loc + var_name.size() + 1, string::npos) ;
			ss >> string() ; // discard '=' + whitespace
			ss >> store ;
			return ;
		}
	}
}