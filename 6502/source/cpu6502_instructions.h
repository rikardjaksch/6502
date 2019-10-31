#pragma once

#include <stdint.h>

typedef struct cpu6502 cpu6502_t;

typedef uint8_t(*addr_mode)(cpu6502_t* cpu);
typedef uint8_t(*instr_operation)(cpu6502_t* cpu);

typedef struct instruction
{
	// Base cost of cycles for this instruction
	// (Can be modified based on certain actions of the instruction)
	uint8_t cycle_cost;

	// User-friendly name of this instruction
	const char* mnemonic;

	// Logic for handling the addressing mode of this instruction
	addr_mode addressing_mode;

	// Logic for handling the operation of this instruction
	instr_operation operation;
} instruction_t;

// Linear lookup table for all possible 256 instructions.
// Note that some elements will be null-pointers since not
// all 256 possible instructions are valid.
extern instruction_t* instruction_table;