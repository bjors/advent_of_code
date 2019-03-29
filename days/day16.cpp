#include "InputParser.h"
#include "Machine.h"
#include <iostream>
#include <array>
#include <set>
#include <algorithm>
#include <tuple>





struct TestInput {
	Machine::Registers before;
	std::array<int, 4> instruction;
	Machine::Registers after;
};

std::set<Machine::OpCode> GetPossibleOpcodes(const TestInput& testInput) {
	std::set<Machine::OpCode> possibleOpCodes;
	for (int i=0; i<static_cast<int>(Machine::OpCode::NumOpcodes); ++i) {
		const auto opCode = static_cast<Machine::OpCode>(i);
		const Machine::Instruction testInstruction = {opCode, testInput.instruction[1], testInput.instruction[2], testInput.instruction[3]};
		if (Machine::HandleInstruction(testInstruction, testInput.before) == testInput.after) {
			possibleOpCodes.insert(opCode);
		}
	}
									  
  	return possibleOpCodes;
}

std::tuple<std::vector<TestInput>, std::vector<std::array<int, 4>>> ParseInput() {
	std::vector<TestInput> testInputs;
	std::vector<std::array<int, 4>> testProgram;
	const auto inputLines = utils::getInputLines("inputs/day16.txt");
	for (std::size_t i=0; i<inputLines.size(); ++i) {
		const auto& line = inputLines[i];
		if (line.empty()) {
			continue;
		} else if (line.find("Before") == 0) {
			TestInput input;
			sscanf(line.c_str(), "Before: [%d, %d, %d, %d]", &input.before[0], &input.before[1], &input.before[2], &input.before[3]);
			sscanf(inputLines[i+1].c_str(), "%d %d %d %d", &input.instruction[0], &input.instruction[1], &input.instruction[2], &input.instruction[3]);
			sscanf(inputLines[i+2].c_str(), "After: [%d, %d, %d, %d]", &input.after[0], &input.after[1], &input.after[2], &input.after[3]);
			
			testInputs.push_back(input);
			i+=3;
			continue;
		} else {
			std::array<int, 4> testInstruction;
			sscanf(line.c_str(), "%d %d %d %d", &testInstruction[0], &testInstruction[1], &testInstruction[2], &testInstruction[3]);
			testProgram.push_back(testInstruction);
		}
	}
	return {testInputs, testProgram};
}

std::array<Machine::OpCode, Machine::OpCode::NumOpcodes> VerifyOpcodes(std::array<std::set<Machine::OpCode>, static_cast<int>(Machine::OpCode::NumOpcodes)> possibleOpcodes) {
	std::array<Machine::OpCode, Machine::OpCode::NumOpcodes> verifiedOpCodes;
	while (true) {
		bool found = false;
		for (int i=0; i<possibleOpcodes.size(); ++i) {
			if (possibleOpcodes[i].size() == 1) {
				const auto opcode = *possibleOpcodes[i].begin();
				verifiedOpCodes[i] = opcode;
				
				for (auto& other : possibleOpcodes) {
					other.erase(opcode);
				}
				found = true;
				break;
			}
		}
		
		if (!found) {
			break;
		}
	}
	
	return verifiedOpCodes;
}

int main(int argc, char* argv[]) {

	const auto [testInputs, testProgram] = ParseInput();
	
	std::array<std::set<Machine::OpCode>, static_cast<int>(Machine::OpCode::NumOpcodes)> possibleOpcodes;

	int samplesWithThreeOrMoreOpCodes = 0;
	for (const auto& testInput : testInputs) {
		const auto opcodeValue = testInput.instruction[0];
		const auto possible = GetPossibleOpcodes(testInput);
		if (possible.size() >= 3) {
			samplesWithThreeOrMoreOpCodes++;
		}
		if (!possibleOpcodes[opcodeValue].empty()) {
			std::set<Machine::OpCode> intersection;
			std::set_intersection(possibleOpcodes[opcodeValue].begin(), possibleOpcodes[opcodeValue].end(),
								  possible.begin(), possible.end(),
								  std::inserter(intersection, intersection.begin()));
		} else {
			possibleOpcodes[opcodeValue] = possible;
		}		
	}
	
	std::cout << "Part 1: " << samplesWithThreeOrMoreOpCodes << std::endl;
	
	const auto& verifiedOpCodes = VerifyOpcodes(possibleOpcodes);

	Machine::Registers registers = {0,0,0,0};
	for (const auto& testLine : testProgram) {
		Machine::Instruction instruction{verifiedOpCodes[testLine[0]], testLine[1], testLine[2], testLine[3]};
		registers = Machine::HandleInstruction(instruction, registers);
	}
	
	std::cout << "Part 2: " << registers[0] << std::endl;
	
	return 0;
}
