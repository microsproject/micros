# Porting Guide

MicrOS is designed to run on multiple architectures (ARM Cortex-M, RISC-V, POSIX).

## Steps to add a new architecture
1. **Startup code**
   - Reset handler, stack pointer setup, vector table
2. **Context switch**
   - Implement `arch_swap()`, save/restore registers
   - Configure PendSV (ARM) or software interrupt (RISC-V)
3. **Tick source**
   - SysTick (ARM) or CLINT (RISC-V)
4. **Interrupt handling**
   - Provide `arch_irq_enable/disable`
5. **Toolchain support**
   - Add `toolchain-<arch>.cmake`
   - Provide linker script

## Adding a new board
- Create `boards/<vendor>/<board>/`
- Add `board.cmake`, linker script, pinmux
- Provide `board.yaml` (device tree–like)

See [Board Support](board_support.md) for details.