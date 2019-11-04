#include "test_utils.h"

void test_clc_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x18
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, true);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);
}

void test_cld_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xD8
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_DECIMAL_MODE, true);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);
}

void test_cli_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x58
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_IRQ_DISABLE, true);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);
}

void test_clv_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xB8
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, true);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);
}

void test_sec_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x38
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, false);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_CARRY, cpu->status_register);
	test_check(2, cycles);
}

void test_sed_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xF8
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_DECIMAL_MODE, false);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_DECIMAL_MODE, cpu->status_register);
	test_check(2, cycles);
}

void test_sei_imp(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x78
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_IRQ_DISABLE, false);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_IRQ_DISABLE, cpu->status_register);
	test_check(2, cycles);
}