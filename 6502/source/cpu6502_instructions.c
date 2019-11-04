#include "cpu6502_instructions.h"
#include "cpu6502.h"

// All addressing modes of the 6502 micro processor
uint8_t imp(cpu6502_t* cpu); uint8_t acc(cpu6502_t* cpu); uint8_t imm(cpu6502_t* cpu); 
uint8_t zpg(cpu6502_t* cpu); uint8_t zpx(cpu6502_t* cpu); uint8_t zpy(cpu6502_t* cpu); 
uint8_t rel(cpu6502_t* cpu); uint8_t abs(cpu6502_t* cpu); uint8_t abx(cpu6502_t* cpu); 
uint8_t aby(cpu6502_t* cpu); uint8_t ind(cpu6502_t* cpu); uint8_t izx(cpu6502_t* cpu);
uint8_t izy(cpu6502_t* cpu);

// All instructions (not all permutations) supported
// by the 6502 micro processor
uint8_t adc(cpu6502_t* cpu); uint8_t and(cpu6502_t* cpu); uint8_t asl(cpu6502_t* cpu); uint8_t bcc(cpu6502_t* cpu);
uint8_t bcs(cpu6502_t* cpu); uint8_t beq(cpu6502_t* cpu); uint8_t bit(cpu6502_t* cpu); uint8_t bmi(cpu6502_t* cpu);
uint8_t bne(cpu6502_t* cpu); uint8_t bpl(cpu6502_t* cpu); uint8_t brk(cpu6502_t* cpu); uint8_t bvc(cpu6502_t* cpu);
uint8_t bvs(cpu6502_t* cpu); uint8_t clc(cpu6502_t* cpu); uint8_t cld(cpu6502_t* cpu); uint8_t cli(cpu6502_t* cpu);
uint8_t clv(cpu6502_t* cpu); uint8_t cmp(cpu6502_t* cpu); uint8_t cpx(cpu6502_t* cpu); uint8_t cpy(cpu6502_t* cpu);
uint8_t dec(cpu6502_t* cpu); uint8_t dex(cpu6502_t* cpu); uint8_t dey(cpu6502_t* cpu); uint8_t eor(cpu6502_t* cpu);
uint8_t inc(cpu6502_t* cpu); uint8_t inx(cpu6502_t* cpu); uint8_t iny(cpu6502_t* cpu); uint8_t jmp(cpu6502_t* cpu);
uint8_t jsr(cpu6502_t* cpu); uint8_t lda(cpu6502_t* cpu); uint8_t ldx(cpu6502_t* cpu); uint8_t ldy(cpu6502_t* cpu);
uint8_t lsr(cpu6502_t* cpu); uint8_t nop(cpu6502_t* cpu); uint8_t ora(cpu6502_t* cpu); uint8_t pha(cpu6502_t* cpu);
uint8_t php(cpu6502_t* cpu); uint8_t pla(cpu6502_t* cpu); uint8_t plp(cpu6502_t* cpu); uint8_t rol(cpu6502_t* cpu);
uint8_t ror(cpu6502_t* cpu); uint8_t rti(cpu6502_t* cpu); uint8_t rts(cpu6502_t* cpu); uint8_t sbc(cpu6502_t* cpu);
uint8_t sec(cpu6502_t* cpu); uint8_t sed(cpu6502_t* cpu); uint8_t sei(cpu6502_t* cpu); uint8_t sta(cpu6502_t* cpu);
uint8_t stx(cpu6502_t* cpu); uint8_t sty(cpu6502_t* cpu); uint8_t tax(cpu6502_t* cpu); uint8_t tay(cpu6502_t* cpu);
uint8_t tsx(cpu6502_t* cpu); uint8_t txa(cpu6502_t* cpu); uint8_t txs(cpu6502_t* cpu); uint8_t tya(cpu6502_t* cpu);

