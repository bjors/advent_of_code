#pragma once

#include <array>

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
