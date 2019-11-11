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
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x35, 0x80,
		0x35, 0x80,
		0x35, 0x80,
		0x35, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x007F, 0x05);
	cpu6502_io_write(cpu_io, 0x0081, 0x05);
	cpu6502_io_write(cpu_io, 0x0082, 0x00);
	cpu6502_io_write(cpu_io, 0x0083, 0x85);

	cpu->x_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cpu->accumulator = 0x03;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(4, cycles);

	cpu->x_register = 0x02;
	cpu->accumulator = 0x03;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x00, cpu->accumulator);
	test_check(4, cycles);

	cpu->x_register = 0x03;
	cpu->accumulator = 0x90;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2008, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x80, cpu->accumulator);
	test_check(4, cycles);
}

void test_and_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x2D, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2005, 0x05);

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_and_abx(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x3D, 0x05, 0x20,
		0x3D, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x05);
	cpu6502_io_write(cpu_io, 0x2100, 0x05);

	cpu->x_register = 0x02;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->x_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_and_aby(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x39, 0x05, 0x20,
		0x39, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x05);
	cpu6502_io_write(cpu_io, 0x2100, 0x05);

	cpu->y_register = 0x02;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->y_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_and_inx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x21, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0085, 0xBB);
	cpu6502_io_write(cpu_io, 0x0086, 0xAA);
	cpu6502_io_write(cpu_io, 0xAABB, 0x05);

	cpu->x_register = 0x05;
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(6, cycles);
}

void test_and_iny(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x31, 0x80,
		0x31, 0x90,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0080, 0x00);
	cpu6502_io_write(cpu_io, 0x0081, 0x10);
	cpu6502_io_write(cpu_io, 0x0090, 0xFF);
	cpu6502_io_write(cpu_io, 0x0091, 0x10);

	cpu6502_io_write(cpu_io, 0x1005, 0x05);
	cpu6502_io_write(cpu_io, 0x1100, 0x05);

	cpu->y_register = 0x05;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(5, without_page_overflow);

	cpu->y_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x01, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(6, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_eor_imm(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x49, 0x05,
		0x49, 0x00,
		0x49, 0x80,
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0002, cpu->program_counter);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);

	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(2, cycles);

	cpu->accumulator = 0x05;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x85, cpu->accumulator);
	test_check(2, cycles);
}

void test_eor_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x45, 0x00,
		0x45, 0x01,
		0x45, 0x02,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0000, 0x05);
	cpu6502_io_write(cpu_io, 0x0001, 0x00);
	cpu6502_io_write(cpu_io, 0x0002, 0x80);

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(3, cycles);

	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(3, cycles);

	cpu->accumulator = 0x05;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x85, cpu->accumulator);
	test_check(3, cycles);
}

void test_eor_zpx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x55, 0x80,
		0x55, 0x80,
		0x55, 0x80,
		0x55, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x007F, 0x05);
	cpu6502_io_write(cpu_io, 0x0081, 0x05);
	cpu6502_io_write(cpu_io, 0x0082, 0x00);
	cpu6502_io_write(cpu_io, 0x0083, 0x80);

	cpu->x_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cpu->accumulator = 0x03;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(4, cycles);

	cpu->x_register = 0x02;
	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x00, cpu->accumulator);
	test_check(4, cycles);

	cpu->x_register = 0x03;
	cpu->accumulator = 0x05;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2008, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x85, cpu->accumulator);
	test_check(4, cycles);
}

void test_eor_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x4D, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2005, 0x05);

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_eor_abx(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x5D, 0x05, 0x20,
		0x5D, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x05);
	cpu6502_io_write(cpu_io, 0x2100, 0x05);

	cpu->x_register = 0x02;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->x_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_eor_aby(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x59, 0x05, 0x20,
		0x59, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x05);
	cpu6502_io_write(cpu_io, 0x2100, 0x05);

	cpu->y_register = 0x02;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->y_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_eor_inx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x41, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0085, 0xBB);
	cpu6502_io_write(cpu_io, 0x0086, 0xAA);
	cpu6502_io_write(cpu_io, 0xAABB, 0x05);

	cpu->x_register = 0x05;
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(6, cycles);
}

void test_eor_iny(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x51, 0x80,
		0x51, 0x90,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0080, 0x00);
	cpu6502_io_write(cpu_io, 0x0081, 0x10);
	cpu6502_io_write(cpu_io, 0x0090, 0xFF);
	cpu6502_io_write(cpu_io, 0x0091, 0x10);

	cpu6502_io_write(cpu_io, 0x1005, 0x05);
	cpu6502_io_write(cpu_io, 0x1100, 0x05);

	cpu->y_register = 0x05;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(5, without_page_overflow);

	cpu->y_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x06, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(6, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_ora_imm(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x09, 0x05,
		0x09, 0x00,
		0x09, 0x80,
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0002, cpu->program_counter);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(2, cycles);

	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0004, cpu->program_counter);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(2, cycles);

	cpu->accumulator = 0x05;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x85, cpu->accumulator);
	test_check(2, cycles);
}

