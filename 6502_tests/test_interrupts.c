#include "test_utils.h"

void test_reset_interrupt(void)
{
	// Setup runs reset interrupt internally
	setup_cpu(0xAABB);

	test_check(0xFD, cpu->stack_pointer);
	test_check(0xAABB, cpu->program_counter);
	test_check(CPU_STATUS_UNUSED, cpu->status_register);
}