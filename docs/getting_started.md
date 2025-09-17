# Getting Started with MicrOS

Welcome to **MicrOS — simple, open, embedded**.  
This guide will help you build and run your first application.

## Requirements
- CMake ≥ 3.20
- GCC (arm-none-eabi for embedded, system GCC for POSIX)
- Ninja (recommended)
- Python 3.8+

## Building on POSIX (Linux/macOS/Windows WSL)
```bash
git clone https://github.com/YOURNAME/micros.git
cd micros
cmake -S . -B build -DMICROS_PORT=posix
cmake --build build
./build/samples/blinky/blinky
````

## Building for hardware (example: nRF52840 DK)

```bash
cmake -S . -B build -DBOARD=nrf52840dk
cmake --build build
nrfjprog -f nrf52 --program build/samples/blinky/blinky.hex --reset
```

## Next Steps

* Try more [samples](../samples)
* Read the [Driver Model](driver_model.md)
* Learn how [Concurrency](concurrency.md) works