void test_ora_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x05, 0x00,
		0x05, 0x01,
		0x05, 0x02,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0000, 0x05);
	cpu6502_io_write(cpu_io, 0x0001, 0x00);
	cpu6502_io_write(cpu_io, 0x0002, 0x80);

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(3, cycles);

	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(3, cycles);

	cpu->accumulator = 0x05;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x85, cpu->accumulator);
	test_check(3, cycles);
}

void test_ora_zpx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x15, 0x80,
		0x15, 0x80,
		0x15, 0x80,
		0x15, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x007F, 0x05);
	cpu6502_io_write(cpu_io, 0x0081, 0x05);
	cpu6502_io_write(cpu_io, 0x0082, 0x00);
	cpu6502_io_write(cpu_io, 0x0083, 0x80);

	cpu->x_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cpu->accumulator = 0x03;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(4, cycles);

	cpu->x_register = 0x02;
	cpu->accumulator = 0x00;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x00, cpu->accumulator);
	test_check(4, cycles);

	cpu->x_register = 0x03;
	cpu->accumulator = 0x05;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2008, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x85, cpu->accumulator);
	test_check(4, cycles);
}

void test_ora_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x0D, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2005, 0x05);

	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_ora_abx(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x1D, 0x05, 0x20,
		0x1D, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x05);
	cpu6502_io_write(cpu_io, 0x2100, 0x05);

	cpu->x_register = 0x02;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->x_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_ora_aby(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x19, 0x05, 0x20,
		0x19, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x05);
	cpu6502_io_write(cpu_io, 0x2100, 0x05);

	cpu->y_register = 0x02;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->y_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_ora_inx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x01, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0085, 0xBB);
	cpu6502_io_write(cpu_io, 0x0086, 0xAA);
	cpu6502_io_write(cpu_io, 0xAABB, 0x05);

	cpu->x_register = 0x05;
	cpu->accumulator = 0x03;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(6, cycles);
}

void test_ora_iny(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x11, 0x80,
		0x11, 0x90,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0080, 0x00);
	cpu6502_io_write(cpu_io, 0x0081, 0x10);
	cpu6502_io_write(cpu_io, 0x0090, 0xFF);
	cpu6502_io_write(cpu_io, 0x0091, 0x10);

	cpu6502_io_write(cpu_io, 0x1005, 0x05);
	cpu6502_io_write(cpu_io, 0x1100, 0x05);

	cpu->y_register = 0x05;
	cpu->accumulator = 0x03;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(5, without_page_overflow);

	cpu->y_register = 0x01;
	cpu->accumulator = 0x03;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x07, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(6, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_bit_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x24, 0x01,
		0x24, 0x02,
		0x24, 0x03,
		0x24, 0x04,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0001, 0xFF);
	cpu6502_io_write(cpu_io, 0x0002, 0x00);
	cpu6502_io_write(cpu_io, 0x0003, 0xFF);
	cpu6502_io_write(cpu_io, 0x0004, 0x00);

	cpu->accumulator = 0xFF;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x2002, cpu->program_counter);
	test_check(CPU_STATUS_NEGATIVE, cpu->status_register & CPU_STATUS_NEGATIVE);
	test_check(3, cycles);

	cpu->accumulator = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2004, cpu->program_counter);
	test_check(0, cpu->status_register & CPU_STATUS_NEGATIVE);
	test_check(3, cycles);

	cpu->accumulator = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2006, cpu->program_counter);
	test_check(CPU_STATUS_OVERFLOW, cpu->status_register & CPU_STATUS_OVERFLOW);
	test_check(3, cycles);

	cpu->accumulator = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x2008, cpu->program_counter);
	test_check(0, cpu->status_register & CPU_STATUS_OVERFLOW);
	test_check(3, cycles);
}

void test_bit_abs(void)
{	
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x2C, 0x01, 0x20,
		0x2C, 0x02, 0x20,
		0x2C, 0x03, 0x20,
		0x2C, 0x04, 0x20,
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2001, 0xFF);
	cpu6502_io_write(cpu_io, 0x2002, 0x00);
	cpu6502_io_write(cpu_io, 0x2003, 0xFF);
	cpu6502_io_write(cpu_io, 0x2004, 0x00);

	cpu->accumulator = 0xFF;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x0003, cpu->program_counter);
	test_check(CPU_STATUS_NEGATIVE, cpu->status_register & CPU_STATUS_NEGATIVE);
	test_check(4, cycles);

	cpu->accumulator = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0006, cpu->program_counter);
	test_check(0, cpu->status_register & CPU_STATUS_NEGATIVE);
	test_check(4, cycles);

	cpu->accumulator = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x0009, cpu->program_counter);
	test_check(CPU_STATUS_OVERFLOW, cpu->status_register & CPU_STATUS_OVERFLOW);
	test_check(4, cycles);

	cpu->accumulator = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x000C, cpu->program_counter);
	test_check(0, cpu->status_register & CPU_STATUS_OVERFLOW);
	test_check(4, cycles);
}
