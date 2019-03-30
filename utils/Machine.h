#pragma once

#include <array>
#include <iostream>

template <int NumRegisters = 4>
struct MachineT {
	using Registers = std::array<int, NumRegisters>;
	
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

using Machine = MachineT<4>;

struct Machine2 : public MachineT<6> {
	Machine2(const int ipRegister)
	:ipRegister(ipRegister) {
		
	}
	
	int GetNextInstruction() const {
		return registers[ipRegister];
	}
	
	
	void PrintRegisters() const {
		std::cout << " [";
		for (const auto& r : registers) {
			std::cout << r << ",";
		}
		std::cout << "]";
	}
	
	
	OpCode ToOpCode(const std::string& instructionName) const {
		if (instructionName == "seti") {
			return OpCode::Seti;
		} else if (instructionName == "addi") {
			return OpCode::Addi;
		} else if (instructionName == "addr") {
			return OpCode::Addr;
		} else if (instructionName == "setr") {
			return OpCode::Setr;
		} else if (instructionName == "mulr") {
			return OpCode::Mulr;
		} else if (instructionName == "muli") {
			return OpCode::Muli;
		} else if (instructionName == "eqir") {
			return OpCode::Eqir;
		} else if (instructionName == "eqri") {
			return OpCode::Eqri;
		} else if (instructionName == "eqrr") {
			return OpCode::Eqrr;
		} else if (instructionName == "gtir") {
			return OpCode::Gtir;
		} else if (instructionName == "gtri") {
			return OpCode::Gtri;
		} else if (instructionName == "gtrr") {
			return OpCode::Gtrr;
		} else if (instructionName == "bani") {
			return OpCode::Bani;
		} else if (instructionName == "banr") {
			return OpCode::Banr;
		} else if (instructionName == "borr") {
			return OpCode::Borr;
		} else if (instructionName == "bori") {
			return OpCode::Bori;
		} else {
			return OpCode::NumOpcodes;
		}
	}
	
	void HandleInstruction(const Instruction& instruction) {
		registers = MachineT<6>::HandleInstruction(instruction, registers);
		
		//PrintRegisters();
		registers[ipRegister]++;
	}
	
	const int ipRegister;
};
