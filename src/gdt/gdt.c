#include "gdt.h"
#define GDTBASE 0x00000800

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
void fill_gdt_entry() {
                               // P  DP  T   E  DC RW A
  int kernel_code_acc = 0x9A;  // 1  00  1   1  0  1  0
  int kernel_data_acc = 0x92;  // 1  00  1   0  0  1  0
  int kernel_stack_acc = 0x96; // 1  00  1   0  1  1  0
  
  int user_code_acc = 0xFA;    // 1  11  1   1  0  1  0
  int user_data_acc = 0xF2;    // 1  11  1   0  0  1  0
  int user_stack_acc = 0xF6;   // 1  11  1   0  1  1  0

  int granularity = 0x0C;      // 1 1 0 0 

  create_segment_desc(&gdt[0], 0, 0, 0, 0); // null descriptor
  // kernel segments
  create_segment_desc(&gdt[1], 0, 0xFFFFFFFF, kernel_code_acc, granularity); // code
  create_segment_desc(&gdt[2], 0, 0xFFFFFFFF, kernel_data_acc, granularity); // data
  create_segment_desc(&gdt[3], 0, 0xFFFFFFFF, kernel_stack_acc, granularity); // stack
  // user segments
  create_segment_desc(&gdt[4], 0, 0xFFFFFFFF, user_code_acc, granularity); // code
  create_segment_desc(&gdt[5], 0, 0xFFFFFFFF, user_data_acc, granularity); // data
  create_segment_desc(&gdt[6], 0, 0xFFFFFFFF, user_stack_acc, granularity); // stack
}

/// @brief init gdt_pointer and fill gdt with all segments descriptors
void init_gdt() {
  gdt_ptr.limit = (sizeof(struct segment_desc) * GDT_NB_ENTRIES) - 1;
  gdt_ptr.base = (uint32_t)&gdt;
  fill_gdt_entry();

  struct segment_desc *ggdt = &gdt;
}
