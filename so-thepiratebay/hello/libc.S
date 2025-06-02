#    SPDX-FileCopyrightText: 2025 Rodrigo Valeretto <rodrigoaugustovaleretto@gmail.com>
#   
#    SPDX-License-Identifier: GPL-3.0-or-later
#
#  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
#  and includes modifications made by the following author(s):
#  Rodrigo Valeretto <rodrigoaugustovaleretto@gmail.com>

	/* Suppose we receive the argument in %cx. */

	.code16gcc
	.global printf
        .globl get_conv_mem_size

	.section .text
	
printf:	

	mov   %cx, %bx
	
        mov   $0x0e,%ah               
        mov   $0x0, %si               
loop:                                 
        mov   (%bx, %si), %al           
        cmp   $0x0, %al               
        je    end                    
        int   $0x10                   
        add   $0x1, %si               
        jmp   loop                    
end:
        mov %si, %ax
        ret

get_conv_mem_size:
        int $0x12        # Chama a BIOS para obter o tamanho da memória
        ret              # Retorna em %ax (valor em KB)