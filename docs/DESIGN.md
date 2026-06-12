# Architectural Design & Engineering Decisions

## 1. Subsystem Isolation (Modularity)
To keep the code clean and easy to test, the project is broken down into specific, single-purpose files:
* **`memory.c`:** Handles all file reading and memory management. It converts strings to hex and scales memory dynamically as needed.
* **`decoder.c`:** The core logic engine. It takes raw 32-bit instructions and decodes them, without worrying about where the data came from.
* **`main.c`:** The main control loop. It connects the memory logic to the decoder and formats the final output for the terminal.

## 2. Hardware-Mirrored Bit Extraction
To mimic how physical wires are sliced in hardware design (like Verilog), all bit extraction goes through one shared C macro:

```c
#define EXTRACT_BITS(inst, start, end) (((inst) >> (start)) & ((1U << ((end) - (start) + 1)) - 1))
```

Instead of writing messy bitwise logic over and over, this macro uses bit-shifting and masking to cleanly grab opcodes, registers, and immediates right from the instruction.

## 3. Immediate Sign-Extension Safety
RISC-V branches and jumps rely heavily on negative numbers. A major design goal was making sure these sign extensions work safely across different systems.
* Immediates are stored specifically as signed 32-bit integers (`int32_t`).
* The code checks the Most Significant Bit (MSB) to see if the number is negative.
* If it is, the code pads the upper bits by safely shifting an unsigned block of 1s (`0xFFFFFFFFU`) before converting it back to a signed integer. This prevents the compiler warnings and undefined behavior caused by left-shifting negative values in C.

## 4. Elimination of Magic Numbers
To keep the code readable and easy to maintain, hardcoded hex values were completely removed. Instead, the official RISC-V opcodes are mapped to an `opcode_t` enum. The main switch statement uses clear labels (like `OP_I_TYPE_ARITH` or `OP_J_TYPE_JAL`), making the routing logic instantly obvious to anyone reading the code.

## 5. Automated Testing & Memory Validation
A solid system needs reliable, automated testing to prove it actually works.
* **Unit Testing:** A standalone test file (`test_decoder.c`) uses standard C assertions to check tricky edge cases, like scrambled B/J-Type immediates and split S-Type offsets without needing to read external files.
* **Memory Leak Checks:** The decoder scales dynamically using `realloc` to handle any file size. The `make valgrind` command automatically runs the program through memory checks to guarantee absolutely zero memory leaks before termination.