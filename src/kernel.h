#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

// git commit/tag version
#ifndef VERSION
#define VERSION "UNKNOWN"
#endif

#define OS_NAME "KFS"

//  MEMORY

#ifndef SIGNATURE_ADDRESS
/// Where the signature is set, used to test if the kernel initialized correctly
#define SIGNATURE_ADDRESS 0x00001337
#endif
#ifndef SIGNATURE_VALUE
/// The value we want to set as our signature
#define SIGNATURE_VALUE 0x00DEAD42
#endif

/// can get big quickly depending on the history size
#define SCREEN_BUFFER_ADDR 0x001FFFFF

// Screens
#define KFS_LOGO                                                               \
  "                  d8888  .d8888b.                                      \n"  \
  "                 d8P888 d88P  Y88b '||        .'|.        '            \n"  \
  "                d8P 888        888  ||  ..  .||.    ....     /|        \n"  \
  "               d8P  888      .d88P  || .'    ||    ||. '    /||        \n"  \
  "              d88   888  .od888P\"   ||'|.    ||    . '|..    ||        \n" \
  "              8888888888d88P\"      .||. ||. .||.   |'..|'    ||        \n" \
  "                    888 888\"                                 ||        \n" \
  "                    888 888888888                           ,/-'       \n"

#endif
