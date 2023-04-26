#ifndef STACK_H
#define STACK_H
/// Stack management utilities

#include <stddef.h>
#include <stdint.h>

/// @brief A stack frame
struct stack_frame {
  struct stack_frame *ebp; /// base pointer
  void *ret;               /// stack return address
} __attribute__((packed));

/// @brief Take a walk around the stack, calling *callback along the way
/// @param ebp where to start walking
/// @param max max number of frames to walk through (avoids infinitely looping)
/// @param callback callback function, can be used to print or dump the stack.
/// The first parameter is the frame and the second is the frame number
/// @return the number of frames stepped into
size_t stack_walk(struct stack_frame *ebp, size_t max,
                  void (*callback)(struct stack_frame *, size_t));

#endif