// Setup the lookup table with all permutations for all valid instructions
// Lookup table is based on https://www.masswerk.at/6502/6502_instruction_set.html
// and https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
// (The second link is the reason a lot of invalid op-codes are marked as NOP instructions)
#define I(op, addr, c) {c, #op, addr, op}
#define INV {0, "<invalid>", 0, 0}
instruction_t lookup[256] =
{
	I(brk, imp, 7), I(ora, izx, 6), INV, INV, INV, I(ora, zpg, 3), I(asl, zpg, 5), INV, I(php, imp, 3), I(ora, imm, 2), I(asl, acc, 2), INV, INV, I(ora, abs, 4), I(asl, abs, 6), INV,
	I(bpl, rel, 2 ), I(ora, izy, 5 ), INV, INV, I(nop, imp, 4 ), I(ora, zpx, 4 ), I(asl, zpx, 6 ), INV, I(clc, imp, 2 ), I(ora, aby, 4 ), I(nop, imp, 2 ), INV, I(nop, imp, 4 ), I(ora, abx, 4 ), I(asl, abx, 7 ), INV,
	I(jsr, abs, 6 ), I(and, izx, 6 ), INV, INV, I(bit, zpg, 3 ), I(and, zpg, 3 ), I(rol, zpg, 5 ), INV, I(plp, imp, 4 ), I(and, imm, 2 ), I(rol, imp, 2 ), INV, I(bit, abs, 4 ), I(and, abs, 4 ), I(rol, abs, 6 ), INV,
	I(bmi, rel, 2 ), I(and, izy, 5 ), INV, INV, I(nop, imp, 4 ), I(and, zpx, 4 ), I(rol, zpx, 6 ), INV, I(sec, imp, 2 ), I(and, aby, 4 ), I(nop, imp, 2 ), INV, I(nop, imp, 4 ), I(and, abx, 4 ), I(rol, abx, 7 ), INV,
	I(rti, imp, 6 ), I(eor, izx, 6 ), INV, INV, I(nop, imp, 3 ), I(eor, zpg, 3 ), I(lsr, zpg, 5 ), INV, I(pha, imp, 3 ), I(eor, imm, 2 ), I(lsr, imp, 2 ), INV, I(jmp, abs, 3 ), I(eor, abs, 4 ), I(lsr, abs, 6 ), INV,
	I(bvc, rel, 2 ), I(eor, izy, 5 ), INV, INV, I(nop, imp, 4 ), I(eor, zpx, 4 ), I(lsr, zpx, 6 ), INV, I(cli, imp, 2 ), I(eor, aby, 4 ), I(nop, imp, 2 ), INV, I(nop, imp, 4 ), I(eor, abx, 4 ), I(lsr, abx, 7 ), INV,
	I(rts, imp, 6 ), I(adc, izx, 6 ), INV, INV, I(nop, imp, 3 ), I(adc, zpg, 3 ), I(ror, zpg, 5 ), INV, I(pla, imp, 4 ), I(adc, imm, 2 ), I(ror, imp, 2 ), INV, I(jmp, ind, 5 ), I(adc, abs, 4 ), I(ror, abs, 6 ), INV,
	I(bvs, rel, 2 ), I(adc, izy, 5 ), INV, INV, I(nop, imp, 4 ), I(adc, zpx, 4 ), I(ror, zpx, 6 ), INV, I(sei, imp, 2 ), I(adc, aby, 4 ), I(nop, imp, 2 ), INV, I(nop, imp, 4 ), I(adc, abx, 4 ), I(ror, abx, 7 ), INV,
	I(nop, imp, 2 ), I(sta, izx, 6 ), I(nop, imp, 2 ), INV, I(sty, zpg, 3 ), I(sta, zpg, 3 ), I(stx, zpg, 3 ), INV, I(dey, imp, 2 ), I(nop, imp, 2 ), I(txa, imp, 2 ), INV, I(sty, abs, 4 ), I(sta, abs, 4 ), I(stx, abs, 4 ), INV,
	I(bcc, rel, 2 ), I(sta, izy, 6 ), INV, INV, I(sty, zpx, 4 ), I(sta, zpx, 4 ), I(stx, zpy, 4 ), INV, I(tya, imp, 2 ), I(sta, aby, 5 ), I(txs, imp, 2 ), INV, I(nop, imp, 5 ), I(sta, abx, 5 ), INV, INV,
	I(ldy, imm, 2 ), I(lda, izx, 6 ), I(ldx, imm, 2 ), INV, I(ldy, zpg, 3 ), I(lda, zpg, 3 ), I(ldx, zpg, 3 ), INV, I(tay, imp, 2 ), I(lda, imm, 2 ), I(tax, imp, 2 ), INV, I(ldy, abs, 4 ), I(lda, abs, 4 ), I(ldx, abs, 4 ), INV,
	I(bcs, rel, 2 ), I(lda, izy, 5 ), INV, INV, I(ldy, zpx, 4 ), I(lda, zpx, 4 ), I(ldx, zpy, 4 ), INV, I(clv, imp, 2 ), I(lda, aby, 4 ), I(tsx, imp, 2 ), INV, I(ldy, abx, 4 ), I(lda, abx, 4 ), I(ldx, aby, 4 ), INV,
	I(cpy, imm, 2 ), I(cmp, izx, 6 ), I(nop, imp, 2 ), INV, I(cpy, zpg, 3 ), I(cmp, zpg, 3 ), I(dec, zpg, 5 ), INV, I(iny, imp, 2 ), I(cmp, imm, 2 ), I(dex, imp, 2 ), INV, I(cpy, abs, 4 ), I(cmp, abs, 4 ), I(dec, abs, 6 ), INV,
	I(bne, rel, 2 ), I(cmp, izy, 5 ), INV, INV, I(nop, imp, 4 ), I(cmp, zpx, 4 ), I(dec, zpx, 6 ), INV, I(cld, imp, 2 ), I(cmp, aby, 4 ), I(nop, imp, 2 ), INV, I(nop, imp, 4 ), I(cmp, abx, 4 ), I(dec, abx, 7 ), INV,
	I(cpx, imm, 2 ), I(sbc, izx, 6 ), I(nop, imp, 2 ), INV, I(cpx, zpg, 3 ), I(sbc, zpg, 3 ), I(inc, zpg, 5 ), INV, I(inx, imp, 2 ), I(sbc, imm, 2 ), I(nop, imp, 2 ), I(sbc, imp, 2 ), I(cpx, abs, 4 ), I(sbc, abs, 4 ), I(inc, abs, 6 ), INV,
	I(beq, rel, 2 ), I(sbc, izy, 5 ), INV, INV, I(nop, imp, 4 ), I(sbc, zpx, 4 ), I(inc, zpx, 6 ), INV, I(sed, imp, 2 ), I(sbc, aby, 4 ), I(nop, imp, 2 ), INV, I(nop, imp, 4 ), I(sbc, abx, 4 ), I(inc, abx, 7 ), INV,
};
instruction_t* instruction_table = lookup;

