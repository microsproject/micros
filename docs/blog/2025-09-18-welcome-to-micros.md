---
title: "Welcome to MicrOS"
date: 2025-09-18
tags:
    - embedded
    - operating system
    - open source
    - education
    - announcement
---
MicrOS is a new open-source embedded operating system designed to be **simple, open, and educational**.
It’s inspired by great projects like [Zephyr](https://zephyrproject.org/), [FreeRTOS](https://www.freertos.org/), and [RIOT](https://www.riot-os.org/), but with a twist: MicrOS aims to stay small, transparent, and beginner-friendly.

---

## 🎯 Why MicrOS?

The embedded ecosystem is rich with feature-packed RTOSes. They are powerful, but often come with steep learning curves, complex build systems, and thousands of lines of code to digest before you can understand the basics.

MicrOS was born to answer a simple question:

> *What if we had an OS that anyone could read, understand, and hack on — without drowning in complexity?*

MicrOS is therefore designed with:

* **Clarity first**: every line of code has a purpose.
* **Education in mind**: great for learning context switching, scheduling, and drivers.
* **Open community**: MIT licensed, open to contributions, easy to fork and experiment with.

---

## 🧩 First Milestone

The very first MicrOS sample will showcase **context switching** running on:

* **STM32 (Cortex-M)** hardware
* **QEMU ARM emulation**

This is a crucial step: once the OS can switch between tasks, everything else — scheduling, drivers, user applications — builds on top of it.

Expect a full walkthrough blog post soon 👀

---

## 📂 Code Organization

The [repository](https://github.com/microsproject/micros) is structured with simplicity in mind:

* `kernel/` – core OS logic (context switch, scheduler)
* `arch/` – architecture-specific code (Cortex-M, RISC-V in the future)
* `drivers/` – simple, modular drivers
* `samples/` – example applications to learn from
* `docs/` – documentation and blog posts (like this one!)

The codebase is set around `Kconfig` and CMake for fast pick-up and start of your next project.
---

## 🙌 How You Can Get Involved

MicrOS is at the very beginning of its journey. If you’d like to help shape it, here’s how:

* ⭐ **Star the repo** to show support.
* 🐛 **Open issues** if you find bugs or have feature requests.
* 🔧 **Contribute code** — even small things like fixing typos help.
* 📣 **Spread the word** if you believe in the idea of an open, educational embedded OS.

Contributions are welcome — check out the [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines.

---

## 🚀 What’s Next?

Here’s the rough roadmap:

1. Get a basic context switch running on STM32 + QEMU.
2. Add cooperative scheduler.
3. Implement basic drivers (GPIO, UART, Timer).
4. Release first "hello world" multitasking demo.
5. Grow community through tutorials and sample projects.

---

MicrOS is an experiment, a playground, and hopefully — a community project.
If you’ve ever wanted to **build your own OS, contribute to one, or just learn how these systems tick**, this is the place for you.

**MicrOS — simple, open, embedded.**

