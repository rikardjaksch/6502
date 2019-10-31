#include "cpu6502_io.h"
#include <stdlib.h>
#include <assert.h>

typedef struct io_connection
{
	void* device;
	io_write_cb write;
	io_read_cb read;
} io_connection_t;

#define MAX_IO_DEVICES 32
typedef struct cpu6502_io
{
	io_connection_t connections[MAX_IO_DEVICES];
	uint8_t num_connections;
} cpu6502_io_t;

cpu6502_io_t* cpu6502_io_create()
{
	cpu6502_io_t* io = (cpu6502_io_t*)malloc(sizeof(cpu6502_io_t));
	if (io == NULL)
		return io;

	io->num_connections = 0;
	return io;
}

void cpu6502_io_destroy(cpu6502_io_t* io)
{
	free(io);
}

void cpu6502_io_attach_device(cpu6502_io_t* io, void* device, io_write_cb write_cb, io_read_cb read_cb)
{
	assert(io->num_connections < MAX_IO_DEVICES);
	io_connection_t* c = &io->connections[io->num_connections++];
	c->device = device;
	c->write = write_cb;
	c->read = read_cb;
}

void cpu6502_io_write(cpu6502_io_t* io, uint16_t address, uint8_t data)
{
	if (io == NULL)
		return;

	for (uint8_t index = 0; index < io->num_connections; ++index)
	{
		io_connection_t* c = &io->connections[index];
		if (c->write != NULL)
		{
			c->write(c->device, address, data);
		}
	}
}

uint8_t cpu6502_io_read(cpu6502_io_t* io, uint16_t address)
{
	if (io == NULL)
		return 0;

	uint8_t data = 0;
	for (uint8_t index = 0; index < io->num_connections; ++index)
	{
		io_connection_t* c = &io->connections[index];
		if (c->read != NULL)
		{
			if (c->read(c->device, address, &data) == true)
				break;
		}
	}

	return data;
}