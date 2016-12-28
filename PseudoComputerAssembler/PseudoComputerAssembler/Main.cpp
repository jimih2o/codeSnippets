
#include "Defs.hpp"

#include <cstdio>
#include <string>
#include <fstream>

#include "PreProcessor\MetaDriver.hpp"
#include "Core\AsmToVerilog.hpp" 

static void PrintUsage(char const *execName)
{
	std::printf("%s\n", execName) ;
	std::printf("Usage:\n") ;
	std::printf("FileName [-flags] [--options]") ;
	std::printf("\n\n") ;
}

static std::string FileToStringStream(std::string const &fileName)
{ 
	std::string fileStream = "" ;
	std::ifstream file(fileName.c_str()) ;

	ASSERT(file.good(), "Could not open file!") ;

	while (file.good()) {
		std::string line ;
		std::getline(file, line) ;
		fileStream += line + "\n" ;
	}
	return fileStream ;
}

static void RunPreProcessor(std::string &fileStream)
{
	PreProcess::StripComments(fileStream) ;
	PreProcess::CaseDesensitize(fileStream) ;

	// LET X VAL
	PreProcess::HandleDefinitions(fileStream) ;

	PreProcess::StripEmptyLines(fileStream) ;
}

static void RunAssembler(std::string &fileStream)
{
	Core::AsmAdjustGOTOs(fileStream) ;
	Core::AsmToRomAssign(fileStream) ;

}

static void StringToFile(std::string const &fileStream, std::string const &fileName)
{
	std::ofstream file(fileName) ;
	file << fileStream ;
}

int main(int argc, char **argv)
{
	// check command line
	if (argc > 1) {
		// commands were passed

		// TODO: Parse options, etc
		std::string fileName = argv[1] ;
		std::string fileStream = FileToStringStream(fileName) ;

		RunPreProcessor(fileStream) ;
		RunAssembler(fileStream) ;

		// tje = "Tyler Jimi Experience"
		StringToFile(fileStream, fileName + ".tje") ;

	} else {
		// print usage
		PrintUsage(argv[0]) ;
	}

	return 0 ;
}