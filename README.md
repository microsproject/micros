# MicrOS

**MicrOS — simple, open, embedded**

[![Build - Samples](https://github.com/microsproject/micros/actions/workflows/build.yml/badge.svg)](https://github.com/microsproject/micros/actions/workflows/build.yml)
[![Security Policy](https://img.shields.io/badge/security-report--vulnerabilities-red)](SECURITY.md)


MicrOS is a lightweight, open-source real-time operating system for microcontrollers and small systems.  
Inspired by Zephyr and FreeRTOS, but designed to stay tiny, transparent, and hackable.

## ✨ Features (MVP)
- Preemptive scheduler
- Mutexes, semaphores, message queues
- Tickless timers
- Portable across ARM Cortex-M, RISC-V, and POSIX (native sim)
- Clear driver model with initcalls
- YAML-based board description → generated headers
- Open governance, MIT licensed

## 🚀 Getting Started
```bash
git clone https://github.com/microsproject/micros.git
cd micros
cmake -S . -B build/ -DMICROS_BOARD=lm/lm3s6965evb -DMICROS_SAMPLE=hello_world -DCMAKE_BUILD_TYPE=Debug
cmake --build build/

qemu-system-arm -M lm3s6965evb -nographic -kernel build/samples/hello_world/hello_world.bin
````

For hardware (e.g. nRF52840 DK), see [docs/getting\_started.md](docs/getting_started.md).

## 📚 Documentation

* [Getting Started](docs/getting_started.md)
* [Porting Guide](docs/porting_guide.md)
* [Driver Model](docs/driver_model.md)
* [Concurrency Model](docs/concurrency.md)

## 📜 Governance

* [Code of Conduct](CODE_OF_CONDUCT.md)  
* [Security Policy](SECURITY.md)  
* [Contributing Guide](CONTRIBUTING.md)  

## 📜 License

MIT — see [LICENSE](LICENSE).

