---
name: 🔧 Hardware Support Request
about: Request support for a new MCU, board, or peripheral
title: "[HARDWARE] "
labels: ["hardware", "porting", "enhancement"]
assignees: []
---

## Hardware Information
- **MCU/SoC**: (e.g., STM32F407VGT6, LM4F120H5QR)
- **Board Name**: (e.g., STM32F4-Discovery, Tiva LaunchPad)
- **Architecture**: (e.g., ARM Cortex-M4F, ARM Cortex-M3)
- **Memory**: 
  - Flash: ___ KB
  - RAM: ___ KB
- **Clock Speed**: ___ MHz
- **Vendor**: (e.g., STMicroelectronics, Texas Instruments)

## Request Type
- [ ] New MCU support
- [ ] New board support  
- [ ] New peripheral driver
- [ ] Port existing support from another platform

## Current Status
- [ ] I have this hardware available for testing
- [ ] I can help with development/testing
- [ ] I need this for a specific project
- [ ] This is a commonly requested platform

## Technical Details
### Peripherals Needed
- [ ] GPIO
- [ ] UART/USART  
- [ ] SPI
- [ ] I2C
- [ ] Timers
- [ ] ADC
- [ ] DMA
- [ ] USB
- [ ] Ethernet
- [ ] Other: ___________

### Memory Layout
If known, provide memory map information:
```
Flash: 0x08000000 - 0x080FFFFF (1MB)
RAM:   0x20000000 - 0x2001FFFF (128KB)
```

### Clock Configuration
Expected clock sources and speeds:
- Main clock: ___ MHz
- Peripheral clocks: ___ MHz
- Timer resolution needed: ___ μs

## Reference Materials
Please provide links to:
- [ ] Official datasheet
- [ ] Reference manual
- [ ] Development board documentation
- [ ] Existing code examples (if any)
- [ ] Vendor SDK/HAL

**Links:**
- Datasheet: 
- Reference Manual:
- Board Info:

## Similar Hardware
Is this similar to any currently supported hardware?
- Similar to: (e.g., "Similar to LM3S6965 but with USB")
- Differences: 

## Implementation Priority
- [ ] High - Needed for current project
- [ ] Medium - Would be very useful
- [ ] Low - Nice to have for future

## Development Commitment
- [ ] I can provide hardware for testing
- [ ] I can help write/test the port
- [ ] I can provide funding/sponsorship
- [ ] I can only test once implemented

## Additional Context
Any other information that might help with implementation:
- Specific use cases
- Performance requirements  
- Size/power constraints
- Compatibility requirements