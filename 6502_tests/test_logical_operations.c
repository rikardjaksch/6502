#include "test_utils.h"

void test_and_imm(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x29, 0x05,
		0x29, 0x00,
		0x29, 0x85,
	};
	write_code(0x0000, code, array_length(code));
	
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0002, cpu->program_counter);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);

	cpu->accumulator = 0x03;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(2, cycles);

	cpu->accumulator = 0x90;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x80, cpu->accumulator);
	test_check(2, cycles);
}

void test_and_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x25, 0x00,
		0x25, 0x01,
		0x25, 0x02,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0000, 0x05);
	cpu6502_io_write(cpu_io, 0x0001, 0x00);
	cpu6502_io_write(cpu_io, 0x0002, 0x85);

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(3, cycles);

	cpu->accumulator = 0x03;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(3, cycles);

	cpu->accumulator = 0x90;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x80, cpu->accumulator);
	test_check(3, cycles);
}

void test_and_zpx(void)
{
}

void test_and_abs(void)
{
}

void test_and_abx(void)
{

}

void test_and_aby(void)
{
}

void test_and_inx(void)
{
}

void test_and_iny(void)
{
}

void test_eor_imm(void)
{
}

void test_eor_zpg(void)
{
}

void test_eor_zpx(void)
{
}

void test_eor_abs(void)
{
}

void test_eor_abx(void)
{
}

void test_eor_aby(void)
{
}

void test_eor_inx(void)
{
}

void test_eor_iny(void)
{
}

void test_ora_imm(void)
{
}

void test_ora_zpg(void)
{
}

void test_ora_zpx(void)
{
}

void test_ora_abs(void)
{
}

void test_ora_abx(void)
{
}

void test_ora_aby(void)
{
}

void test_ora_inx(void)
{
}

void test_ora_iny(void)
{
}

void test_bit_zpg(void)
{
}

void test_bit_abs(void)
{
}
