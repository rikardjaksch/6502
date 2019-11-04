#include "test_utils.h"

void test_jmp_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x4C, 0x34, 0x12
	};
	write_code(0x0000, code, array_length(code));

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x1234, cpu->program_counter);
	test_check(3, cycles);
}

void test_jmp_ind(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x6C, 0xFE, 0x00
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x00FE, 0x34);
	cpu6502_io_write(cpu_io, 0x00FF, 0x12);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x1234, cpu->program_counter);
	test_check(5, cycles);
}

void test_jsr_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x20, 0x34, 0x12
	};
	write_code(0x0000, code, array_length(code));

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x1234, cpu->program_counter);
	test_check(6, cycles);

	uint8_t hi = cpu6502_pop_stack(cpu);
	uint8_t lo = cpu6502_pop_stack(cpu);

	test_check(0x0003, bytes_to_word(hi, lo));
}

void test_rts_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x60
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_push_stack(cpu, 0x34);
	cpu6502_push_stack(cpu, 0x12);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x1234, cpu->program_counter);
	test_check(6, cycles);
}