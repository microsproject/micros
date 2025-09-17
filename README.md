# MicrOS

**MicrOS — simple, open, embedded**

MicrOS is a lightweight, open-source real-time operating system for microcontrollers and small systems.  
Inspired by Zephyr and FreeRTOS, but designed to stay tiny, transparent, and hackable.

## ✨ Features (MVP)
- Cooperative + (optional) preemptive scheduler
- Mutexes, semaphores, message queues
- Tickless timers
- Portable across ARM Cortex-M, RISC-V, and POSIX (native sim)
- Clear driver model with initcalls
- YAML-based board description → generated headers
- Open governance, Apache-2.0 licensed

## 🚀 Getting Started
```bash
git clone https://github.com/YOURNAME/micros.git
cd micros
cmake -S . -B build -DMICROS_PORT=posix
cmake --build build -j
./build/samples/blinky/blinky
````

For hardware (e.g. nRF52840 DK), see [docs/getting\_started.md](docs/getting_started.md).

## 📚 Documentation

* [Getting Started](docs/getting_started.md)
* [Porting Guide](docs/porting_guide.md)
* [Driver Model](docs/driver_model.md)
* [Concurrency Model](docs/concurrency.md)

## 🤝 Contributing

We welcome contributions of all forms: code, docs, samples, issue triage.
See [CONTRIBUTING.md](CONTRIBUTING.md).

## 📜 License

MIT — see [LICENSE](LICENSE).

