#pragma once

#define cpu_set_flag(c, flag, cond) if (cpu) { if (cond) {cpu->status_register |= flag;} else {cpu->status_register &= ~flag;} }
#define cpu_get_flag(c, flag) ((cpu) ? ((cpu->status_register & f > 0) ? 1  : 0) : 0)
