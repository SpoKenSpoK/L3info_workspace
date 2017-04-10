%include 'io.asm'

section .bss
sinput: resb 255	;reserve a 255 byte space in memory for the users input string
$tab: rest 10

section .text
global _start
_start:
    call main
    mov  eax, 1                              ; 1 est le code de SYS_EXIT
    int 0x80

initialiser:
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
    push 0
    push 8
    pop  ebx
    mov  [$tab], ebx   ; global
    push 1
    push 6
    pop  ebx
    mov  [$tab], ebx   ; global
    push 2
    push 9
    pop  ebx
    mov  [$tab], ebx   ; global
    push 3
    push 9
    pop  ebx
    mov  [$tab], ebx   ; global
    push 4
    push 4
    pop  ebx
    mov  [$tab], ebx   ; global
    push 5
    push 2
    pop  ebx
    mov  [$tab], ebx   ; global
    push 6
    push 3
    pop  ebx
    mov  [$tab], ebx   ; global
    push 7
    push 1
    pop  ebx
    mov  [$tab], ebx   ; global
    push 8
    push 4
    pop  ebx
    mov  [$tab], ebx   ; global
    push 9
    push 5
    pop  ebx
    mov  [$tab], ebx   ; global
    pop  ebp             ; restaure la valeur de ebp
    ret

afficher:
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
    sub  esp, 4   ; allocation variable locale
    push 0
    pop  ebx
    mov  [ebp - 4], ebx  ; local
e0:
    mov  ebx, [ebp - 4] ; lit variable dans ebx
    push ebx    ; local
    mov  ebx, [ebp + 8] ; lit variable dans ebx
    push ebx    ; parametre
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
    mov  ebx, [ebp - 4] ; lit variable dans ebx
    push ebx    ; local
    mov  ebx, [$tab]
    push ebx    ; global
    pop	 eax
    call iprintLF           ; écrire + \n"
    mov  ebx, [ebp - 4] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    add  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    pop  ebx
    mov  [ebp - 4], ebx  ; local
    jmp  e0
e1:
    push 0
    pop	 eax
    call iprintLF           ; écrire + \n"
    add  esp, 4   ; desallocation variable locale
    pop  ebp             ; restaure la valeur de ebp
    ret

echanger:
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
    sub  esp, 4   ; allocation variable locale
    mov  ebx, [ebp + 8] ; lit variable dans ebx
    push ebx    ; parametre
    mov  ebx, [$tab]
    push ebx    ; global
    pop  ebx
    mov  [ebp - 4], ebx  ; local
    mov  ebx, [ebp + 8] ; lit variable dans ebx
    push ebx    ; parametre
    mov  ebx, [ebp + 12] ; lit variable dans ebx
    push ebx    ; parametre
    mov  ebx, [$tab]
    push ebx    ; global
    pop  ebx
    mov  [$tab], ebx   ; global
    mov  ebx, [ebp + 12] ; lit variable dans ebx
    push ebx    ; parametre
    mov  ebx, [ebp - 4] ; lit variable dans ebx
    push ebx    ; local
    pop  ebx
    mov  [$tab], ebx   ; global
    add  esp, 4   ; desallocation variable locale
    pop  ebp             ; restaure la valeur de ebp
    ret

trier:
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
    sub  esp, 12   ; allocation variable locale
    mov  ebx, [ebp + 8] ; lit variable dans ebx
    push ebx    ; parametre
    pop  ebx
    mov  [ebp - 12], ebx  ; local
    push 1
    pop  ebx
    mov  [ebp - 4], ebx  ; local
e4:
    mov  ebx, [ebp - 4] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx     ; depile la seconde operande dans ebx
    pop	 eax		; depile la permière operande dans eax
    cmp	 eax, ebx
    je   e6
    push 0
    jmp	 e7
e6:
    push 1
e7:
    pop  eax
    cmp	 eax, 00
    jz	 e4
    push 0
    pop  ebx
    mov  [ebp - 4], ebx  ; local
    push 0
    pop  ebx
    mov  [ebp - 8], ebx  ; local
e8:
    mov  ebx, [ebp - 8] ; lit variable dans ebx
    push ebx    ; local
    mov  ebx, [ebp - 12] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    sub  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    pop  ebx     ; depile la seconde operande dans ebx
    pop	 eax		; depile la permière operande dans eax
    cmp	 eax, ebx
    jl   e10
    push 0
    jmp	 e11
e10:
    push 1
e11:
    pop  eax
    cmp	 eax, 00
    jz	 e9
    mov  ebx, [ebp - 8] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    add  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    mov  ebx, [$tab]
    push ebx    ; global
    mov  ebx, [ebp - 8] ; lit variable dans ebx
    push ebx    ; local
    mov  ebx, [$tab]
    push ebx    ; global
    pop  ebx     ; depile la seconde operande dans ebx
    pop	 eax		; depile la permière operande dans eax
    cmp	 eax, ebx
    jl   e14
    push 0
    jmp	 e15
e14:
    push 1
e15:
    pop  eax
    cmp	 eax, 00
    jz	 e13
    sub esp, 4  ; allocation valeur de retour
    mov  ebx, [ebp - 8] ; lit variable dans ebx
    push ebx    ; local
    mov  ebx, [ebp - 8] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    add  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    call echanger
    add  esp, 8  ;desallocation parametre
    push 1
    pop  ebx
    mov  [ebp - 4], ebx  ; local
    jmp  e13
e13:
    mov  ebx, [ebp - 8] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    add  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    pop  ebx
    mov  [ebp - 8], ebx  ; local
    jmp  e8
e9:
    mov  ebx, [ebp - 12] ; lit variable dans ebx
    push ebx    ; local
    push 1
    pop  ebx		 ; depile la seconde operande dans ebx
    pop  eax		 ; depile la permière operande dans eax
    sub  eax, ebx	 ; effectue l'opération
    push eax		 ; empile le résultat
    pop  ebx
    mov  [ebp - 12], ebx  ; local
    jmp  e4
e5:
    add  esp, 12   ; desallocation variable locale
    pop  ebp             ; restaure la valeur de ebp
    ret

main:
    push ebp            ; sauvegarde la valeur de ebp
    mov  ebp, esp            ; nouvelle valeur de ebp
    sub esp, 4  ; allocation valeur de retour
    call initialiser
    add  esp, 0  ;desallocation parametre
    sub esp, 4  ; allocation valeur de retour
    push 10
    call afficher
    add  esp, 4  ;desallocation parametre
    sub esp, 4  ; allocation valeur de retour
    push 10
    call trier
    add  esp, 4  ;desallocation parametre
    sub esp, 4  ; allocation valeur de retour
    push 10
    call afficher
    add  esp, 4  ;desallocation parametre
    pop  ebp             ; restaure la valeur de ebp
    ret
