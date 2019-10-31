#include "test_utils.h"

void test_lda_imm(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xA9, 0x05,
		0xA9, 0x00,
		0xA9, 0x85,
	};
	write_code(0x0000, code, array_length(code));

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(2, cycles);
}

void test_lda_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xA5, 0x00,
		0xA5, 0x01,
		0xA5, 0x02,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0000, 0x05);
	cpu6502_io_write(cpu_io, 0x0001, 0x00);
	cpu6502_io_write(cpu_io, 0x0002, 0x85);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(3, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(3, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x2006, cpu->program_counter);
	test_check(3, cycles);
}

void test_lda_zpx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xB5, 0x80,
		0xB5, 0x80,
		0xB5, 0x80,
		0xB5, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x007F, 0x05);
	cpu6502_io_write(cpu_io, 0x0081, 0x05);	
	cpu6502_io_write(cpu_io, 0x0082, 0x00);
	cpu6502_io_write(cpu_io, 0x0083, 0x85);
	
	cpu->x_register = 1;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 2;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x2006, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 3;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x2008, cpu->program_counter);
	test_check(4, cycles);
}

void test_lda_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xAD, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2005, 0x12);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_lda_abx(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xBD, 0x05, 0x20,
		0xBD, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x12);
	cpu6502_io_write(cpu_io, 0x2100, 0x34);

	cpu->x_register = 2;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->x_register = 1;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x34, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_lda_aby(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xB9, 0x05, 0x20,
		0xB9, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x12);
	cpu6502_io_write(cpu_io, 0x2100, 0x34);

	cpu->y_register = 2;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->y_register = 1;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x34, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_lda_inx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xA1, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0085, 0xBB);
	cpu6502_io_write(cpu_io, 0x0086, 0xAA);
	cpu6502_io_write(cpu_io, 0xAABB, 0x66);

	cpu->x_register = 5;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x66, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(6, cycles);
}

void test_lda_iny(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xB1, 0x80,
		0xB1, 0x90,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0080, 0x00);
	cpu6502_io_write(cpu_io, 0x0081, 0x10);
	cpu6502_io_write(cpu_io, 0x0090, 0xFF);
	cpu6502_io_write(cpu_io, 0x0091, 0x10);

	cpu6502_io_write(cpu_io, 0x1005, 0x66);
	cpu6502_io_write(cpu_io, 0x1100, 0x77);

	cpu->y_register = 5;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x66, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(5, without_page_overflow);

	cpu->y_register = 1;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x77, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(6, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_ldx_imm(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xA2, 0x05,
		0xA2, 0x00,
		0xA2, 0x85,
	};
	write_code(0x0000, code, array_length(code));

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(2, cycles);
}

void test_ldx_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xA6, 0x00,
		0xA6, 0x01,
		0xA6, 0x02,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0000, 0x05);
	cpu6502_io_write(cpu_io, 0x0001, 0x00);
	cpu6502_io_write(cpu_io, 0x0002, 0x85);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(3, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(3, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x2006, cpu->program_counter);
	test_check(3, cycles);
}

void test_ldx_zpy(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xB6, 0x80,
		0xB6, 0x80,
		0xB6, 0x80,
		0xB6, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x007F, 0x05);
	cpu6502_io_write(cpu_io, 0x0081, 0x05);
	cpu6502_io_write(cpu_io, 0x0082, 0x00);
	cpu6502_io_write(cpu_io, 0x0083, 0x85);

	cpu->y_register = 1;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(4, cycles);

	cpu->y_register = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(4, cycles);

	cpu->y_register = 2;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x2006, cpu->program_counter);
	test_check(4, cycles);

	cpu->y_register = 3;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->x_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x2008, cpu->program_counter);
	test_check(4, cycles);
}

void test_ldx_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xAE, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2005, 0x12);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_ldx_aby(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xBE, 0x05, 0x20,
		0xBE, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x12);
	cpu6502_io_write(cpu_io, 0x2100, 0x34);

	cpu->y_register = 2;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->y_register = 1;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x34, cpu->x_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_ldy_imm(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xA0, 0x05,
		0xA0, 0x00,
		0xA0, 0x85,
	};
	write_code(0x0000, code, array_length(code));

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0002, cpu->program_counter);
	test_check(2, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x0004, cpu->program_counter);
	test_check(2, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(2, cycles);
}

void test_ldy_zpg(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xA4, 0x00,
		0xA4, 0x01,
		0xA4, 0x02,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0000, 0x05);
	cpu6502_io_write(cpu_io, 0x0001, 0x00);
	cpu6502_io_write(cpu_io, 0x0002, 0x85);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(3, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(3, cycles);

	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x2006, cpu->program_counter);
	test_check(3, cycles);
}

void test_ldy_zpx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0xB4, 0x80,
		0xB4, 0x80,
		0xB4, 0x80,
		0xB4, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x007F, 0x05);
	cpu6502_io_write(cpu_io, 0x0081, 0x05);
	cpu6502_io_write(cpu_io, 0x0082, 0x00);
	cpu6502_io_write(cpu_io, 0x0083, 0x85);

	cpu->x_register = 1;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x05, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 2;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x00, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_ZERO, cpu->status_register);
	test_check(0x2006, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 3;
	cycles = cpu6502_step_instruction(cpu);
	test_check(0x85, cpu->y_register);
	test_check(CPU_STATUS_UNUSED | CPU_STATUS_NEGATIVE, cpu->status_register);
	test_check(0x2008, cpu->program_counter);
	test_check(4, cycles);
}

