#pragma once

// Interuppt operations
void test_reset_interrupt(void);

// Load and store operations
void test_lda_imm(void);
void test_lda_zpg(void);
void test_lda_zpx(void);
void test_lda_abs(void);
void test_lda_abx(void);
void test_lda_aby(void);
void test_lda_inx(void);
void test_lda_iny(void);

void test_ldx_imm(void);
void test_ldx_zpg(void);
void test_ldx_zpy(void);
void test_ldx_abs(void);
void test_ldx_aby(void);

void test_ldy_imm(void);
void test_ldy_zpg(void);
void test_ldy_zpx(void);
void test_ldy_abs(void);
void test_ldy_abx(void);

void test_sta_zpg(void);
void test_sta_zpx(void);
void test_sta_abs(void);
void test_sta_abx(void);
void test_sta_aby(void);
void test_sta_inx(void);
void test_sta_iny(void);

void test_stx_zpg(void);
void test_stx_zpy(void);
void test_stx_abs(void);

void test_sty_zpg(void);
void test_sty_zpx(void);
void test_sty_abs(void);

// Register transfer operations
void test_tax_imp(void);
void test_tay_imp(void);
void test_txa_imp(void);
void test_tya_imp(void);

// Stack operations
void test_tsx_imp(void);
void test_txs_imp(void);
void test_pha_imp(void);
void test_php_imp(void);
void test_pla_imp(void);
void test_plp_imp(void);

// Logical operations
// Arithmetic operations
// Incremenet and decrement operations
// Shift operations

// Jump and call operations
void test_jmp_abs(void);
void test_jmp_ind(void);
void test_jsr_abs(void);
void test_rts_imp(void);

// Status flag change operations
void test_clc_imp(void);
void test_cld_imp(void);
void test_cli_imp(void);
void test_clv_imp(void);
void test_sec_imp(void);
void test_sed_imp(void);
void test_sei_imp(void);

// System function operations

#define test_interrupts							\
	{"reset_interrupt", test_reset_interrupt}	

#define test_load_store_operations	\
	{"lda_imm", test_lda_imm},		\
	{"lda_zpg", test_lda_zpg},		\
	{"lda_zpx", test_lda_zpx},		\
	{"lda_abs", test_lda_abs},		\
	{"lda_abx", test_lda_abx},		\
	{"lda_aby", test_lda_aby},		\
	{"lda_inx", test_lda_inx},		\
	{"lda_iny", test_lda_iny},		\
	{"ldx_imm", test_ldx_imm},		\
	{"ldx_zpg", test_ldx_zpg},		\
	{"ldx_zpy", test_ldx_zpy},		\
	{"ldx_abs", test_ldx_abs},		\
	{"ldx_aby", test_ldx_aby},		\
	{"ldy_imm", test_ldy_imm},		\
	{"ldy_zpg", test_ldy_zpg},		\
	{"ldy_zpx", test_ldy_zpx},		\
	{"ldy_abs", test_ldy_abs},		\
	{"ldy_abx", test_ldy_abx},		\
	{"sta_zpg", test_sta_zpg},		\
	{"sta_zpx", test_sta_zpx},		\
	{"sta_abs", test_sta_abs},		\
	{"sta_abx", test_sta_abx},		\
	{"sta_aby", test_sta_aby},		\
	{"sta_inx", test_sta_inx},		\
	{"sta_iny", test_sta_iny},		\
	{"stx_zpg", test_sta_iny},		\
	{"stx_zpy", test_sta_iny},		\
	{"stx_abs", test_sta_iny},		\
	{"sty_zpg", test_sta_iny},		\
	{"sty_zpx", test_sta_iny},		\
	{"sty_abs", test_sta_iny}

#define test_register_transfer_operations \
	{"tax_imp", test_tax_imp},		\
	{"tay_imp", test_tay_imp},		\
	{"txa_imp", test_txa_imp},		\
	{"tya_imp", test_tya_imp}

#define test_stack_operations \
	{"tsx_imp", test_tsx_imp},		\
	{"txs_imp", test_txs_imp},		\
	{"pha_imp", test_pha_imp},		\
	{"php_imp", test_php_imp},		\
	{"pla_imp", test_pla_imp},		\
	{"plp_imp", test_plp_imp}

#define test_jump_and_call_operations \
	{"jmp_abs", test_jmp_abs},		\
	{"jmp_ind", test_jmp_ind},		\
	{"jsr_abs", test_jsr_abs},		\
	{"rts_imp", test_rts_imp}

#define test_status_flag_operations \
	{"clc_imp", test_clc_imp},		\
	{"cld_imp", test_cld_imp},		\
	{"cli_imp", test_cli_imp},		\
	{"clv_imp", test_clv_imp},		\
	{"sec_imp", test_sec_imp},		\
	{"sed_imp", test_sed_imp},		\
	{"sei_imp", test_sei_imp}