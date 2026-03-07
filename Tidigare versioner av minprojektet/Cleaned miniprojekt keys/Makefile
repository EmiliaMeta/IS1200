# ============================================================
#  PocketCreature Makefile – Works on both DTEK-V and PC
# ============================================================

SRC_DIR ?= ./
OBJ_DIR ?= ./
LINKER  := $(SRC_DIR)/dtekv-script.lds

# ============================================================
#  OS detection (Windows vs Linux)
# ============================================================
ifeq ($(OS),Windows_NT)
    RM := del /Q
    TRUE := rem
    EXE := .exe
else
    RM := rm -f
    TRUE := true
    EXE :=
endif

# ============================================================
#  Toolchains & Flags
# ============================================================

# --- DTEK-V (RISC-V) ---
TOOLCHAIN_DTEKV = riscv32-unknown-elf-
CFLAGS_DTEKV = -Wall -nostdlib -O3 -mabi=ilp32 -march=rv32imzicsr -fno-builtin

# --- PC Build ---
TOOLCHAIN_PC =
CFLAGS_PC = -Wall -O2 -DPC_TEST

# ============================================================
#  Source selection
# ============================================================

ifeq ($(MAKECMDGOALS),pc)
    # Only C sources in PC build (no .S assembly)
    SOURCES := $(wildcard $(SRC_DIR)/*.c)
else ifeq ($(MAKECMDGOALS),pc-run)
    SOURCES := $(wildcard $(SRC_DIR)/*.c)
else
    # DTEK-V: include both C and Assembly
    SOURCES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.S)
endif

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(SOURCES)))

# ============================================================
#  Targets
# ============================================================

.PHONY: all clean pc pc-run dtekv run

# Default target (DTEK-V)
all: dtekv

# ------------------------------------------------------------
#  DTEK-V Build
# ------------------------------------------------------------
dtekv:
	@echo Building for DTEK-V (RISC-V)...
	$(TOOLCHAIN_DTEKV)gcc -c $(CFLAGS_DTEKV) $(SOURCES)
	$(TOOLCHAIN_DTEKV)ld -o main.elf -T $(LINKER) $(filter-out boot.o, $(OBJECTS)) softfloat.a
	$(TOOLCHAIN_DTEKV)objcopy --output-target binary main.elf main.bin
	$(TOOLCHAIN_DTEKV)objdump -D main.elf > main.elf.txt
	@echo ✅ Build complete for DTEK-V!

# ------------------------------------------------------------
#  PC Build
# ------------------------------------------------------------
pc:
	@echo Building for PC (Windows/Linux)...
	gcc -c $(CFLAGS_PC) $(SOURCES)
	@echo Linking for PC...
	gcc -o main.exe $(OBJECTS)
	@echo ✅ PC build complete: main.exe

# ------------------------------------------------------------
#  PC Build + Run
# ------------------------------------------------------------
pc-run:
	$(MAKE) pc
	@echo ------------------------------------------
	@echo Running PocketCreature in PC simulation...
	@echo ------------------------------------------
	@./main.exe

# ------------------------------------------------------------
#  Cleanup
# ------------------------------------------------------------
clean:
	@echo Cleaning project...
	-$(RM) *.o *.elf *.bin *.txt *.exe 2>nul || $(TRUE)
	@echo ✅ Clean complete!
