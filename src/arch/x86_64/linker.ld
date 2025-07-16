ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 1M;
    .text : ALIGN(4096)
    {
        *(.text)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    /*
    Sticking all the assembly after all C code (above)
    that'll keep the alignment between the two
    */

    .asm : ALIGN(4096)
    {
        *(.asm)
    }
}