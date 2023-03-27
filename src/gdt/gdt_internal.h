#ifndef GDT_INTERNAL_H
#define GDT_INTERNAL_H
// flat memory model 0 to 0x000FFFFF : see

#define SEG_BASE 0
#define SEG_LIMIT 0x000FFFFF

/// GRANULARITY = |G |DB|0 |0 | empty  |
///               |7 |6 |5 |4 |3 2 1 0 |
/// G = granularity size for scalling limit value : 1 = 4 kB blocks
/// DB = set to 1 for 32-bit
#define GRANULARITY 0x0C // 1 1 0 0

/// ACCESS = |P |PL  |T |E |DC|RW|A |
///          |7 |6  5|4 |3 |2 |1 |0 |
/// P = segment present in memory, set byte to 1
/// PL = desc privilege level(ring 0-3 : 0 for kernel, 3 for user)
/// T = desc type 0 = system segment, 1 for code /data segment
/// E = executable : 0 for data seg, 1 for code seg
/// DC = direction/ conforming
///      if data : direction 0 grow up, 1 grow down
///      if code : conforming 0 for only executable by equal privilege lvl, 1
///      for equal or lower
/// RW = readable/writable if code segment 1 readable 0 not readable
///                       if data segment 1 writable 0 not writable
/// A = access, let to 0 will be set by cpu

enum access_byte {
  ACCESS,
  READ_WRITE,
  DIRECTION_CONFORMING,
  EXECUTABLE,
  TYPE,
  PRIVILEGE_LVL = 5,
  PRESENT = 7
};
#define KERNEL_CODE_ACC                                                        \
  1 << PRESENT | 0 << PRIVILEGE_LVL | 1 << TYPE | 1 << EXECUTABLE |            \
      1 << READ_WRITE
#define KERNEL_DATA_ACC                                                        \
  1 << PRESENT | 0 << PRIVILEGE_LVL | 1 << TYPE | 1 << READ_WRITE
#define KERNEL_STACK_ACC                                                       \
  1 << PRESENT | 0 << PRIVILEGE_LVL | 1 << TYPE | 1 << DIRECTION_CONFORMING |  \
      1 << READ_WRITE

#define USER_CODE_ACC                                                          \
  1 << PRESENT | 3 << PRIVILEGE_LVL | 1 << TYPE | 1 << EXECUTABLE |            \
      1 << READ_WRITE
#define USER_DATA_ACC                                                          \
  1 << PRESENT | 3 << PRIVILEGE_LVL | 1 << TYPE | 1 << READ_WRITE
#define USER_STACK_ACC                                                         \
  1 << PRESENT | 3 << PRIVILEGE_LVL | 1 << TYPE | 1 << DIRECTION_CONFORMING |  \
      1 << READ_WRITE

/// @brief Flush GDT to kernel, ASM
extern void _gdt_flush();

#endif
