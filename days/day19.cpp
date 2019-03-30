#include "InputParser.h"
#include "Machine.h"

#include <vector>
#include <iostream>



int Part2() {
	
	/*
	 #ip 2
	 0  addi 2 16 2		r2 += 16						jump to 17
	 1  seti 1 1 5		r5 = 1						r5 = 1
	 2  seti 1 1 3		r3 = 1						r3 = 1
	 3  mulr 5 3 4 		r4 = r5*r3 					r4 = r5*r3
	 4  eqrr 4 1 4 		r4 = (r4 == r1) ? 1 : 0 		r4 = (r4 == r1) ? 1 : 0
	 5  addr 4 2 2 		r2 += r4 					jump by r4
	 6  addi 2 1 2		r2 += 1 						jump by 1
	 7  addr 5 0 0 		r0 += r5 					r0 += r5
	 8  addi 3 1 3 		r3 += 1 						r3 += 1
	 9  gtrr 3 1 4		r4 = (r3 > r1) ? 1 : 0  		r4 = (r3 > r1) ? 1 : 0
	 10 addr 2 4 2 		r2 += r4 					jump by r4
	 11 seti 2 8 2 		r2 = 2 						jump to 3
	 12 addi 5 1 5		r5 += 1 						r5 += 1
	 13 gtrr 5 1 4 		r4 = (r5 > r1) ? 1 : 0 		r4 = (r5 > r1) ? 1 : 0
	 14 addr 4 2 2 		r2 += r4						jump by r4
	 15 seti 1 5 2 		r2 = 1 						jump to 2
	 16 mulr 2 2 2 		r2 = r2*r2 					exit
	 17 addi 1 2 1 		r1 += 2  					r1 += 2
	 18 mulr 1 1 1 		r1 = r1*r1. 					r1 = r1*r1
	 19 mulr 2 1 1 		r1 = r2*r1. 					r1 = 19*r1
	 20 muli 1 11 1 		r1 *= 11 					r1 *= 11
	 21 addi 4 3 4 		r4 += 3 						r4 += 3
	 22 mulr 4 2 4 		r4 *= r2 					r4 *= 22
	 23 addi 4 7 4		r4 += 7 						r4 += 7
	 24 addr 1 4 1 		r1 += r4 					r1 += r4
	 25 addr 2 0 2  		r2 += r0 					jump by r0
	 26 seti 0 4 2 		r2 = 0 						jump to 1
	 27 setr 2 8 4 		r4 = r2 						r4 = 27
	 28 mulr 4 2 4		r4 *= r2 					r4 *= 28
	 29 addr 2 4 4 		r4 += r2 					r4 += 29
	 30 mulr 2 4 4 		r4 *= r2 					r4 *= 30
	 31 muli 4 14 4 		r4 *= 14 					r4 *= 14
	 32 mulr 4 2 4		r4 *= r2 					r4 *= 32
	 33 addr 1 4 1 		r1 += r4 					r1 += r4
	 34 seti 0 5 0 		r0 = 0 						r0 = 0
	 35 seti 0 8 2 		r2 = 0 						jump to 1
	 */
	
	int r0=1;
	int r1=0;
	//int r2=0;
	int r3=0;
	int r4=0;
	int r5=0;
	
	r1 += 2;
	r1 = r1*r1;
	r1 = 19*r1;
	r1 *= 11;
	r4 += 3;
	r4 *= 22;
	r4 += 7;
	r1 += r4;
	goto i27;
	goto i1;
i27: r4 = 27;
	r4 *= 28;
	r4 += 29;
	r4 *= 30;
	r4 *= 14;
	r4 *= 32;
	r1 += r4;
	r0 = 0;
	goto i1;
i1:
	
	r5 = 1;
	while (true) {
		r3 = 1;
		
		/*
		 while (true) {
		 if (r5*r3 == r1) {
		 r0 += r5;
		 }
		 
		 r3 += 1;
		 if (r3 > r1) {
		 break;
		 }
		 }
		 */
		
		// optimized the while loop above
		if ((r1 % r5) == 0) {
			r0 += r5;
		}
		// ^^
		
		r5 += 1;
		if (r5 > r1) {
			break;
		}
	}
	
	return r0;
}
int main(int argc, char* argv[]) {
	
	/*
	
	return 0;
	*/
	
	const auto inputLines = utils::getInputLines("inputs/day19.txt");

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
	
	while (true) {
		const auto nextInstruction = machine.GetNextInstruction();
		if (nextInstruction < 0 || nextInstruction >= instructions.size()) {
			break;
		}
		
		const auto& instruction = instructions[nextInstruction];
		machine.HandleInstruction(instruction);
	}
	
	std::cout << "Part 1: " << machine.registers[0] << std::endl;
	
	std::cout << "Part 2: " << Part2() << std::endl;
	
	return 0;
}
