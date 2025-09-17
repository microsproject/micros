# Driver Model

MicrOS drivers are initialized using **initcalls** and accessed through a **binding API**.

## Concepts
- **Device structure**: name, API vtable, init function
- **Initcall levels**: early → core → post-kernel
- **Binding**: `device_get_binding("uart0")`

## Example
```c
DEVICE_INIT(uart0, "uart0", uart_nrf_init, PRE_KERNEL_1,
            CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
            &uart_nrf_api);
````

Application usage:

```c
const struct device *uart = device_get_binding("uart0");
uart_poll_out(uart, 'A');
```

## Current driver classes

* GPIO
* UART
* I2C
* SPI

See [samples](../samples) for usage.