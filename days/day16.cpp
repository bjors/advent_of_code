#include "InputParser.h"
#include <iostream>
#include <array>
#include <set>
#include <algorithm>
#include <tuple>

enum OpCode {
	Addr, // (add register) stores into register C the result of adding register A and register B.
	Addi, // (add immediate) stores into register C the result of adding register A and value B.
	Mulr, // (multiply register) stores into register C the result of multiplying register A and register B.
	Muli, // (multiply immediate) stores into register C the result of multiplying register A and value B.
	Banr, // (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
	Bani, // (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
	Borr, // (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
	Bori, // (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
	Setr, // (set register) copies the contents of register A into register C. (Input B is ignored.)
	Seti, // (set immediate) stores value A into register C. (Input B is ignored.)
	Gtir, // (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
	Gtri, // (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
	Gtrr, // (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
	Eqir, // (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
	Eqri, // (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
	Eqrr, // (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
	
	NumOpcodes
};

struct Instruction {
	OpCode opCode;
	int a;
	int b;
	int c;
};

struct Machine {
	using Registers = std::array<int, 4>;
	
	static Registers HandleInstruction(const Instruction instruction, Registers registers) {
		switch (instruction.opCode) {
			case Addr: {
				registers[instruction.c] = registers[instruction.a] + registers[instruction.b];
				break;
			}
			case Addi: {
				registers[instruction.c] = registers[instruction.a] + instruction.b;
				break;
			}
			case Mulr: {
				registers[instruction.c] = registers[instruction.a] * registers[instruction.b];
				break;
			}
			case Muli: {
				registers[instruction.c] = registers[instruction.a] * instruction.b;
				break;
			}
			case Banr: {
				registers[instruction.c] = registers[instruction.a] & registers[instruction.b];
				break;
			}
			case Bani: {
				registers[instruction.c] = registers[instruction.a] & instruction.b;
				break;
			}
			case Borr: {
				registers[instruction.c] = registers[instruction.a] | registers[instruction.b];
				break;
			}
			case Bori: {
				registers[instruction.c] = registers[instruction.a] | instruction.b;
				break;
			}
			case Setr: {
				registers[instruction.c] = registers[instruction.a];
				break;
			}
			case Seti: {
				registers[instruction.c] = instruction.a;
				break;
			}
			case Gtir: {
				registers[instruction.c] = (instruction.a > registers[instruction.b]) ? 1 : 0;
				break;
			}
			case Gtri: {
				registers[instruction.c] = (registers[instruction.a] > instruction.b) ? 1 : 0;
				break;
			}
			case Gtrr: {
				registers[instruction.c] = (registers[instruction.a] > registers[instruction.b]) ? 1 : 0;
				break;
			}
			case Eqir: {
				registers[instruction.c] = (instruction.a == registers[instruction.b]) ? 1 : 0;
				break;
			}
			case Eqri: {
				registers[instruction.c] = (registers[instruction.a] == instruction.b) ? 1 : 0;
				break;
			}
			case Eqrr: {
				registers[instruction.c] = (registers[instruction.a] == registers[instruction.b]) ? 1 : 0;
				break;
			}
			
			case NumOpcodes: {
				break;
			}
		}
		return registers;
	}
	
	Registers registers;
};

struct TestInput {
	Machine::Registers before;
	std::array<int, 4> instruction;
	Machine::Registers after;
};

std::set<OpCode> GetPossibleOpcodes(const TestInput& testInput) {
	std::set<OpCode> possibleOpCodes;
	for (int i=0; i<static_cast<int>(OpCode::NumOpcodes); ++i) {
		const auto opCode = static_cast<OpCode>(i);
		const Instruction testInstruction = {opCode, testInput.instruction[1], testInput.instruction[2], testInput.instruction[3]};
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

std::array<OpCode, OpCode::NumOpcodes> VerifyOpcodes(std::array<std::set<OpCode>, static_cast<int>(OpCode::NumOpcodes)> possibleOpcodes) {
	std::array<OpCode, OpCode::NumOpcodes> verifiedOpCodes;
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
	
	std::array<std::set<OpCode>, static_cast<int>(OpCode::NumOpcodes)> possibleOpcodes;

	int samplesWithThreeOrMoreOpCodes = 0;
	for (const auto& testInput : testInputs) {
		const auto opcodeValue = testInput.instruction[0];
		const auto possible = GetPossibleOpcodes(testInput);
		if (possible.size() >= 3) {
			samplesWithThreeOrMoreOpCodes++;
		}
		if (!possibleOpcodes[opcodeValue].empty()) {
			std::set<OpCode> intersection;
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
		Instruction instruction{verifiedOpCodes[testLine[0]], testLine[1], testLine[2], testLine[3]};
		registers = Machine::HandleInstruction(instruction, registers);
	}
	
	std::cout << "Part 2: " << registers[0] << std::endl;
	
	return 0;
}
