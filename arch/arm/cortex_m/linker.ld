ENTRY(Reset_Handler)

SECTIONS
{
  .text : {
    KEEP(*(.isr_vector))
    *(.text*)

    KEEP(*(.init))
    KEEP(*(.fini))

    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    *(.rodata*)
  } > FLASH

  .data : {
    . = ALIGN(4);
    __data_start__ = .;

    *(vtable)
    *(.data)
    *(.data.*)

    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    . = ALIGN(4);
    __data_end__ = .;
  } > RAM AT > FLASH

    __data_load__ = LOADADDR(.data);

  .bss : {
    . = ALIGN(4);
    __bss_start__ = .;  
    
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    __bss_end__ = .;    
  } > RAM

  .heap : {
    . = ALIGN(8);
    __heap_start__ = .;

    . = . + __HEAP_SIZE;

    . = ALIGN(8);
    __heap_end__ = .;
  } > RAM

  .stack (ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE) : {
    . = ALIGN(8);
    __stack_limit__ = .;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
    __stack_top__ = .;
  } > RAM

  PROVIDE(__stack = __stack_top__);

  ASSERT(__stack_limit__ >= __heap_end__, "Region RAM is too small for stack and heap")
}