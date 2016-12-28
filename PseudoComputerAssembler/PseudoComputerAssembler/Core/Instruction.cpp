#include "Instruction.hpp"

InstructionIndex InstructionStringToIndex(std::string const &string)
{
	// this will get optimized eventually
	for (auto instruction : gInstructions) {
		if (instruction.mName == string) {
			return instruction.mIndex ;
		}
	}
	return (InstructionIndex)(-1) ;
}

Instruction const &InstructionFromString(std::string const &string)
{
	auto index = InstructionStringToIndex(string) ;
	if (index == (InstructionIndex)(-1)) return BAD_INSTRUCTION ;
	return gInstructions[index] ;
}