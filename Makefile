#
# Kurzanleitung
# =============
#
# +++ General Targets +++
#
# make                  -- siehe -> make all
#
# make all              -- Baut kernel.elf und generiert einen dump
#
# make install          -- Baut kernel.img und transferiert ihn auf den Server.
#                          Das Board holt sich diesen Kernel beim nächsten Reset.
#
# make qemu             -- Baut den Kernel und führt ihn unter QEMU aus
#
# make qemu_debug       -- Baut den Kernel und führt ihn unter QEMU mit debug
#                          Optionen aus. Zum debuggen in einem zweiten Terminal
#                          folgendes ausführen:
#                          $ arm-none-eabi-gdb build/kernel.elf
#                          $ target remote localhost:1234
#
# make clean            -- Löscht alle erzeugten Dateien.
#
# make submission       -- Verpackt alle Dateien die von git getrackt sind
#                          Hierbei wird immer der "main" branch genutzt!
#
# make home             -- kopiert das fertige image nach $TFTP_PATH, für die
#                          Arbeit zuhause einfach den Pfad eintragen
#
# +++ Single File Targets +++
#
# make kernel           -- Baut kernel.elf im build Ordner
#
# make kernel.bin       -- Baut kernel.bin im build Ordner
#
# make kernel.img       -- Baut kernel.img im build Ordner
#
# make kernel_only      -- Baut den kernel aber nur mit den Dateien aus SRC
#                          Zum testen, ob der Kernel unabhängig kompiliert
#
# make user_only        -- Baut nur die User Dateien aus USRC
#                          Zum testen, ob Userland unabhängig kompiliert

# Binäre Lsg (falls verwendet, ansonsten leer lassen)
BIN_LSG = 

# Hier eure source files hinzufügen
SRC = arch/cpu/mmu.c arch/cpu/mmu_asm.S arch/cpu/syscall_routines.c kernel/thread.c arch/cpu/exception_creator.S arch/cpu/kernel_access.S arch/cpu/exception_handler.c arch/cpu/entry.S arch/cpu/ivt.S arch/cpu/interrupt.c kernel/start.c arch/bsp/yellow_led.c lib/utils.c arch/bsp/pl011_uart.c arch/bsp/timer.c tests/regcheck.c tests/regcheck_asm.S

# Hier separate user source files hinzufügen
USRC = user/main.c kernel/syscall.c

# Hier können eigene GCC flags mit angegeben werden.
# Die vorgegebenen Flags können weiter unten gefunden werden unter
# CFLAGS_ALL und dürfen nicht verändert werden
# Bsp: CFLAGS = -Wpedantic -Werror -O2
CFLAGS =

# Hier den git branch angeben, der für die Abgabe benutzt werden soll
SUBMISSION_BRANCH = main

# Wenn ihr zuhause auf der Hardware arbeitet, hier das TFTP-Verzeichnis eintragen
TFTP_PATH = /srv/tftp

# +------------------------------------+
# |                                    |
# |   Ab hier nichts mehr angepasst!   |
# |                                    |
# +------------------------------------+

# build directory
BUILD_DIR = build

# separate kernel files and add build prefix
KSRC_C = $(filter %.c, $(SRC))
KSRC_S = $(filter %.S, $(SRC))
KOBJ_C = $(addprefix $(BUILD_DIR)/,$(KSRC_C:%.c=%.o))
KOBJ_S = $(addprefix $(BUILD_DIR)/,$(KSRC_S:%.S=%.o))
KOBJ =  $(KOBJ_C) $(KOBJ_S)

# separate user files and add build prefix
USRC_C = $(filter %.c, $(USRC))
USRC_S = $(filter %.S, $(USRC))
UOBJ_C = $(addprefix $(BUILD_DIR)/,$(USRC_C:%.c=%.o))
UOBJ_S = $(addprefix $(BUILD_DIR)/,$(USRC_S:%.S=%.o))
UOBJ =  $(UOBJ_C) $(UOBJ_S)

