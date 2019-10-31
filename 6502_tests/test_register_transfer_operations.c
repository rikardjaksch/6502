#include "test_utils.h"

void test_tax_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xAA,
		0xAA,
		0xAA,
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x05;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(2, cycles);

	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cpu->accumulator = 0x85;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(2, cycles);
}

void test_tay_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xA8,
		0xA8,
		0xA8,
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x05;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(2, cycles);

	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cpu->accumulator = 0x85;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(2, cycles);
}

void test_txa_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x8A,
		0x8A,
		0x8A,
	};
	write_code(0x0000, code, array_length(code));

	cpu->x_register = 0x05;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(2, cycles);

	cpu->x_register = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cpu->x_register = 0x85;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(2, cycles);
}

void test_tya_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x98,
		0x98,
		0x98,
	};
	write_code(0x0000, code, array_length(code));

	cpu->y_register = 0x05;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(2, cycles);

	cpu->y_register = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cpu->y_register = 0x85;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(2, cycles);
}