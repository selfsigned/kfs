#include "gdt.h"
#define GDTBASE 0x00000800

#define GDT_NB_ENTRIES 5

struct segment_desc gdt[GDT_NB_ENTRIES];
struct gdt_ptr gdt_ptr;

/// @brief fill the segment descriptor passed as 1st parameter
/// @param segment_desc segment descriptor to set
void create_segment_desc(struct segment_desc *segment_desc, uint32_t base,
                         uint32_t limit, uint8_t access, uint8_t flags) {
  segment_desc->base_low = (base & 0xFFFF);
  segment_desc->base_middle = (base >> 16) & 0xFF;
  segment_desc->base_high = (base >> 24) & 0xFF;

  segment_desc->limit_low = (limit & 0xFFFF);
  segment_desc->limit_high = (limit >> 16) & 0x0F;

  segment_desc->flags |= (flags & 0xF);

  segment_desc->access = access;
}

/// @brief fill gdt with all segments descriptors
void fill_gdt_entry() {
  create_segment_desc(&gdt[0], 0, 0, 0, 0);
  create_segment_desc(&gdt[1], 0, 0, 0, 0);
  create_segment_desc(&gdt[2], 0, 0, 0, 0);
  create_segment_desc(&gdt[3], 0, 0, 0, 0);
  create_segment_desc(&gdt[4], 0, 0, 0, 0);
}

/// @brief init gdt_pointer and fill gdt with all segments descriptors
void init_gdt() {
  gdt_ptr.limit = (sizeof(struct segment_desc) * GDT_NB_ENTRIES) - 1;
  gdt_ptr.base = (uint32_t)&gdt;
  fill_gdt_entry();

  struct segment_desc *ggdt = &gdt;
}
