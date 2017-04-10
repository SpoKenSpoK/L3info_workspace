%include 'io.asm'

section .bss
sinput: resb 255	;reserve a 255 byte space in memory for the users input string
$i: rest 1
$carre: rest 1

section .text
global _start
_start:
    call main
    mov  eax, 1                              ; 1 est le code de SYS_EXIT
    int 0x80

main:
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
    push 0
    pop  ebx
    mov  [$i], ebx   ; global
e0:
    mov  ebx, [$i]
    push ebx    ; global
    push 10
    pop  ebx     ; depile la seconde operande dans ebx
    pop	 eax		; depile la permière operande dans eax
    cmp	 eax, ebx
    jl   e2
    push 0
    jmp	 e3
e2:
    push 1
e3:
    pop  eax
    cmp	 eax, 00
    jz	 e1
    mov  ebx, [$i]
    push ebx    ; global
    mov  ebx, [$i]
    push ebx    ; global
    pop	 ebx		 ; depile la seconde operande dans ebx
    pop	 eax		 ; depile la permière operande dans eax
    mul	 ebx		 ; effectue l'opération
    push eax		 ; empile le résultat
    pop  ebx
    mov  [$carre], ebx   ; global
    mov  ebx, [$carre]
    push ebx    ; global
    pop	 eax
    call iprintLF           ; écrire + \n"
    mov  ebx, [$i]
    push ebx    ; global
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    add  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    pop  ebx
    mov  [$i], ebx   ; global
    jmp  e0
e1:
    pop  ebp             ; restaure la valeur de ebp
    ret
