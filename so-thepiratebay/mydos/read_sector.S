.code16gcc
.section .text
.global bios_read

bios_read:
    pusha
    pushw %bp
    movw %sp, %bp

    movw 4(%bp), %bx        # buffer
    xorw %ax, %ax
    movw %ax, %ds
    movw $0x12, %bx  

    movw 6(%bp), %si        # setor LBA
    movw 8(%bp), %cx        # número de setores

.loop:
    cmpw $0x0000, %cx
    je .done

    # Convert LBA (in SI) to CHS
    movw %si, %ax
    xorw %dx, %dx           # DX:AX = LBA
    movw $18, %bx           # setores por trilha
    divw %bx                # AX / BX → AX = trilha, DX = setor

    movb %dl, %cl           # setor = resto + 1
    incb %cl                # BIOS é 1-based (1–18)

    movb $0x00, %ch         # cilindro = 0
    movb $0x00, %dh         # cabeça = 0
    movb $0x00, %dl         # drive = 0

    pushw %cx
    pushw %si

    movb $0x02, %ah         # função: ler setores
    movb $0x01, %al         # 1 setor por vez
    int $0x13
    jc .error

    popw %si
    popw %cx

    addw $512, %bx
    incw %si
    decw %cx
    jmp .loop

.done:
    popw %bp
    popa
    ret

.error:
    movb $'E', %al
    call kwrite_char
    popw %si
    popw %cx
    jmp .done

