## Vars ##
# files
NAME			:= kfs
IMG_NAME	:= $(NAME).iso

# toolchain, hard-coded but we provide containers!
ARCH		:=	i686
PREFIX	:= $(ARCH)-elf
CC			:= $(PREFIX)-gcc
AS			:= $(PREFIX)-as
HAS_CC	:= $(shell $(CC) --version 2>/dev/null)

# TODO check how to  combine this and env
CFLAGS := -fno-builtin \
 -fno-exceptions \
 -fno-stack-protector \
 -fno-rtti \
 -nostdlib \
 -nodefaultlibs

# docker
DOCKER_BIN		?= docker
HAS_DOCKER		:= $(shell $(DOCKER_BIN) ps 2>/dev/null)
DOCKER_CC			:= ghcr.io/l-aurelie/i686-cc:latest
HAS_DOCKER_CC = $(shell $(DOCKER_BIN) image inspect $(DOCKER_CC) 2>/dev/null) 

# helpers
ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SRC_ASM	= $(shell find src/ -name "*.s")
SRC			= $(shell find src/ -name "*.c")
OBJ			= $(SRC_ASM:.s=.o) $(SRC:.c=.o)

## Rulez ##
.PHONY: all toolchain_set

all:
	@$(MAKE) $(NAME)
	# $(IMG_NAME)

toolchain_set:
	echo my objects $(OBJ)
ifdef HAS_CC
	@echo "[INFO] using $(CC) "
else ifdef HAS_DOCKER_CC
	@echo "[INFO] Re-running in $(DOCKER_CC)"
	$(DOCKER_BIN) run --rm -v $(ROOT_DIR):/build $(DOCKER_CC) $(MAKE) $(MAKECMDGOALS)
else ifdef HAS_DOCKER
	@echo "[INFO] $(CC) not found, pulling $(DOCKER_CC)"
	$(DOCKER_BIN) pull --platform linux/amd64 $(DOCKER_CC)
	$(MAKE) $(MAKECMDGOALS)
else
	@$(error "[ERROR] Couldn't find $(CC) or use docker")
endif

$(NAME): toolchain_set $(OBJ)
	echo $(OBJ)
ifdef HAS_CC
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lgcc
endif

$(IMG_NAME): $(NAME)

 %.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

 %.o: %.s
	$(AS) $< -o $@