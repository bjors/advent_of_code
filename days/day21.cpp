#include "InputParser.h"
#include "Machine.h"
#include <set>

int main(int argc, char* argv[]) {
	const auto inputLines = utils::getInputLines("inputs/day21.txt");
	
	int ipRegister;
	std::sscanf(inputLines.front().c_str(), "#ip %d", &ipRegister);
	
	Machine2 machine(ipRegister);

	std::vector<Machine2::Instruction> instructions;
	for (std::size_t i=1; i<inputLines.size(); ++i) {
		char instr[5] {};
		int a,b,c;
		std::sscanf(inputLines[i].c_str(), "%s %d %d %d", instr, &a, &b, &c);
		
		const auto opCode = machine.ToOpCode(instr);
		instructions.push_back({opCode, a, b, c});
	}
	
	std::set<int> r5Values;
	
	int lastUniqueValue = 0;
	
	while (true) {
		const auto nextInstruction = machine.GetNextInstruction();
		if (nextInstruction < 0 || nextInstruction >= instructions.size()) {
			break;
		}
		
		const auto& instruction = instructions[nextInstruction];
		
		if (instruction.opCode == MachineT<6>::OpCode::Eqrr) {
			const auto value = machine.registers[5];
			if (r5Values.find(value) != r5Values.end()) {
				std::cout << "Part 2: " << lastUniqueValue << std::endl;
				break;
			} else {
				if (r5Values.empty()) {
					std::cout << "Part 1: " << value << std::endl;
				}
				
				lastUniqueValue = value;
				r5Values.insert(value);
			}
		}
		
		machine.HandleInstruction(instruction);
	}
	
	return 0;
}

