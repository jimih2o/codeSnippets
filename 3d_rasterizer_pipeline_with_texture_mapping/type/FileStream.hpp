/* Start Header -------------------------------------------------------
File Name: FileStream.hpp
Purpose: Contains declaration for FileStream
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "Type.hpp"
#include "Array.hpp"

class FileStream {
	Array<string> buffer ;
	Array< Pair< string, Array<string>::iterator > > elements ;
public:
	void Parse(const string &file) ;

	void GetVarBlock(const string &block, const string &var_name, float32 &store) ;
	void ReadSet(const string &block, uint32 element, float32 &x, float32 &y, float32 &z, float32 &w) ;
	void ReadSet(const string &block, uint32 element, uint32 &i1, uint32 &i2, uint32 &i3) ;
	void ReadSet(const string &block, uint32 element, float32 &u, float32 &v) ;
};