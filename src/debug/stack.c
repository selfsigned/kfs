#include "stack.h"

size_t stack_walk(struct stack_frame *ebp, size_t max,
                  void (*callback)(struct stack_frame *, size_t)) {
  struct stack_frame *frame = ebp;
  size_t count;

  for (count = 0; frame && frame->ebp && count < max; ++count) {
    callback(frame, count);
    frame = frame->ebp;
  }
  return count;
}
