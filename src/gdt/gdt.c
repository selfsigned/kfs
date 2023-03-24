#include "gdt.h"

#define GDT_NB_ENTRIES 7
struct segment_desc gdt[GDT_NB_ENTRIES];
struct gdt_ptr gdt_ptr;

/// @brief fill the segment descriptor passed as 1st parameter
/// @param segment_desc segment descriptor to set
/// @param base beginning of the segment in memory
/// @param limit end of segment in memory
/// @param access segment type and permissions
/// @param granularity
void create_segment_desc(struct segment_desc *segment_desc, uint32_t base,
                         uint32_t limit, uint8_t access, uint8_t granularity) {
  segment_desc->base_low = (base & 0xFFFF);
  segment_desc->base_middle = (base >> 16) & 0xFF;
  segment_desc->base_high = (base >> 24) & 0xFF;

  segment_desc->limit_low = (limit & 0xFFFF);
  segment_desc->limit_high = (limit >> 16) & 0x0F;

  segment_desc->granularity |= (granularity & 0xF);

  segment_desc->access = access;
}



/// @brief fill gdt with all segments descriptors
/// flat memory model 0 to 0xFFFFFFFF : see https://forum.osdev.org/viewtopic.php?f=1&t=31835  |  https://forum.osdev.org/viewtopic.php?f=1&t=10691  |  

/// ACCESS = |P |PL  |T |E |DC|RW|A |
///          |7 |6  5|4 |3 |2 |1 |0 |
/// P = segment present in memory, set byte to 1
/// PL = desc privilege level(ring 0-3 : 0 for kernel, 3 for user)
/// T = desc type 0 = system segment, 1 for code /data segment
/// E = executable : 0 for data seg, 1 for code seg
/// DC = direction/ conforming
///      if data : direction 0 grow up, 1 grow down
///      if code : conforming 0 for only executable by equal privilege lvl, 1 for equal or lower 
/// RW = readable/writable if code segment 1 readable 0 not readable
///                       if data segment 1 writable 0 not writable        
/// A = access, let to 0 will be set by cpu

/// GRANULARITY = |G |DB|0 |0 | empty  |
///               |7 |6 |5 |4 |3 2 1 0 |
/// G = granularity size for scalling limit value : 1 = 4 kB blocks
/// DB = set to 1 for 32-bit 
#define SEG_BASE 0
#define SEG_LIMIT 0x000FFFFF
#define GRANULARITY 0x0C     // 1 1 0 0
enum access_byte{ACCESS = 0, READ_WRITE, DIRECTION_CONFORMING, EXECUTABLE, TYPE, PRIVILEGE_LVL = 5, PRESENT = 7};
#define KERNEL_CODE_ACC 1 << PRESENT | 0 << PRIVILEGE_LVL | 1 << TYPE | 1 << EXECUTABLE | 1 << READ_WRITE
#define KERNEL_DATA_ACC 1 << PRESENT | 0 << PRIVILEGE_LVL | 1 << TYPE | 1 << READ_WRITE
#define KERNEL_STACK_ACC 1 << PRESENT | 0 << PRIVILEGE_LVL | 1 << TYPE | 1 << DIRECTION_CONFORMING | 1 << READ_WRITE

#define USER_CODE_ACC 1 << PRESENT | 3 << PRIVILEGE_LVL | 1 << TYPE | 1 << EXECUTABLE | 1 << READ_WRITE
#define USER_DATA_ACC 1 << PRESENT | 3 << PRIVILEGE_LVL | 1 << TYPE | 1 << READ_WRITE
#define USER_STACK_ACC 1 << PRESENT | 3 << PRIVILEGE_LVL | 1 << TYPE | 1 << DIRECTION_CONFORMING| 1 << READ_WRITE

void fill_gdt_entry() {
  create_segment_desc(&gdt[0], 0, 0, 0, 0); // null descriptor
  // kernel segments
  create_segment_desc(&gdt[1], SEG_BASE, SEG_LIMIT, KERNEL_CODE_ACC, GRANULARITY); // code
  create_segment_desc(&gdt[2], SEG_BASE, SEG_LIMIT, KERNEL_DATA_ACC, GRANULARITY); // data
  create_segment_desc(&gdt[3], SEG_BASE, SEG_LIMIT, KERNEL_STACK_ACC, GRANULARITY); // stack
  // user segments
  create_segment_desc(&gdt[4], SEG_BASE, SEG_LIMIT, USER_CODE_ACC, GRANULARITY); // code
  create_segment_desc(&gdt[5], SEG_BASE, SEG_LIMIT, USER_DATA_ACC, GRANULARITY); // data
  create_segment_desc(&gdt[6], SEG_BASE, SEG_LIMIT, USER_STACK_ACC, GRANULARITY); // stack
}



/// @brief init gdt_pointer and fill gdt with all segments descriptors
void init_gdt() {
  gdt_ptr.limit = (sizeof(struct segment_desc) * GDT_NB_ENTRIES) - 1;
  gdt_ptr.base = (uint32_t)&gdt;
  fill_gdt_entry();

  //struct segment_desc *ggdt = &gdt;
}
