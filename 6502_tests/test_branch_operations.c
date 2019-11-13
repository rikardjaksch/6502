#include "test_utils.h"

void test_bcc_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x90, 0x06,
		0x90, 0x06,
		0x90, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, false);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0x90);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_bcs_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xB0, 0x06,
		0xB0, 0x06,
		0xB0, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, true);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0xB0);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_CARRY, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_beq_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xF0, 0x06,
		0xF0, 0x06,
		0xF0, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, true);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0xF0);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_bmi_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x30, 0x06,
		0x30, 0x06,
		0x30, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, true);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0x30);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_bne_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xD0, 0x06,
		0xD0, 0x06,
		0xD0, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, false);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0xD0);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_bpl_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x10, 0x06,
		0x10, 0x06,
		0x10, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, true);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0x10);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_bvc_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x50, 0x06,
		0x50, 0x06,
		0x50, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, false);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0x50);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}

void test_bvs_rel(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x70, 0x06,
		0x70, 0x06,
		0x70, (0x06 ^ 0xFF) + 1 // 2's compliment of -6
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, true);
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0008, cpu->program_counter);
	test_check(3, cycles);

	cpu->program_counter = 0x0002;
	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, false);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0000, cpu->program_counter);
	test_check(3, cycles);

	cpu6502_io_write(cpu_io, 0x00F0, 0x70);
	cpu6502_io_write(cpu_io, 0x00F1, 0x20);
	cpu->program_counter = 0x00F0;
	cpu6502_set_status_bit(cpu, CPU_STATUS_OVERFLOW, true);
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0112, cpu->program_counter);
	test_check(4, cycles);
}