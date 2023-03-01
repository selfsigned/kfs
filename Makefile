# requires GNU make >4.2.0
# call `use_docker` before rules to run inside docker, set INTERACTIVE to something e.g INTERACTIVE=1 to make docker use an interactive shell

## Vars ##
# files
NAME			:= kfs.bin
IMG_NAME	:= $(NAME:.bin=.iso)
SYM_NAME	:= $(NAME:.bin=.sym)

# iso
IMG_BUILD_DIR := .iso

# toolchain, hard-coded but we provide containers!
ARCH		:=	i686
PREFIX	:= $(ARCH)-elf
CC			:= $(PREFIX)-gcc
AS			:= $(PREFIX)-as
HAS_CC	:= $(shell $(CC) --version 2>/dev/null)

# macros
SIGNATURE_ADDRESS=0x0030DEAD
SIGNATURE_VALUE=0x00DEAD42 # !! NEEDS TO BE A WORD FOR THE TEST TO WORK !!

CPPFLAGS += \
 -MMD \
 -D SIGNATURE_ADDRESS=$(SIGNATURE_ADDRESS) \
 -D SIGNATURE_VALUE=$(SIGNATURE_VALUE)

CFLAGS ?= \
 -Wextra -Wall \
 -g
CFLAGS += \
 -std=c11 \
 -fno-builtin \
 -fno-exceptions \
 -fno-stack-protector \
 -nostdlib \
 -nodefaultlibs \
 -ffreestanding

# grub
TIMEOUT_GRUB  := 5
ifneq (,$(wildcard /usr/lib/grub/i386-pc))
HAS_GRUB := $(shell grub-file -u 2>/dev/null && xorriso -version 2>/dev/null)
endif

# docker
ifdef INTERACTIVE
INTERACTIVE := -it
endif
DOCKER_BIN		?= docker
DOCKER_CC			:= ghcr.io/l-aurelie/i686-cc:latest
DOCKER_CMD		= $(DOCKER_BIN) run $(INTERACTIVE) --platform linux/amd64 --rm -v $(ROOT_DIR):/build $(DOCKER_CC) make $(MAKECMDGOALS)
HAS_DOCKER		:= $(shell $(DOCKER_BIN) ps 2>/dev/null)
HAS_DOCKER_CC = $(shell $(DOCKER_BIN) image inspect $(DOCKER_CC) 2>/dev/null) 
ifneq (,$(wildcard /.dockerenv))
INSIDE_DOCKER := true
endif

# gbb
GDB_PORT	:= 4666

# sources
SRC_PATH		:= src
SRC_ASM			:= $(shell find $(SRC_PATH) -name "*.s")
SRC					:= $(shell find $(SRC_PATH) -name "*.c")
DEP					:= $(SRC:.c=.d)
OBJ					:= $(SRC_ASM:.s=.o) $(SRC:.c=.o)
LINKER_FILE	:= $(SRC_PATH)/linker.ld

# helpers
ROOT_DIR			:= $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

# Error
ERROR_CC		= @$(error "[ERROR] $(CC) not found, either run `make use_docker $(MAKECMDGOALS)` or run: $(DOCKER_CMD)")
ERROR_GRUB	= @$(error "[ERROR] grub, xorriso or grub*-bios missing, either run `make use_docker $(MAKECMDGOALS)` or run: $(DOCKER_CMD)")

## Rulez ##
.PHONY: all use_docker run gdb test clean fclean re

all: $(NAME) $(IMG_NAME)

$(NAME): $(OBJ)
	$(CC) -T $(LINKER_FILE) -o $(NAME) $(OBJ) -lgcc $(CFLAGS) $(CPPFLAGS)

$(IMG_NAME): $(NAME)
ifdef HAS_GRUB
	grub-file --is-x86-multiboot $(NAME)
	mkdir -p $(IMG_BUILD_DIR)/boot/grub
	cp $(NAME) $(IMG_BUILD_DIR)/boot
	printf "set timeout=$(TIMEOUT_GRUB)\nmenuentry \"$(NAME:.bin=)\" { multiboot /boot/$(NAME) }" > $(IMG_BUILD_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(IMG_NAME) $(IMG_BUILD_DIR)
else
	$(ERROR_GRUB)
endif

 %.o: %.c $(HEADERS)
ifdef HAS_CC
	$(CC) $(INCLUDE) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)
else
	$(ERROR_CC)
endif

 %.o: %.s
ifdef HAS_CC
	$(AS) $< -o $@
else
	$(ERROR_CC)
endif

-include $(DEP)

use_docker:
ifdef INSIDE_DOCKER
	@echo "[INFO] Running make inside docker"
	$(MAKE) $(patsubst $@,,$(MAKECMDGOALS))
else ifdef HAS_DOCKER_CC
	@echo "[INFO] Re-running in $(DOCKER_CC)"
	$(DOCKER_CMD)
else ifdef HAS_DOCKER
	@echo "[INFO] $(CC) not found, pulling $(DOCKER_CC)"
	$(DOCKER_BIN) pull --platform linux/amd64 $(DOCKER_CC)
	$(DOCKER_CMD)
else
	@$(error "[ERROR] Couldn't use docker")
endif

## virtual rules ##

run: $(IMG_NAME)
	@echo "[INFO] Qemu is running a gdb server at $(GDB_PORT)"
	qemu-system-i386 -boot d -cdrom $(IMG_NAME) \
		-m 4M \
		-display curses \
		-gdb tcp:localhost:$(GDB_PORT) # ncurses interface and gdb server

test: $(IMG_NAME)
	timeout 15 qemu-system-i386 \
    -boot d -cdrom $(IMG_NAME) \
		-m 4M -nographic -gdb tcp:localhost:$(GDB_PORT) &
	sleep 10 && (echo "x/wx $(SIGNATURE_ADDRESS)") | make gdb 2>/dev/null| grep -i $(SIGNATURE_VALUE)

gdb: $(NAME)
	gdb -ex "target remote localhost:$(GDB_PORT)" -ex "symbol-file $(NAME)"

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)

fclean: clean
	$(RM) $(NAME) $(IMG_NAME)
	$(RM) -r $(IMG_BUILD_DIR)

re:
	$(MAKE) fclean
	$(MAKE) all
