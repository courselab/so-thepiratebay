/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 Rodrigo Valeretto <rodrigoaugustovaleretto@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  Rodrigo Valeretto <rodrigoaugustovaleretto@gmail.com>
 */

/* This is the linker script used to build user programs. */

OUTPUT_FORMAT(binary)		/* Output flat binary (no structure). */
SECTIONS
{
	PRG_LOAD_ADDR = 0xFE00;	/* Program load address.              */

        . = PRG_LOAD_ADDR;

        .bin :
	{
          *.o   (.text .data .bss .rodata) /* Command line objects. */
	  libtpdos.o (.text .data .bss .rodata) /* Runtime library. */
	}
}
INPUT(libtpdos.a)		/* Link with the TpDOS user library. */




