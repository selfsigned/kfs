#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

#ifndef SIGNATURE_ADDRESS
/// Signature of our kernel, at around 3MB
#define SIGNATURE_ADDRESS 0x0030DEAD
#endif
#ifndef SIGNATURE_VALUE
/// The value we want to set as our signature
#define SIGNATURE_VALUE 0x00DEAD42
#endif

/// can get big quickly depending on the history size
#define SCREEN_BUFFER_ADDR 0x001FFFFF

#endif
