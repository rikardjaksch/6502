#pragma once

#define TEST_NO_MAIN
#include "acutest.h"
#include "cpu6502.h"
#include "cpu6502_io.h"
#include <stdint.h>

#define test_check(expected, actual) \
	TEST_CHECK((expected) == (actual)); \
	TEST_MSG("Expected value to be 0x%04X but was: 0x%04X", expected, actual);


#define array_length(a) (sizeof(a)/sizeof(a[0]))

typedef struct ram_io_device ram_io_device_t;
typedef struct cpu6502 cpu6502_t;
typedef struct cpu6502_io cpu6502_io_t;

extern ram_io_device_t* ram_device;
extern cpu6502_t* cpu;
extern cpu6502_io_t* cpu_io;

// Sets up the cpu for each test, call at the start of each test case.
void setup_cpu(uint16_t start_address);

// Writes a piece of code over the io interface (for testing, writes in in ram)
void write_code(uint16_t offset, uint8_t* code, uint16_t length);