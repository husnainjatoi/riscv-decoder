# RISC-V RV32I Instruction Decoder

[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white&labelColor=00599C)]()
[![GNU Make](https://img.shields.io/badge/GNU%20Make-008080?style=flat-square&logo=gnu&logoColor=white)]()
[![Status](https://img.shields.io/badge/status-active-success)]()
[![License](https://img.shields.io/badge/license-MIT-green)]()

**Author:** Husnain Jatoi  
**Project:** MEDS Lab Module 2 Capstone  

## Project Description
An automated C-based RISC-V RV32I instruction decoder engineered to parse raw machine code into human-readable assembly. This tool serves as the front-end validation layer for hardware simulation, demonstrating robust dynamic memory management, precise bitwise manipulation, and automated Software Quality Assurance (SQA) workflows.

## Repository Structure
~~~text
riscv-decoder/
├── docs/
│   └── DESIGN.md               # Architectural blueprint and engineering decisions
├── include/
│   ├── decoder.h               # Core instruction decoding data structures
│   └── memory.h                # Dynamic memory allocation definitions
├── src/
│   ├── decoder.c               # Datapath, bit extraction, and formatting logic
│   ├── main.c                  # Execution loop and formatted console output
│   └── memory.c                # File I/O and dynamic heap management
├── test/
│   ├── programs/               # Hexadecimal machine code test vectors
│   │   ├── branch.hex
│   │   ├── i_type.hex
│   │   ├── j_type.hex
│   │   ├── mixed.hex
│   │   ├── r_type.hex
│   │   ├── s_type.hex
│   │   └── u_type.hex
│   └── test_decoder.c          # Automated SQA unit test suite
├── Makefile                    # Automation layer for build, test, and diagnostics
└── README.md                   # Project documentation
~~~

## Core Capabilities
* **Full Base Integer Support:** Extracts and decodes R-Type, I-Type, S-Type, B-Type, U-Type, and J-Type instructions natively, covering 100% of the standard datapath edge cases.
* **Dynamic Memory Management:** Safely parses hexadecimal machine code files of arbitrary length with proactive capacity scaling and strict zero-leak constraints.
* **Automated SQA Validation:** Enforces zero memory leaks through automated Valgrind integration and validates core logic via standalone C unit testing.
* **Hardware Architecture Alignment:** Utilizes wire-slicing bitwise macros and rigorous sign-extension logic to mirror physical RISC-V processor datapaths.

## Installation and Setup

### Prerequisites
Ensure the following development dependencies are installed on your Linux environment:
* **GCC** (GNU Compiler Collection)
* **GNU Make**
* **Valgrind** (for memory leak diagnostics)

### Installation
Clone the repository and navigate into the project directory:
```bash
git clone https://github.com/husnainjatoi/riscv-decoder
cd riscv-decoder
```

## Build Instructions
This project utilizes a Make-based build system to automate the compilation, execution, and evaluation pipeline. 

Ensure GCC, Make, and Valgrind are installed on your Linux environment.

~~~bash
# Display all available build and test commands
make help

# Build the production executable
make

# Compile and execute the decoder on a default or specified file
make run
make run FILE=test/programs/j_type.hex

# Execute the automated SQA unit test suite
make test

# Run dynamic memory leak analysis on the compiled binary
make valgrind

# Clean the workspace of object files and binaries
make clean
~~~

## Usage Examples
If bypassing the `make run` target, provide the path to a compiled hexadecimal text file as the single argument to the compiled binary.

~~~bash
./bin/riscv-decoder test/programs/mixed.hex
~~~

## Execution Summary (Sample Output)
~~~text
RISC-V RV32I Instruction Decoder
========================================================

Loaded 8 instructions from test/programs/mixed.hex

Addr        Hex         Assembly
--------------------------------------------------------
0x00000000 00500113    addi    x2, 5(x0)
0x00000004 00A00193    addi    x3, 10(x0)
0x00000008 003100B3    add     x1, x2, x3
0x0000000C 40310133    sub     x2, x2, x3
0x00000010 0020A023    sw      x2, 0(x1)
0x00000014 0000A103    lw      x2, 0(x1)
0x00000018 FE209CE3    bne     x1, x2, -8
0x0000001C 004000EF    jal     x1, 4

Decoded 8 instructions (8 valid, 0 unknown)
~~~