void test_ldy_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xAC, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2005, 0x12);

	uint8_t cycles = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_ldy_abx(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0xBC, 0x05, 0x20,
		0xBC, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x2007, 0x12);
	cpu6502_io_write(cpu_io, 0x2100, 0x34);

	cpu->x_register = 2;
	uint8_t without_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x12, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, without_page_overflow);

	cpu->x_register = 1;
	uint8_t with_page_overflow = cpu6502_step_instruction(cpu);
	test_check(0x34, cpu->y_register);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, with_page_overflow);

	test_check(true, with_page_overflow != without_page_overflow);
}

void test_sta_zpg(void)
{
	setup_cpu(0x2000);
	uint8_t code[] =
	{
		0x85, 0x05,
	};
	write_code(0x2000, code, array_length(code));

	cpu->accumulator = 0x66;
	uint8_t cycles = cpu6502_step_instruction(cpu);	
	uint8_t byte = cpu6502_io_read(cpu_io, 0x0005);
	
	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(3, cycles);
}

void test_sta_zpx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x95, 0x80,
		0x95, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu->x_register = 1;
	cpu->accumulator = 0x66;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x0081);
	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cpu->accumulator = 0x77;
	cycles = cpu6502_step_instruction(cpu);
	byte = cpu6502_io_read(cpu_io, 0x007F);
	test_check(0x77, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(4, cycles);
}

void test_sta_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x8D, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x12;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x2005);
	test_check(0x12, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_sta_abx(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x9D, 0x05, 0x20,
		0x9D, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x12;
	cpu->x_register = 2;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x2007);
	test_check(0x12, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(5, cycles);

	cpu->accumulator = 0x34;
	cpu->x_register = 1;
	cycles = cpu6502_step_instruction(cpu);
	byte = cpu6502_io_read(cpu_io, 0x2100);
	test_check(0x34, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, cycles);
}

void test_sta_aby(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x99, 0x05, 0x20,
		0x99, 0xFF, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu->accumulator = 0x12;
	cpu->y_register = 2;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x2007);
	test_check(0x12, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(5, cycles);

	cpu->accumulator = 0x34;
	cpu->y_register = 1;
	cycles = cpu6502_step_instruction(cpu);
	byte = cpu6502_io_read(cpu_io, 0x2100);
	test_check(0x34, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0006, cpu->program_counter);
	test_check(5, cycles);
}

void test_sta_inx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x81, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0085, 0xBB);
	cpu6502_io_write(cpu_io, 0x0086, 0xAA);

	cpu->accumulator = 0x66;
	cpu->x_register = 5;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0xAABB);
	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(6, cycles);
}

void test_sta_iny(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x81, 0x80,
		0x81, 0x90,
	};
	write_code(0x2000, code, array_length(code));

	cpu6502_io_write(cpu_io, 0x0080, 0x00);
	cpu6502_io_write(cpu_io, 0x0081, 0x10);
	cpu6502_io_write(cpu_io, 0x0090, 0xFF);
	cpu6502_io_write(cpu_io, 0x0091, 0x10);

	cpu->accumulator = 0x66;
	cpu->y_register = 5;
	uint8_t cycle = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x1005);
	test_check(0x66, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(6, cycle);

	cpu->accumulator = 0x77;
	cpu->y_register = 1;	
	cycle = cpu6502_step_instruction(cpu);
	byte = cpu6502_io_read(cpu_io, 0x1100);
	test_check(0x77, cpu->accumulator);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(6, cycle);
}

void test_stx_zpg(void)
{
	setup_cpu(0x2000);
	uint8_t code[] =
	{
		0x86, 0x05,
	};
	write_code(0x2000, code, array_length(code));

	cpu->x_register = 0x66;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x0005);

	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(3, cycles);
}

void test_stx_zpy(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x96, 0x80,
		0x96, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu->y_register = 1;
	cpu->x_register = 0x66;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x0081);
	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(4, cycles);

	cpu->y_register = 0xFF;
	cpu->x_register = 0x77;
	cycles = cpu6502_step_instruction(cpu);
	byte = cpu6502_io_read(cpu_io, 0x007F);
	test_check(0x77, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(4, cycles);
}

void test_stx_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x8E, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu->x_register = 0x12;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x2005);
	test_check(0x12, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}

void test_sty_zpg(void)
{
	setup_cpu(0x2000);
	uint8_t code[] =
	{
		0x84, 0x05,
	};
	write_code(0x2000, code, array_length(code));

	cpu->y_register = 0x66;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x0005);

	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(3, cycles);
}

void test_sty_zpx(void)
{
	setup_cpu(0x2000);

	uint8_t code[] =
	{
		0x94, 0x80,
		0x94, 0x80,
	};
	write_code(0x2000, code, array_length(code));

	cpu->x_register = 1;
	cpu->y_register = 0x66;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x0081);
	test_check(0x66, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2002, cpu->program_counter);
	test_check(4, cycles);

	cpu->x_register = 0xFF;
	cpu->y_register = 0x77;
	cycles = cpu6502_step_instruction(cpu);
	byte = cpu6502_io_read(cpu_io, 0x007F);
	test_check(0x77, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x2004, cpu->program_counter);
	test_check(4, cycles);
}

void test_sty_abs(void)
{
	setup_cpu(0x0000);

	uint8_t code[] =
	{
		0x8C, 0x05, 0x20
	};
	write_code(0x0000, code, array_length(code));

	cpu->y_register = 0x12;
	uint8_t cycles = cpu6502_step_instruction(cpu);
	uint8_t byte = cpu6502_io_read(cpu_io, 0x2005);
	test_check(0x12, byte);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
	test_check(0x0003, cpu->program_counter);
	test_check(4, cycles);
}