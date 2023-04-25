
#ifndef CMD_HEXDUMP_H
#define CMD_HEXDUMP_H
// hexdump printing commands (required by KFS-2)

#include "../shell.h"

#define CMD_HEXDUMP_DEFAULT_SIZE 48

#define CMD_HEXDUMP_USAGE "Usage: hexdump <0xaddr> [size]"
#define CMD_HEXDUMP_SMSG "Displays memory chunks"
#define CMD_HEXDUMP_LMSG                                                       \
  CMD_HEXDUMP_USAGE "\naddr needs to be specified in hexadecimal!"

/// @brief first argument -> addr to hexdump, second argument -> nbr of bytes
int cmd_hexdump(uint8_t screen_nbr, int ac, char **av);

/// @brief equivalent of hexdump -C, doesn't care about endian-ness
/// @param screen_nbr screen to print to
/// @param data what to dump
/// @param size how many bytes to dump
void hexdump(uint8_t screen_nbr, void *data, size_t size);

#endif