// State affected by the addressing modes
uint16_t absolute_address = 0;
uint16_t relative_address = 0;
uint8_t M = 0;

uint8_t imp(cpu6502_t* cpu)
{
	return 0;
}

uint8_t acc(cpu6502_t* cpu)
{
	M = cpu->accumulator;
	return 0;
}

uint8_t imm(cpu6502_t* cpu)
{
	M = cpu6502_read_data(cpu, cpu->program_counter++);
	return 0;
}

uint8_t zpg(cpu6502_t* cpu)
{
	absolute_address = cpu6502_read_data(cpu, cpu->program_counter++);
	M = cpu6502_read_data(cpu, absolute_address);
	return 0;
}

uint8_t zpx(cpu6502_t* cpu)
{
	uint16_t zero_page_offset = cpu6502_read_data(cpu, cpu->program_counter++);
	absolute_address = (zero_page_offset + cpu->x_register) & 0x00FF;
	M = cpu6502_read_data(cpu, absolute_address);
	return 0;
}

uint8_t zpy(cpu6502_t* cpu)
{
	uint8_t zero_page_offset = cpu6502_read_data(cpu, cpu->program_counter++);
	absolute_address = (uint16_t)zero_page_offset + (uint16_t)cpu->y_register;
	M = cpu6502_read_data(cpu, absolute_address & 0x00FF);
	return 0;
}

uint8_t rel(cpu6502_t* cpu)
{
	return 0;
}

uint8_t abs(cpu6502_t* cpu)
{
	uint8_t lo = cpu6502_read_data(cpu, cpu->program_counter++);
	uint8_t hi = cpu6502_read_data(cpu, cpu->program_counter++);
	absolute_address = (uint16_t)hi << 8 | lo;
	M = cpu6502_read_data(cpu, absolute_address);

	return 0;
}

