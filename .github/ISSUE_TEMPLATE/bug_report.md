---
name: 🐛 Bug Report
about: Report a bug or unexpected behavior in MicrOS
title: "[BUG] "
labels: ["bug", "needs-triage"]
assignees: []
---

## Bug Description
A clear and concise description of what the bug is.

## Steps to Reproduce
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

## Expected Behavior
A clear and concise description of what you expected to happen.

## Actual Behavior
A clear and concise description of what actually happened.

## Environment
- **Board/MCU**: (e.g., STM32F407, LM3S6965EVB)
- **MicrOS Version**: (e.g., v0.1.0, commit hash)
- **Toolchain**: (e.g., arm-none-eabi-gcc 12.2.1)
- **Build Type**: (e.g., Debug, Release)
- **Host OS**: (e.g., Ubuntu 22.04, Windows 11, macOS 13)

## Code/Configuration
If applicable, add relevant code snippets or configuration:

```c
// Your code here
```

## Debug Information
- [ ] I have enabled debug logging
- [ ] I have attached GDB backtrace (if applicable)
- [ ] I have checked memory usage reports

### GDB Backtrace (if crash/hardfault)
```
(paste GDB backtrace here)
```

### Memory Usage
```
(paste arm-none-eabi-size output here)
```

## Additional Context
Add any other context about the problem here, including:
- Screenshots (if applicable)
- Console output
- Memory dumps
- Register states

## Checklist
- [ ] I have searched for existing issues
- [ ] I have provided all requested information
- [ ] I can reproduce this issue consistently
- [ ] I have tested with the latest version