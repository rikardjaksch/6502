#include "cpu6502.h"
#include "cpu6502_io.h"
#include "cpu6502_instructions.h"
#include <stdlib.h>
#include <assert.h>

// Internal cpu implementation data that we want to hide from
// the public API.
typedef struct cpu6502_internal
{
	cpu6502_io_t* io_interface;
	uint8_t current_cycles;
	uint32_t total_cycles;
} cpu6502_internal_t;

cpu6502_t* cpu6502_create(cpu6502_io_t* io_interface)
{
	cpu6502_t* cpu = (cpu6502_t*)malloc(sizeof(cpu6502_t));
	if (cpu == NULL)
		return cpu;

	cpu->internal = (cpu6502_internal_t*)malloc(sizeof(cpu6502_internal_t));
	if (cpu->internal == NULL)
	{
		free(cpu);
		return NULL;
	}

	cpu->internal->io_interface = io_interface;
	cpu->internal->current_cycles = 0;
	cpu->internal->total_cycles = 0;

	return cpu;
}

void cpu6502_destroy(cpu6502_t* cpu)
{
	free(cpu);
}

void cpu6502_write_data(cpu6502_t* cpu, uint16_t address, uint8_t data)
{
	assert(cpu && cpu->internal);
	cpu6502_io_write(cpu->internal->io_interface, address, data);
}

void cpu6502_push_stack(cpu6502_t* cpu, uint8_t data)
{
	assert(cpu);
	cpu6502_write_data(cpu, CPU_6502_STACK_BASE + cpu->stack_pointer, data);
	cpu->stack_pointer--;
}

uint8_t cpu6502_read_data(cpu6502_t* cpu, uint16_t address)
{
	assert(cpu && cpu->internal);
	return cpu6502_io_read(cpu->internal->io_interface, address);
}

uint8_t cpu6502_pop_stack(cpu6502_t* cpu)
{
	assert(cpu);
	cpu->stack_pointer++;
	return cpu6502_read_data(cpu, CPU_6502_STACK_BASE + cpu->stack_pointer);
}

void cpu6502_set_status_bit(cpu6502_t* cpu, enum cpu6502_status_bit bit, bool toogle)
{
	assert(cpu);
	if (toogle)
		cpu->status_register |= bit;
	else
		cpu->status_register &= ~bit;
}

void cpu6502_clock(cpu6502_t* cpu)
{
	if (!cpu)
		return;

	if (cpu->internal->current_cycles == 0)
	{
		uint8_t opcode = cpu6502_read_data(cpu, cpu->program_counter++);
		instruction_t i = instruction_table[opcode];

		// Just ingore (treat as NOP) the invalid instructions
		if (i.addressing_mode != 0 && i.operation != 0)
		{
			cpu->internal->current_cycles = i.cycle_cost;
			// Perform addressing mode
			uint8_t c1 = i.addressing_mode(cpu);
			// Perform instruction
			uint16_t c2 = i.operation(cpu);

			// If both addressing mode and operation returns 1,
			// we need to add one extra cycle to the total cost of
			// the current instruction
			cpu->internal->current_cycles += (c1 & c2);
			cpu->internal->total_cycles += cpu->internal->current_cycles;
		}		
	}
		
	cpu->internal->current_cycles--;
}

uint8_t cpu6502_step_instruction(cpu6502_t* cpu)
{
	uint8_t clock_iterations = 1;
	cpu6502_clock(cpu);

	while (cpu->internal->current_cycles > 0)
	{
		clock_iterations++;
		cpu6502_clock(cpu);
	}

	return clock_iterations;
}

void cpu6502_reset_interrupt(cpu6502_t* cpu)
{
	cpu->internal->current_cycles = 6;
	
	// Some emulators sets SP to 0xFF
	// but looking at https://www.youtube.com/watch?v=fWqBmmPQP40&feature=youtu.be&t=2565
	// we can see that the reset interrupt pushed both pc and status register to
	// the stack (3 bytes) which places the stack pointer at 0xFD (since it starts at 0x00 and wraps around)
	cpu->stack_pointer = 0xFD;

	// Read high and low bytes for the start address from the reset vector
	// and load it in to the program counter.
	uint8_t lo = cpu6502_read_data(cpu, CPU_6502_RESET_VECTOR_LOW);
	uint8_t hi = cpu6502_read_data(cpu, CPU_6502_RESET_VECTOR_HIGH);
	cpu->program_counter = bytes_to_word(hi, lo);

	// Setup the status register
	cpu->status_register = CPU_STATUS_UNUSED;
}