uint8_t abx(cpu6502_t* cpu)
{
	uint16_t lo = cpu6502_read_data(cpu, cpu->program_counter++);
	uint16_t hi = cpu6502_read_data(cpu, cpu->program_counter++);
	
	absolute_address = (hi << 8) | lo;
	absolute_address += cpu->x_register;

	M = cpu6502_read_data(cpu, absolute_address);

	if ((hi << 8) != (absolute_address & 0xFF00))
		return 1;

	return 0;
}

uint8_t aby(cpu6502_t* cpu)
{
	uint16_t lo = cpu6502_read_data(cpu, cpu->program_counter++);
	uint16_t hi = cpu6502_read_data(cpu, cpu->program_counter++);

	absolute_address = (hi << 8) | lo;
	absolute_address += cpu->y_register;

	M = cpu6502_read_data(cpu, absolute_address);

	if ((hi << 8) != (absolute_address & 0xFF00))
		return 1;

	return 0;
}

uint8_t ind(cpu6502_t* cpu)
{
	uint8_t low_byte_address = cpu6502_read_data(cpu, cpu->program_counter++);
	uint8_t high_byte_address = cpu6502_read_data(cpu, cpu->program_counter++);

	uint16_t indirect_address = bytes_to_word(high_byte_address, low_byte_address);

	uint8_t lo = cpu6502_read_data(cpu, indirect_address++);
	uint8_t hi = cpu6502_read_data(cpu, indirect_address);

	absolute_address = bytes_to_word(hi, lo);

	return 0;
}

uint8_t izx(cpu6502_t* cpu)
{
	uint16_t zero_page_offset = cpu6502_read_data(cpu, cpu->program_counter++);
	uint16_t low_byte_address = cpu6502_read_data(cpu, (zero_page_offset + cpu->x_register) & 0x00FF);
	uint16_t high_byte_address = cpu6502_read_data(cpu, (zero_page_offset + cpu->x_register + 1) & 0x00FF);
	absolute_address = (high_byte_address << 8) | low_byte_address;

	M = cpu6502_read_data(cpu, absolute_address);

	return 0;
}

uint8_t izy(cpu6502_t* cpu)
{
	uint16_t zero_page_offset = cpu6502_read_data(cpu, cpu->program_counter++);
	uint16_t low_byte_address = cpu6502_read_data(cpu, zero_page_offset & 0x00FF);
	uint16_t high_byte_address = cpu6502_read_data(cpu, (zero_page_offset + 1) & 0x00FF);

	absolute_address = (high_byte_address << 8) | low_byte_address;
	absolute_address += cpu->y_register;

	M = cpu6502_read_data(cpu, absolute_address);

	if ((absolute_address & 0xFF00) != (high_byte_address << 8))
		return 1;

	return 0;
}

uint8_t adc(cpu6502_t* cpu)
{
	return 0;
}

uint8_t and(cpu6502_t* cpu)
{
	return 0;
}

