#include "gdt.h"
#include "gdt_internal.h"

struct gdt_segment_desc *gdt;
struct gdt_ptr gdt_ptr;

/// @brief fill the segment descriptor passed as 1st parameter
/// @param gdt_segment segment descriptor to set
/// @param base beginning of the segment in memory
/// @param limit end of segment in memory
/// @param access segment type and permissions
/// @param granularity
static void create_segment_desc(struct gdt_segment_desc *gdt_segment,
                                uint32_t base, uint32_t limit, uint8_t access,
                                uint8_t granularity) {
  gdt_segment->base_low = (base & 0xFFFF);
  gdt_segment->base_middle = (base >> 16) & 0xFF;
  gdt_segment->base_high = (base >> 24) & 0xFF;

  gdt_segment->limit_low = (limit & 0xFFFF);
  gdt_segment->limit_high = (limit >> 16) & 0x0F;

  gdt_segment->granularity |= (granularity & 0xF);

  gdt_segment->access = access;
}

/// @brief fill gdt with all segments descriptors
static void fill_gdt_entry() {
  create_segment_desc(&gdt[0], 0, 0, 0, 0); // null descriptor
  // kernel segments
  create_segment_desc(&gdt[1], SEG_BASE, SEG_LIMIT, KERNEL_CODE_ACC,
                      GRANULARITY); // code
  create_segment_desc(&gdt[2], SEG_BASE, SEG_LIMIT, KERNEL_DATA_ACC,
                      GRANULARITY); // data
  create_segment_desc(&gdt[3], SEG_BASE, SEG_LIMIT, KERNEL_STACK_ACC,
                      GRANULARITY); // stack
  // user segments
  create_segment_desc(&gdt[4], SEG_BASE, SEG_LIMIT, USER_CODE_ACC,
                      GRANULARITY); // code
  create_segment_desc(&gdt[5], SEG_BASE, SEG_LIMIT, USER_DATA_ACC,
                      GRANULARITY); // data
  create_segment_desc(&gdt[6], SEG_BASE, SEG_LIMIT, USER_STACK_ACC,
                      GRANULARITY); // stack
}

void gdt_init() {
  gdt = (uint32_t *)GDT_ADDRESS;
  gdt_ptr.limit = (sizeof(struct gdt_segment_desc) * GDT_NB_ENTRIES) - 1;
  gdt_ptr.base = (uint32_t)gdt;
  fill_gdt_entry();
  _gdt_flush();
}
