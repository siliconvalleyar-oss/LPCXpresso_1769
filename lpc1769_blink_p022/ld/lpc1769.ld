MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM   (rwx): ORIGIN = 0x10000000, LENGTH = 32K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .isr_vector : { KEEP(*(.isr_vector)) } > FLASH
    .text : { *(.text*) *(.rodata*) } > FLASH
    .data : AT(ADDR(.text) + SIZEOF(.text)) { _sdata = .; *(.data*) _edata = .; } > RAM
    _sidata = LOADADDR(.data);
    .bss : { _sbss = .; *(.bss*) *(COMMON) _ebss = .; } > RAM
}