uint8_t asl(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bcc(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bcs(cpu6502_t* cpu)
{
	return 0;
}

uint8_t beq(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bit(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bmi(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bne(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bpl(cpu6502_t* cpu)
{
	return 0;
}

uint8_t brk(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bvc(cpu6502_t* cpu)
{
	return 0;
}

uint8_t bvs(cpu6502_t* cpu)
{
	return 0;
}

uint8_t clc(cpu6502_t* cpu)
{
	return 0;
}

uint8_t cld(cpu6502_t* cpu)
{
	return 0;
}

uint8_t cli(cpu6502_t* cpu)
{
	return 0;
}

uint8_t clv(cpu6502_t* cpu)
{
	return 0;
}

uint8_t cmp(cpu6502_t* cpu)
{
	return 0;
}

uint8_t cpx(cpu6502_t* cpu)
{
	return 0;
}

uint8_t cpy(cpu6502_t* cpu)
{
	return 0;
}

uint8_t dec(cpu6502_t* cpu)
{
	return 0;
}

uint8_t dex(cpu6502_t* cpu)
{
	return 0;
}

uint8_t dey(cpu6502_t* cpu)
{
	return 0;
}

uint8_t eor(cpu6502_t* cpu)
{
	return 0;
}

uint8_t inc(cpu6502_t* cpu)
{
	return 0;
}

uint8_t inx(cpu6502_t* cpu)
{
	return 0;
}

uint8_t iny(cpu6502_t* cpu)
{
	return 0;
}

uint8_t jmp(cpu6502_t* cpu)
{
	cpu->program_counter = absolute_address;
	return 0;
}

uint8_t jsr(cpu6502_t* cpu)
{
	cpu6502_push_stack(cpu, low_byte(cpu->program_counter));
	cpu6502_push_stack(cpu, high_byte(cpu->program_counter));
	cpu->program_counter = absolute_address;
	return 0;
}


uint8_t lda(cpu6502_t* cpu)
{
	// A,Z,N = M
	cpu->accumulator = M;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->accumulator == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->accumulator & 0x80);
	return 1;
}

uint8_t ldx(cpu6502_t* cpu)
{
	// X,Z,N = M
	cpu->x_register = M;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->x_register == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->x_register & 0x80);
	return 1;
}

uint8_t ldy(cpu6502_t* cpu)
{
	// Y,Z,N = M
	cpu->y_register = M;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->y_register == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->y_register & 0x80);
	return 1;
}

uint8_t lsr(cpu6502_t* cpu)
{
	return 0;
}

uint8_t nop(cpu6502_t* cpu)
{
	return 0;
}

uint8_t ora(cpu6502_t* cpu)
{
	return 0;
}

uint8_t pha(cpu6502_t* cpu)
{
	cpu6502_push_stack(cpu, cpu->accumulator);
	return 0;
}

uint8_t php(cpu6502_t* cpu)
{
	return 0;
}

uint8_t pla(cpu6502_t* cpu)
{
	return 0;
}

uint8_t plp(cpu6502_t* cpu)
{
	return 0;
}

uint8_t rol(cpu6502_t* cpu)
{
	return 0;
}

uint8_t ror(cpu6502_t* cpu)
{
	return 0;
}

uint8_t rti(cpu6502_t* cpu)
{
	return 0;
}

uint8_t rts(cpu6502_t* cpu)
{
	uint8_t hi = cpu6502_pop_stack(cpu);
	uint8_t lo = cpu6502_pop_stack(cpu);

	cpu->program_counter = bytes_to_word(hi, lo);
	return 0;
}

uint8_t sbc(cpu6502_t* cpu)
{
	return 0;
}

uint8_t sec(cpu6502_t* cpu)
{
	return 0;
}

uint8_t sed(cpu6502_t* cpu)
{
	return 0;
}

uint8_t sei(cpu6502_t* cpu)
{
	return 0;
}

uint8_t sta(cpu6502_t* cpu)
{
	cpu6502_write_data(cpu, absolute_address, cpu->accumulator);
	return 0;
}

uint8_t stx(cpu6502_t* cpu)
{
	return 0;
}

uint8_t sty(cpu6502_t* cpu)
{
	return 0;
}

uint8_t tax(cpu6502_t* cpu)
{
	// X,Z,N = A
	cpu->x_register = cpu->accumulator;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->x_register == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->x_register & 0x80);
	return 0;
}

uint8_t tay(cpu6502_t* cpu)
{
	// Y,Z,N = A
	cpu->y_register = cpu->accumulator;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->y_register == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->y_register & 0x80);
	return 0;
}

uint8_t tsx(cpu6502_t* cpu)
{
	// X,Z,N = S
	cpu->x_register = cpu->stack_pointer;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->x_register == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->x_register & 0x80);
	return 0;
}

uint8_t txa(cpu6502_t* cpu)
{
	// A,Z,N = X
	cpu->accumulator = cpu->x_register;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->accumulator == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->accumulator & 0x80);
	return 0;
}

uint8_t txs(cpu6502_t* cpu)
{
	// S = X
	cpu->stack_pointer = cpu->x_register;
	return 0;
}

uint8_t tya(cpu6502_t* cpu)
{
	// A,Z,N = Y
	cpu->accumulator = cpu->y_register;
	cpu6502_set_status_bit(cpu, CPU_STATUS_ZERO, cpu->accumulator == 0x00);
	cpu6502_set_status_bit(cpu, CPU_STATUS_NEGATIVE, cpu->accumulator & 0x80);
	return 0;
}
