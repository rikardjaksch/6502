#include "test_utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct ram_io_device
{
	uint8_t* data;
} ram_io_device_t;

void ram_write(void* device, uint16_t address, uint8_t data)
{
	ram_io_device_t* ram = (ram_io_device_t*)device;
	ram->data[address] = data;
}

bool ram_read(void* device, uint16_t address, uint8_t* data)
{
	ram_io_device_t* ram = (ram_io_device_t*)device;
	*data = ram->data[address];

	return true;
}

ram_io_device_t* ram_device = NULL;
cpu6502_t* cpu = NULL;
cpu6502_io_t* cpu_io = NULL;
static bool initial_setup = true;

void setup_cpu(uint16_t start_address)
{
	if (initial_setup == true)
	{
		cpu_io = cpu6502_io_create();
		cpu = cpu6502_create(cpu_io);

		ram_device = (ram_io_device_t*)malloc(sizeof(ram_io_device_t));
		if (ram_device)
		{
			ram_device->data = malloc(sizeof(uint8_t) * 0x10000);
			cpu6502_io_attach_device(cpu_io, ram_device, ram_write, ram_read);
		}

		initial_setup = false;
	}

	// Clear ram memory
	if (ram_device && ram_device->data)
		memset(ram_device->data, 0, sizeof(uint8_t) * 0x10000);

	// Write 'start_address' to reset vector
	cpu6502_io_write(cpu_io, 0xFFFC, start_address & 0xFF);
	cpu6502_io_write(cpu_io, 0xFFFD, start_address >> 8);

	// Invoke reset interrupt
	cpu6502_reset_interrupt(cpu);
	cpu6502_step_instruction(cpu);
}

void write_code(uint16_t offset, uint8_t* code, uint16_t length)
{
	assert(offset + length < 0x10000);

	for (uint16_t address = 0; address < length; ++address)
	{
		cpu6502_io_write(cpu_io, address + offset, code[address]);
	}
}