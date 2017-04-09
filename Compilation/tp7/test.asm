%include 'io.asm'

section .bss
sinput: resb 255	;reserve a 255 byte space in memory for the users input string

section .text
global _start
_start:
    call main
    mov eax, 1                              ; 1 est le code de SYS_EXIT
    int 0x80                                ; exit

max:
; ---- ENTREE FONCTION ----
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
---- DECLARATION ARGUMENTS ----
    mov  ebx, [ebp + 12] ; lit variable dans ebx
    push ebx
    mov  ebx, [ebp + 8] ; lit variable dans ebx
    push ebx
; ---- eax < ebx ----
    pop  ebx     ; depile la seconde operande dans ebx
    pop	 eax		; depile la permière operande dans eax
    cmp	 eax, ebx
    jl   e0
    push 0
    jmp	 e1
e0:
    push 1
e1:
    pop  eax
    cmp	 eax, 00
    jz	 e-1
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$b]
    push ebx
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$a]
    push ebx
; ---- RETOUR ----
    pop  ebp             ; restaure la valeur de ebp
    ret

main:
; ---- ENTREE FONCTION ----
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
---- DECLARATION ARGUMENTS ----
; ---- LIRE ----
    mov	 ecx, sinput
    mov	 edx, 255
    mov	 eax, 3		 ; 3 est le code de SYS_READ
    mov	 ebx, 0		 ; 0 est le code de STDIN
    int	 80h
    mov	 eax, sinput
    call atoi
    push eax
; ---- AFFECTATION ----
    pop  ebx
    mov  [$v_1], ebx          ; stocke registre dans variable
; ---- LIRE ----
    mov	 ecx, sinput
    mov	 edx, 255
    mov	 eax, 3		 ; 3 est le code de SYS_READ
    mov	 ebx, 0		 ; 0 est le code de STDIN
    int	 80h
    mov	 eax, sinput
    call atoi
    push eax
; ---- AFFECTATION ----
    pop  ebx
    mov  [$v_2], ebx          ; stocke registre dans variable
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$v_1]
    push ebx
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$v_2]
    push ebx
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$v_1]
    push ebx
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$v_1]
    push ebx
; ---- ECRIRE ----
    pop	 eax
    call iprintLF           ; écrire + \n"
; ---- UTILISATION VARIABLE ----
    mov  ebx, [$v_2]
    push ebx
; ---- ECRIRE ----
    pop	 eax
    call iprintLF           ; écrire + \n"
; ---- RETOUR ----
    pop  ebp             ; restaure la valeur de ebp
    ret
