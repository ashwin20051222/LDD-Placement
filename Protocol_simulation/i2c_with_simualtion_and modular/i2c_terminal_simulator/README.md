# I²C Protocol Terminal Simulator

A modular, software-only simulator of a multi-master I²C bus written in strict
C11. It models physical **open-drain wired-AND** behaviour on SDA/SCL, lossless
**multi-master arbitration**, **clock stretching**, and a **custom protocol
modification** that inserts a UART-style **even-parity bit** between every data
byte and its receiver acknowledgement.

The whole simulation is driven by a virtual tick clock and rendered live in
your terminal as a scrolling ASCII waveform with an event log beside it.

---

## Table of Contents

1. [Custom Protocol Modification](#custom-protocol-modification)
2. [Project Layout](#project-layout)
3. [Building](#building)
4. [Running](#running)
5. [Terminal Command Reference](#terminal-command-reference)
6. [Example Sessions](#example-sessions)
7. [Reading the ASCII Waveform](#reading-the-ascii-waveform)
8. [Exit Codes & Errors](#exit-codes--errors)
9. [Design Notes](#design-notes)

---

## Custom Protocol Modification

This is **not** standard I²C. Every transmitted byte is followed by an
**even-parity bit calculated by the sender**, and only then comes the
receiver's ACK/NACK. One byte therefore occupies **10 SCL ticks** instead of 9:

```
 tick :   0   1   2   3   4   5   6   7   |   8    |   9
 line : [b7][b6][b5][b4][b3][b2][b1][b0]  | PARITY |  ACK/NACK
                                          ^        ^
                                          sender   receiver
                                          drives   drives
```

- **Parity rule**: even — i.e. `parity = XOR of the 8 data bits`. Total set
  bits across `{data, parity}` is always even.
- **Receiver behaviour**: the receiver re-computes parity over the 8 data bits
  it just sampled.
  - If parity **matches** and the byte is otherwise acceptable → drives
    SDA LOW (**ACK**) in tick 9.
  - If parity **mismatches** → releases SDA HIGH (**NACK**) in tick 9. The
    master observes this and tears the transaction down with STOP. The byte is
    **not** stored in the slave's memory bank.

The address byte is treated identically — it also carries a parity bit before
the addressed slave's ACK slot.

Full transaction sequence:

```
START
 ├─ 7-bit addr + R/W       (8 ticks)
 ├─ parity(addr+rw)        (1 tick )
 ├─ slave ACK              (1 tick )
 ├─ data byte 1            (8 ticks)
 ├─ parity(data 1)         (1 tick )
 ├─ receiver ACK           (1 tick )
 ├─  ...
 ├─ data byte N            (8 ticks)
 ├─ parity(data N)         (1 tick )
 └─ receiver ACK           (1 tick )
STOP
```

---

## Project Layout

```
i2c_terminal_simulator/
├── Makefile
├── main.c                              -- entry point + REPL bootstrapping
├── core/
│   ├── i2c_types.h                     -- shared enums, constants, helpers
│   └── i2c_clock.h                     -- header-only virtual tick clock
├── bus/
│   ├── i2c_bus.h
│   └── i2c_bus.c                       -- wired-AND bus, device registry
├── master/
│   ├── i2c_master.h
│   └── i2c_master.c                    -- bit-banging master state machine
├── slave/
│   ├── i2c_slave.h
│   └── i2c_slave.c                     -- 256-byte memory bank + parity check
├── arbitration/
│   ├── i2c_arbitration.h
│   └── i2c_arbitration.c               -- lossless multi-master arbitration
├── protocol/
│   ├── i2c_protocol.h
│   └── i2c_protocol.c                  -- START/STOP, parity, ACK framing
└── ui/
    ├── i2c_terminal_ui.h
    └── i2c_terminal_ui.c               -- ASCII waveform + event log + REPL
```

Compile-time limits live in `core/i2c_types.h`:

| Constant            | Value | Meaning                                   |
| ------------------- | ----- | ----------------------------------------- |
| `I2C_ADDRESS_MAX`   | 127   | 7-bit address space                       |
| `MAX_MASTERS`       | 4     | Maximum concurrent bus masters            |
| `MAX_SLAVES`        | 8     | Maximum slaves wired to the bus           |
| `MAX_BUFFER`        | 256   | Slave memory bank size, bytes             |
| `MAX_TX_PAYLOAD`    | 64    | Max bytes a master may queue per txn      |

---

## Building

### Requirements

- A C11-capable C compiler (`gcc ≥ 4.9` or `clang ≥ 3.4`)
- GNU Make
- A POSIX-like terminal (Linux, macOS, WSL). 80 columns minimum, 100+ preferred.

### Build commands

From the `i2c_terminal_simulator/` directory:

```bash
make              # build → ./i2c_sim
make clean        # remove build/ directory
make distclean    # remove build/ and the i2c_sim binary
make rebuild      # distclean + build
make run          # build then launch the simulator
```

Compiler flags are fixed by the spec and applied to every translation unit:

```
-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align
-Wstrict-prototypes -Wmissing-prototypes -O2 -MMD -MP
```

`-MMD -MP` generate `*.d` dependency files under `build/`, so editing a header
correctly rebuilds every dependent `.o`.

---

## Running

```bash
./i2c_sim
```

You will land in an interactive terminal that looks like this (a 4-master,
8-slave default topology is created at startup):

```
+============================================================================+
|  I2C Protocol Terminal Simulator  --  custom even-parity variant           |
+============================================================================+

SCL : ________
SDA : ________
       ^ tick 0   (bus idle, lines pulled HIGH)

[BUS ] initialised: 4 masters, 8 slaves, all lines HIGH
[INFO] type 'help' for a list of commands.

i2c>
```

Type commands at the `i2c>` prompt. Each command either configures the
topology, queues a transaction, or steps the simulation forward.

### CLI flags

```bash
./i2c_sim [--script <file>] [--ticks-per-step N] [--no-color] [--quiet]
```

| Flag                 | Default | Effect                                            |
| -------------------- | ------- | ------------------------------------------------- |
| `--script <file>`    | none    | Read commands from `<file>` then drop to prompt   |
| `--ticks-per-step N` | 1       | How many SCL half-ticks `step` advances at once   |
| `--no-color`         | off     | Disable ANSI colour codes in the waveform/log     |
| `--quiet`            | off     | Suppress per-tick log lines, keep summaries       |

---

## Terminal Command Reference

Commands are **case-insensitive**, whitespace-separated, and one per line.
`#` starts a comment. Numeric arguments accept decimal (`42`), hex (`0x2A`),
or binary (`0b00101010`).

### Topology

| Command                                  | Description                                           |
| ---------------------------------------- | ----------------------------------------------------- |
| `slave add <addr> [name]`                | Register a slave at 7-bit address (0..127)            |
| `slave del <addr>`                       | Remove a slave                                        |
| `slave dump <addr>`                      | Print the slave's 256-byte memory bank               |
| `slave poke <addr> <offset> <byte>`      | Pre-load one byte into a slave (for read tests)       |
| `slave stretch <addr> <ticks>`           | Force the slave to hold SCL LOW for N ticks on next op |
| `master list`                            | Show all configured masters and their current state   |

### Transactions

| Command                                  | Description                                           |
| ---------------------------------------- | ----------------------------------------------------- |
| `write <master_id> <addr> <b0> [b1...]`  | Queue a WRITE from `master_id` to slave `addr`        |
| `read  <master_id> <addr> <n_bytes>`     | Queue a READ of `n_bytes` from slave `addr`           |
| `inject_parity_error <master_id>`        | Next byte sent by this master flips its parity bit    |
| `arbitrate <m1> <m2> <addr> <byte>`      | Queue two writes on the same tick → arbitration demo  |

### Simulation control

| Command                  | Description                                          |
| ------------------------ | ---------------------------------------------------- |
| `step [n]`               | Advance simulation by `n` ticks (default 1)          |
| `run`                    | Run until all queued transactions complete           |
| `pause`                  | Stop a `run` after the current tick                  |
| `reset`                  | Re-initialise bus, clear logs, keep configured devs  |
| `clear`                  | Clear the terminal screen                            |

### Diagnostics

| Command       | Description                                            |
| ------------- | ------------------------------------------------------ |
| `bus`         | Show current SDA/SCL levels and which devices drive   |
| `log [n]`     | Show last `n` log lines (default 20)                  |
| `wave [n]`    | Re-render the last `n` ticks of the waveform          |
| `help [cmd]`  | Show help, optionally for one command                  |
| `quit` / `exit` | Leave the simulator                                  |

---

## Example Sessions

### Example 1 — Single-master write, parity OK

**Input:**

```
slave add 0x42 EEPROM
write 0 0x42 0xA5
run
slave dump 0x42
```

**Output:**

```
[CFG ] slave registered  addr=0x42  name=EEPROM
[Q   ] master#0 queued WRITE addr=0x42 len=1 data=[0xA5]
[BUS ] -- run --
[M#0 ] START                                                  tick=    1
[M#0 ] TX ADDR  byte=0x84  (addr=0x42 rw=W)                   tick=    2
[M#0 ] TX PARITY bit=1  (addr byte has 3 set bits)            tick=   11
[S#42] ADDR MATCH -> ACK                                      tick=   12
[M#0 ] TX DATA  byte=0xA5                                     tick=   13
[M#0 ] TX PARITY bit=0  (data byte has 4 set bits)            tick=   22
[S#42] parity OK -> ACK                                       tick=   23
[M#0 ] STOP                                                   tick=   24
[M#0 ] done  ok=1  bytes=1

SCL : _|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_______
SDA : ‾|___|___|‾|___|___|‾|___|‾‾|___|___|‾‾‾‾‾‾‾‾‾‾
       S  1  0  0  0  0  1  0   P  A  ...        T

Slave 0x42 memory bank (first 16 bytes):
00: A5 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

The `S` annotation marks the START condition, each digit annotates a sampled
bit, `P` is the **custom parity bit**, `A` is the receiver ACK, and `T` is
STOP.

### Example 2 — Parity error injection (slave NACKs)

**Input:**

```
slave add 0x10 SENSOR
inject_parity_error 0
write 0 0x10 0x0F
run
```

**Output:**

```
[CFG ] slave registered  addr=0x10  name=SENSOR
[Q   ] master#0 queued WRITE addr=0x10 len=1 data=[0x0F]
[CFG ] master#0 next parity bit will be inverted
[BUS ] -- run --
[M#0 ] START                                                  tick=    1
[M#0 ] TX ADDR  byte=0x20  (addr=0x10 rw=W)                   tick=    2
[M#0 ] TX PARITY bit=1                                        tick=   11
[S#10] ADDR MATCH -> ACK                                      tick=   12
[M#0 ] TX DATA  byte=0x0F                                     tick=   13
[M#0 ] TX PARITY bit=1  (INJECTED, expected=0)                tick=   22
[S#10] parity FAIL  expected=0 received=1  -> NACK            tick=   23
[M#0 ] received NACK -> abort                                 tick=   23
[M#0 ] STOP                                                   tick=   24
[M#0 ] done  ok=0  bytes=0  reason=PARITY_NACK
```

Note the slave **never stored the byte** because parity validation came first.

### Example 3 — Two masters, arbitration

**Input:**

```
slave add 0x55 RAM
arbitrate 0 1 0x55 0xC3
run
```

What `arbitrate` does: it schedules `master#0` to write `0xC3` and `master#1`
to write `0xFF` (or any different first byte) to the **same** slave on the
**same** tick. Their first bits are sampled simultaneously; the master writing
a `1` while the bus reads `0` loses arbitration silently.

**Output:**

```
[Q   ] master#0 queued WRITE addr=0x55 data=[0xC3]
[Q   ] master#1 queued WRITE addr=0x55 data=[0xFF]
[BUS ] -- run --
[M#0 ] START                                                  tick=    1
[M#1 ] START                                                  tick=    1
[M#0 ] TX ADDR bit b7=1                                       tick=    2
[M#1 ] TX ADDR bit b7=1                                       tick=    2
   ...
[M#0 ] TX ADDR bit b1=1     (bus reads 0)                     tick=    8
[M#0 ] ARBITRATION LOST -> release bus, no STOP               tick=    8
[M#1 ] continues uninterrupted                                tick=    8
[M#1 ] TX PARITY bit=...                                      tick=   11
[S#55] ADDR MATCH -> ACK                                      tick=   12
   ...
[M#1 ] done  ok=1  bytes=1
[M#0 ] done  ok=0  bytes=0  reason=ARBITRATION_LOST
```

### Example 4 — Read with clock stretching

**Input:**

```
slave add 0x33 ADC
slave poke 0x33 0x00 0xDE
slave poke 0x33 0x01 0xAD
slave stretch 0x33 4
read 0 0x33 2
run
```

**Output:**

```
[CFG ] slave registered  addr=0x33  name=ADC
[CFG ] slave 0x33 memory[0x00]=0xDE
[CFG ] slave 0x33 memory[0x01]=0xAD
[CFG ] slave 0x33 will stretch SCL for 4 ticks on next op
[Q   ] master#0 queued READ addr=0x33 len=2
[BUS ] -- run --
[M#0 ] START                                                  tick=    1
[M#0 ] TX ADDR byte=0x67 (addr=0x33 rw=R)                     tick=    2
[M#0 ] TX PARITY bit=1                                        tick=   11
[S#33] ADDR MATCH -> ACK                                      tick=   12
[S#33] CLOCK STRETCH start  (holding SCL low)                 tick=   13
[BUS ] SCL held LOW by slave 0x33                             tick=   13..16
[S#33] CLOCK STRETCH end                                      tick=   17
[S#33] TX DATA byte=0xDE                                      tick=   18
[S#33] TX PARITY bit=1                                        tick=   27
[M#0 ] parity OK -> ACK                                       tick=   28
[S#33] TX DATA byte=0xAD                                      tick=   29
[S#33] TX PARITY bit=1                                        tick=   38
[M#0 ] parity OK -> NACK (last byte)                          tick=   39
[M#0 ] STOP                                                   tick=   40
[M#0 ] done  ok=1  bytes=2  rx=[0xDE,0xAD]
```

The convention "NACK on last received byte" is standard I²C and is preserved.

### Example 5 — Script mode

Save commands to a file `demo.i2c`:

```
# demo.i2c — basic smoke test
slave add 0x42 EEPROM
slave add 0x10 SENSOR
write 0 0x42 0xA5 0x5A
read  0 0x10 1
run
log 10
quit
```

Then launch:

```bash
./i2c_sim --script demo.i2c
```

The simulator executes the file as if you'd typed it, prints the resulting
log, and exits.

---

## Reading the ASCII Waveform

```
SCL : _|‾|_|‾|_|‾|_|‾|
SDA : ‾|_____|‾|_____|
       S  b7 b6 b5 b4 ...
```

| Symbol       | Meaning                                     |
| ------------ | ------------------------------------------- |
| `_`          | line is LOW                                 |
| `‾`          | line is HIGH                                |
| `|`          | edge transition between this tick & previous |
| `S`          | START condition                             |
| `T`          | STOP condition                              |
| `P`          | **custom parity bit slot**                  |
| `A` / `N`    | ACK / NACK slot                             |
| digit `0/1`  | sampled value of a data bit                 |
| `~` (under SCL) | clock stretching by a slave              |
| `X` (under SDA) | bus contention point (arbitration loss)  |

If your terminal can't render `‾` cleanly, run with `--no-color` *and* set
`TERM=dumb`; the renderer will fall back to `-` for HIGH and `_` for LOW.

---

## Exit Codes & Errors

| Code | Meaning                                            |
| ---- | -------------------------------------------------- |
| `0`  | Normal exit (`quit`, end of script, EOF on stdin) |
| `1`  | Command-line argument error                        |
| `2`  | Script file could not be opened or parsed          |
| `3`  | Out-of-memory during device registration           |
| `4`  | Internal invariant violated (should never happen)  |

Common runtime errors surface as `[ERR ]` lines in the log:

| Log message                              | Cause                                       |
| ---------------------------------------- | ------------------------------------------- |
| `[ERR ] slave 0xNN already registered`   | Address collision in `slave add`            |
| `[ERR ] master_id out of range`          | First arg to `write`/`read` ≥ `MAX_MASTERS` |
| `[ERR ] payload too large (max 64)`      | More than 64 bytes queued in one txn         |
| `[ERR ] no slave responds at 0xNN`       | Address NACK — slave not on bus             |
| `[ERR ] parity mismatch on byte k`       | The custom parity check failed              |

---

## Design Notes

- **Determinism.** All time advance flows through a single virtual tick
  counter (`core/i2c_clock.h`). The same input script always produces the
  same output, byte-for-byte. There is no wall-clock dependency, no
  `rand()`, no threading.
- **Wired-AND bus.** Devices never "set" SDA or SCL to HIGH — they either
  pull LOW or release. The bus resolves the wire as
  `AND(release[0], release[1], ...)`. This is the only correct way to model
  multi-master open-drain behaviour in software.
- **Arbitration is lossless.** A master that loses arbitration drops out
  silently at the very tick it detects mismatch; it does **not** emit a
  STOP and does **not** corrupt the bus. The winning master sees nothing
  unusual.
- **Parity is mandatory.** There is no compile-time switch to disable the
  parity bit — the custom frame is the simulator's whole reason to exist.
- **No dynamic allocation in the hot path.** Every buffer is sized by the
  constants in `core/i2c_types.h` and embedded in its owning struct, so
  `make run` will never OOM mid-simulation.

---

## Licence

This codebase is intended for educational use as part of the LDD coursework
exercise. Reuse and modify freely.
