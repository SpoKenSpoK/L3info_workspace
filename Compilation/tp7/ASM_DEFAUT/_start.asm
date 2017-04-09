global _start
_start:
    call main
    mov eax, 1                              ; 1 est le code de SYS_EXIT
    int 0x80                                ; exit
