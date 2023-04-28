#include "syscalls.h"

#define DEMO_SYSCALL(NBR)                                                      \
  void demo_syscall##NBR(struct syscall_parameters *params) {                  \
    *params->ret = NBR + 42;                                                   \
    INFO_MSG("Syscall " #NBR " called. params->ret:%u", *params->ret);         \
  }

DEMO_SYSCALL(0);
DEMO_SYSCALL(1);
DEMO_SYSCALL(2);

struct syscall_entry syscall_table[SYSCALL_NBR] = {
    {&demo_syscall0},
    {&demo_syscall1},
    {&demo_syscall2},
};

/// @brief called from our syscall stub

void syscall_handler(struct general_regs regs) {

  if (regs.eax >= SYSCALL_NBR) {
    WARN_MSG("Syscall %#x not handled", regs.eax);
    return;
  }
  struct syscall_parameters params = {&regs.eax, &regs.ebx, &regs.ecx,
                                      &regs.edx, &regs.esi, &regs.edi,
                                      &regs.ebp};
  syscall_table[regs.eax].func(&params);
  DEBUG_MSG("INT 0x80 (syscall) called with eax %u", *params.ret);
}

void syscalls_init() {
  INFO_MSG("Initializing Syscalls...");
  idt_add_gate(0x80, &syscall_stub, INT_GATE_USER_FLAGS);
}
