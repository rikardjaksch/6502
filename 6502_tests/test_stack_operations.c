#include "test_utils.h"

void test_tsx_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xBA,
		0xBA,
		0xBA,
	};
	write_code(0x0000, code, array_length(code));

	cpu->stack_pointer = 0x05;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(2, cycles);

	cpu->stack_pointer = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cpu->stack_pointer = 0x85;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(2, cycles);
}

void test_txs_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x9A
	};
	write_code(0x0000, code, array_length(code));

	cpu->x_register = 0x05;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->stack_pointer);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(2, cycles);
}

void test_pha_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x48
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x05;
	uint8_t expected_sp = cpu->stack_pointer - 1;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x1000 + cpu->stack_pointer + 1);

	test_check(expected_sp, cpu->stack_pointer);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0001, cpu->program_counter);
	test_check(0x05, byte);
	test_check(3, cycles);
}

void test_php_imp(void)
{

}

void test_pla_imp(void)
{

}

void test_plp_imp(void)
{

}