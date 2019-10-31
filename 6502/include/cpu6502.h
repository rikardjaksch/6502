#pragma once

#include <stdint.h>

typedef struct cpu6502_io cpu6502_io_t;

// State object representing the 6502 micro processor.
typedef struct cpu6502
{
	// Internal registers are publicly visible
	// mainly for tooling (debuggers etc).
	// Consumers are not meant to manually mutate these.
	uint8_t accumulator;
	uint8_t x_register;
	uint8_t y_register;
	uint8_t status_register;
	uint8_t stack_pointer;
	uint16_t program_counter;

	// Opaque pointer to internal data
	struct cpu6502_internal* internal;
} cpu6502_t;

// Enumeration of all bit flags in the status register
enum cpu6502_status_bit
{
	CPU_STATUS_CARRY = (1 << 0),
	CPU_STATUS_ZERO = (1 << 1),
	CPU_STATUS_IRQ_DISABLE = (1 << 2),
	CPU_STATUS_DECIMAL_MODE = (1 << 3),	
	CPU_STATUS_BREAK = (1 << 4),
	CPU_STATUS_UNUSED = (1 << 5),
	CPU_STATUS_OVERFLOW = (1 << 6),
	CPU_STATUS_NEGATIVE = (1 << 7),
};

// Creates an object to represent the 6502 micro processor.
cpu6502_t* cpu6502_create(cpu6502_io_t* io_interface);

// Cleans up any internal resources previosly allocated when
// the object was created.
void cpu6502_destroy(cpu6502_t* cpu);

// Instructs the cpu to write 'data' to 'address' over the internal
// IO interface.
void cpu6502_write_data(cpu6502_t* cpu, uint16_t address, uint8_t data);

// Instructs the cpu to read data from 'address' over the internal
// IO interface.
uint8_t cpu6502_read_data(cpu6502_t* cpu, uint16_t address);

// Sends a single clock pulse to the cpu.
// Majority of instructions do require several clock
// pulses to happen before the instructions finishes.
void cpu6502_clock(cpu6502_t* cpu);

// Utility function that will clock the cpu enough times
// to finish the execution of the current instruction.
// Mainly used for tools such as debuggers and internal
// unit tests.
// Returns the number of clock cycles used to step
// the instruction (doesn't not have to be the correct amount
// if we previosuly clocked half an instruction for example).
uint8_t cpu6502_step_instruction(cpu6502_t* cpu);

// Sends a reset interrupt to the cpu.
// This should always be the first call after creating
// a new cpu object before starting to execute proper code
// since it sets up the cpu state.
void cpu6502_reset_interrupt(cpu6502_t* cpu);