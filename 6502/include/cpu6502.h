#pragma once

#include <stdint.h>
#include <stdbool.h>

// Utility macro to handle bytes and words
#define high_byte(word) (uint8_t)(word >> 8)
#define low_byte(word) (uint8_t)(word & 0x00FF)
#define bytes_to_word(h, l) ((uint16_t)((h) << 8 ) | (l))

typedef struct cpu6502_io cpu6502_io_t;

enum
{
	// The stack resides between 0x1000 and 0x1FF
	// (stack pointer is an offset from the base).
	CPU_6502_STACK_BASE = 0x1000,

	// High and low part of the address that the reset interrupt
	// will use to feed the program counter.
	CPU_6502_RESET_VECTOR_LOW = 0xFFFC,
	CPU_6502_RESET_VECTOR_HIGH = 0xFFFD,
};

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

// Similar to cpu6502_write_data but uses a pre-defined address.
// Will also update the stack pointer to a correct value.
void cpu6502_push_stack(cpu6502_t* cpu, uint8_t data);

// Instructs the cpu to read data from 'address' over the internal
// IO interface.
uint8_t cpu6502_read_data(cpu6502_t* cpu, uint16_t address);

// Similar to cpu6502_read_data but uses a pre-defined address.
// Will also update the stack pointer to a correct value.
uint8_t cpu6502_pop_stack(cpu6502_t* cpu);

// Sets the specífied status bit in the status register of the specified cpu
// to either '1' or '0' based on if 'toogle' is true or false.
void cpu6502_set_status_bit(cpu6502_t* cpu, enum cpu6502_status_bit bit, bool toogle);

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