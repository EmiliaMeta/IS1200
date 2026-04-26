# IS1200: Computer Hardware Engineering (C & RISC-V Assembly)

Course work from **IS1200 Computer Hardware Engineering** at KTH Royal Institute of Technology.

The course covers low-level programming in C and RISC-V assembly, embedded I/O, interrupt handling, processor design in Logisim, and hardware-software integration on the DTEK-V board.

> All labs and the project were done in collaboration with Alena Arsalan Amir.  
> **My contributions:** Full involvement across all parts; coding, hardware integration, I/O configuration, testing, and report writing.

---

## Featured Project: Pocketcreature

An interactive fiction and battle game running directly on the **DTEK-V board with I/O shield**, implemented entirely in C.

The player explores nine unique rooms, collects items, catches creatures, and battles gym leaders to ultimately defeat a final boss — with all game logic, story, and hardware I/O written from scratch.

### Running the Game

```bash
# Navigate to project folder
cd [path to folder]

# Compile
make

# Upload and run on DTEK-V board
dtekv-run main.bin
```

Press **BTN0** to reset the board before loading a new program. Game text (story, menus, dialogue) is displayed in the **JTAG UART Terminal**.

### Controls

| Control | Function |
|---------|---------|
| SW1–SW4 | Navigate menus and rooms |
| LEDs | Indicate current room |
| 7-segment display | Shows player and enemy HP |

### Goal

Follow **Professor Maple's** tutorial, catch creatures, defeat all gym leaders, and beat the **final boss in room 9**.

---

## Labs

### Lab 1 — Assembly Programming (RISC-V)
**Concepts:** RISC-V assembly, subroutines, register conventions, bit manipulation, hardware timing, DTEK-V board
 
Six assignments building up a complete clock program from scratch in RISC-V assembly:
- **Analyzing assembly** — single-stepped through existing code in RARS simulator, modified it to print every third ASCII character
- **hexasc** — wrote a subroutine converting numbers 0–15 to ASCII hex characters (`'0'`–`'9'`, `'A'`–`'F'`) using only the 4 least significant bits of the input, ignoring all others
- **time2string** — wrote a subroutine converting NBCD-encoded time data into a printable `MM:SS\0` string, calling `hexasc` for each digit while correctly saving and restoring all callee-saved registers
- **delay** — translated a nested C loop into assembly, calibrated the loop constant for accurate millisecond timing on the DTEK-V board (30 MHz clock)
- **DTEK-V deployment** — ported all code from RARS simulator to real hardware, handled differences in assembler directives (`.global`, macro syntax) between RARS and GCC, verified timing accuracy within ±10%

---

### Lab 2: C Programming
**Concepts:** Control flow, functions, arrays, pointers, memory layout, stack vs. heap

Five assignments covering C fundamentals:
- **is_prime()** — primality check using loops and conditionals
- **print_primes()** — printing primes in columns using functions and side effects
- **Sieve of Eratosthenes** — implemented with both stack and heap allocation
- **Pointer arithmetic** — translating RISC-V assembly functions (`work`, `copycodes`) into equivalent C using raw pointer operations, no array indexing
- **Memory layout** — inspecting and explaining where global variables, local variables, pointers, and functions are placed in memory on the DTEK-V board

---

### Lab 3 — I/O Programming
**Concepts:** Memory-mapped I/O, polling, hardware timers, interrupt handling, RISC-V calling conventions

Three progressively more advanced assignments on the DTEK-V board:
- **Polling** — reading toggle switches and push-buttons, driving LEDs and 7-segment displays; implemented a real-time clock (HH:MM:SS) with switch-based time adjustment
- **Timer** — replaced busy-wait delays with a hardware timer (30 MHz crystal, 100 ms timeouts) for precise second-accurate display updates
- **Interrupts** — moved time display updates into an interrupt service routine (ISR), freeing the main loop to run a prime number computation concurrently; enabled timer interrupts in RISC-V `mstatus`/`mie` registers via assembly

---

### Lab 4: Processor Design
**Concepts:** Digital logic, ALU design, register files, control units, data paths, RISC-V instruction encoding

Built a functional single-cycle RISC-V processor in **Logisim**, step by step:
- **ALU** — implemented add, subtract (two's complement), AND, OR, and SLT with a Zero output flag
- **Register file** — 8×32-bit registers with 2 read ports and 1 write port; hardwired register `x0` to always read zero
- **Control unit** — decode logic for `addi`, `add`, and `beq` instructions including branch condition evaluation
- **Data path** — full single-cycle pipeline: PC increment, ROM instruction fetch, register read/write, ALU computation, and conditional branching
- **Factorial in assembly** — wrote a RISC-V program computing n! using only `add`, `addi`, and `beq`, loaded it into the processor's ROM, and verified results for 0!, 3!, and 8!

---

## Tech Stack

| Tool | Purpose |
|------|---------|
| C | Lab 2, Lab 3, Pocketcreature game logic |
| RISC-V Assembly | Lab 1, Lab 4 factorial, boot code |
| Logisim Evolution | Lab 4 processor design |
| DTEK-V board | Hardware target for Labs 3, 4 and Pocketcreature |
| RARS simulator | RISC-V assembly simulation and testing |
| gcc / make | Compilation toolchain |

---

## Authors

**Emilia Lindqvist** — KTH Information Technology  
[GitHub Profile](https://github.com/EmiliaMeta)
