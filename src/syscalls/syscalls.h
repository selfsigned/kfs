#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "../hardware/interrupts/idt.h"
#include "../hardware/interrupts/interrupts.h"
#include "../log.h"

// helpful reference for linux syscalls:
// https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md

/// number of handled syscalls
#define SYSCALL_NBR 3

/// @brief values passed to interrupt handler when pusha
struct general_regs {
  // popal: EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
}; // not packed, hopefully fine?

/// @brief parameters passed to syscalls
struct syscall_parameters {
  uint32_t *ret;  /// eax
  uint32_t *arg0; /// ebx
  uint32_t *arg1; /// ecx
  uint32_t *arg2; /// edx
  uint32_t *arg3; /// esi
  uint32_t *arg4; /// edi
  uint32_t *arg5; /// ebp
};

struct syscall_entry {
  void (*func)(struct syscall_parameters *);
};

/// @brief syscall stub used to push user registeries in our syscall handler
extern void syscall_stub();

/// @brief Initialize syscall handlers !! Initialize interrutps first !!
void syscalls_init();

#endif
