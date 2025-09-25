---
name: 🏗️ Build/Toolchain Issue
about: Report problems with building, compilation, or toolchain setup
title: "[BUILD] "
labels: ["build", "toolchain", "needs-investigation"]
assignees: []
---

## Build Issue Type
- [ ] Compilation error
- [ ] Linker error
- [ ] CMake configuration issue
- [ ] Toolchain installation problem
- [ ] Missing dependencies
- [ ] Cross-compilation issue
- [ ] CI/CD pipeline problem

## Environment
- **Host OS**: (e.g., Ubuntu 22.04, Windows 11, macOS 13)
- **Toolchain**: 
  - arm-none-eabi-gcc version: `arm-none-eabi-gcc --version`
  - CMake version: `cmake --version`
- **Target Board**: (e.g., LM3S6965EVB, STM32F407)
- **Build Type**: Debug/Release
- **Generator**: (e.g., Ninja, Make)

## Build Commands
What commands did you run?

```bash
# Your build commands here
mkdir build && cd build
cmake ...
```

## Error Output
Please paste the complete error output:

```
[Paste error output here]
```

## Expected Behavior
What did you expect to happen?

## Additional Files
If relevant, please include:
- [ ] CMakeCache.txt (for CMake issues)
- [ ] Full build log
- [ ] Linker map file (for linker issues)

## Workaround
Have you found any temporary workaround?

## System Information
```bash
# Please run and paste output:
uname -a
which arm-none-eabi-gcc
arm-none-eabi-gcc --print-search-dirs
```

## Checklist
- [ ] I have tried a clean build (`rm -rf build/`)
- [ ] I have verified my toolchain installation
- [ ] I have checked that all submodules are updated
- [ ] I can build other ARM projects successfully