# accumulate
OBJ_C = $(KOBJ_C) $(UOBJ_C)
OBJ_S = $(KOBJ_S) $(UOBJ_S)
OBJ = $(KOBJ) $(UOBJ)

# auto generated dep files
DEP = $(OBJ:.o=.d)

# linker script
LSCRIPT = kernel.lds

# static lib location for binary solution
BIN_LSG_DIR = ./
BIN_LSG_FILE = $(BIN_LSG_DIR)/lib$(BIN_LSG).a

# toolchain
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
IMG = mkimage
QEMU = qemu-system-arm

# configuration
CFLAGS_ALL = -Wall -Wextra -ggdb -ffreestanding -mcpu=cortex-a7 $(CFLAGS)
CPPFLAGS = -Iinclude
LDFLAGS = -T$(LSCRIPT) -nostdlib
ifneq ($(BIN_LSG), )
	LDFLAGS += -L$(BIN_LSG_DIR) -l$(BIN_LSG)
endif
OBJCOPYFLAGS = -Obinary -S --set-section-flags .bss=contents,alloc,load,data
OBJDUMPFLAGS = -dfzh
IMGFLAGS = -A arm -T standalone -C none -a 0x8000
QEMUFLAGS = -M raspi2b -nographic

# Regeln
.PHONY: all

# default target
all: kernel dump

# include auto generated targets
-include $(DEP)

# single file targets
$(OBJ_S):$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS_ALL) -MMD -MP -o $@ -c $<

$(OBJ_C):$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS_ALL) -MMD -MP -o $@ -c $<

$(BUILD_DIR)/kernel.elf: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(BUILD_DIR)/kernel.img: $(BUILD_DIR)/kernel.bin
	$(IMG) $(IMGFLAGS) -d $< $@

$(BUILD_DIR)/kernel_only.elf: $(KOBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/user_only.elf: $(UOBJ)
	$(LD) -o $@ $^

$(BUILD_DIR)/kernel_dump.s: $(BUILD_DIR)/kernel.elf
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@

# aliases
.PHONY: dump kernel kernel.bin kernel.img kernel_only user_only
dump: $(BUILD_DIR)/kernel_dump.s
kernel: $(BUILD_DIR)/kernel.elf
kernel.bin: $(BUILD_DIR)/kernel.bin
kernel.img: $(BUILD_DIR)/kernel.img
kernel_only: $(BUILD_DIR)/kernel_only.elf
user_only: $(BUILD_DIR)/user_only.elf

# general targets
.PHONY: install home qemu qemu_debug clean submission
install: $(BUILD_DIR)/kernel.img
	arm-install-image $<

home: $(BUILD_DIR)/kernel.img
	cp -v $< $(TFTP_PATH)

qemu: $(BUILD_DIR)/kernel.elf
	$(QEMU) $(QEMUFLAGS) -kernel $<

qemu_debug: $(BUILD_DIR)/kernel.elf
	$(QEMU) $(QEMUFLAGS) -s -S -kernel $<

clean:
	rm -rf $(BUILD_DIR)

CURR_BRANCH = $(shell git branch --show-current)
STATUS_BRANCH = "$(shell git status --porcelain)"

submission:
ifneq ($(CURR_BRANCH), $(SUBMISSION_BRANCH))
	@echo "Refusing to create submission!"
	@echo "Current branch does not match submission branch."
	@echo " > Currently on branch: '$(CURR_BRANCH)'"
	@echo " > But registered submission branch is: '$(SUBMISSION_BRANCH)'"
	@echo "Please change branch or fix submission branch."
	@exit 1
endif
ifneq ($(STATUS_BRANCH), "")
	@echo "Refusing to create submission!"
	@echo "Submission branch is dirty or contains untracked files."
	@echo ""
	@echo "Please commit changes and add non essential files to '.gitignore'."
	@echo "Use 'git status' for a detailed list."
	@echo ""
	@exit 1
endif
	git archive --format=tar.gz -o submission.tar.gz $(SUBMISSION_BRANCH)
	@echo Successfully created submission.tar.gz
	@echo We recommend to test the created archive before submitting it.
