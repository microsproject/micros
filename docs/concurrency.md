# Concurrency in MicrOS

MicrOS provides a simple kernel with:
- Threads (cooperative + optional preemptive)
- Mutexes
- Semaphores
- Message queues
- Timers

## Rules
- ISRs must use `_irqsafe` APIs
- Do not block inside an ISR
- Priority 0 = highest, larger numbers = lower priority

## Example
```c
void worker(void *arg) {
    while (1) {
        do_work();
        k_sleep_ms(100);
    }
}