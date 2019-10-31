#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void (*io_write_cb)(void*, uint16_t, uint8_t);
typedef bool (*io_read_cb)(void*, uint16_t, uint8_t*);

// Read/Write interface for the cpu (like a bus)
// that you can connect devices to.
typedef struct cpu6502_io cpu6502_io_t;

// Creates an io interface that can be attached to a 6502 micro processor.
cpu6502_io_t* cpu6502_io_create();

// Destroys a previosly created instance of an io interface.
void cpu6502_io_destroy(cpu6502_io_t* io);

// Attaches a "device" to the io interface that will be notified when
// the cpu tries to read or write data.
void cpu6502_io_attach_device(cpu6502_io_t* io, void* device, io_write_cb write_cb, io_read_cb read_cb);

// Writes the data to the specified address over the io interface.
void cpu6502_io_write(cpu6502_io_t* io, uint16_t address, uint8_t data);

// Reads data from the specified address over the io interface.
uint8_t cpu6502_io_read(cpu6502_io_t* io, uint16_t address);
