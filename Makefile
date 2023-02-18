## Vars ##
# files
NAME		:= kfs
IMG_NAME	:= $(NAME).iso

# toolchain, hard-coded but we provide containers!
ARCH	=	i686
CC		:=	$(ARCH)-elf-gcc
HAS_CC	:= $(shell $(CC) 2>/dev/null)

# docker
DOCKER_BIN		?= docker
HAS_DOCKER		:= $(shell $(DOCKER_BIN) ps 2>/dev/null)
DOCKER_CC		:= ghcr.io/l-aurelie/i686-cc:latest
HAS_DOCKER_CC = $(shell $(DOCKER_BIN) image inspect $(DOCKER_CC) 2>/dev/null) 

# helpers
ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

## Rulez ##
.PHONY: all

all:
	@$(MAKE) $(NAME)
	# $(IMG_NAME)

$(NAME):
ifdef HAS_CC
	@echo "[INFO] using $(CC) "
else ifdef HAS_DOCKER_CC
	@echo "[INFO] Re-running in $(DOCKER_CC)"
	$(DOCKER_BIN) run --rm -v $(ROOT_DIR):/build $(DOCKER_CC) make $(NAME)
else ifdef HAS_DOCKER
	@echo "[INFO] $(CC) not found, pulling $(DOCKER_CC)"
	$(DOCKER_BIN) pull --platform linux/amd64 $(DOCKER_CC)
else
	@$(error "[ERROR] Couldn't find $(CC) or use docker")
endif

$(IMG_NAME): $(NAME)
