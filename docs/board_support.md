# Board Support

MicrOS uses **YAML-based board descriptions** to generate headers.

## Example board file
```yaml
model: "nRF52840-DK"
cpus: { arch: arm, core: cortex-m4f, freq_mhz: 64 }
uart0:
  type: uart
  base: 0x40002000
  irq: 8
  tx_pin: P0.06
  rx_pin: P0.08
led0: { pin: P0.13, active_low: false }
````

## Adding a new board

1. Create folder: `boards/<vendor>/<board>/`
2. Add `board.yaml`
3. Add linker script & CMake config
4. Run generator (`tools/genheaders.py`)

Generated headers appear in `build/boards/<board>/`.
