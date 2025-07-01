/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 keny-azvd <kenner-azevedo@hotmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  keny-azvd <kenner-azevedo@hotmail.com>
 */

/* This source file implements the kernel entry function 'kmain' called
   by the bootloader, and the command-line interpreter. Other kernel functions
   were implemented separately in another source file for legibility. */

#include "kernel.h" /* Essential kernel functions.  */
#include "bios1.h"  /* For kwrite() etc.            */
#include "bios2.h"  /* For kread() etc.             */
#include "kaux.h"   /* Auxiliary kernel functions.  */

/* Kernel's entry function. */
//extern void read_sector(unsigned char c, unsigned char h, unsigned char s,
//                        unsigned short segment, unsigned short offset);


void kmain(void)
{
  int i, j;

  register_syscall_handler(); /* Register syscall handler at int 0x21.*/

  splash(); /* Uncessary spash screen.              */

  shell(); /* Invoke the command-line interpreter. */

  halt(); /* On exit, halt.                       */

}

/* Tiny Shell (command-line interpreter). */

char buffer[BUFF_SIZE];
int go_on = 1;

void shell()
{
  int i;
  clear();
  kwrite("TinyDOS 1.0\n");

  while (go_on)
  {

    /* Read the user input.
       Commands are single-word ASCII tokens with no blanks. */
    do
    {
      kwrite(PROMPT);
      kread(buffer);
    } while (!buffer[0]);

    /* Check for matching built-in commands */

    i = 0;
    while (cmds[i].funct)
    {
      if (!strcmp(buffer, cmds[i].name))
      {
        cmds[i].funct();
        break;
      }
      i++;
    }

    /* If the user input does not match any built-in command name, just
       ignore and read the next command. If we were to execute external
       programs, on the other hand, this is where we would search for a
       corresponding file with a matching name in the storage device,
       load it and transfer it the execution. Left as exercise. */

    if (!cmds[i].funct)
      kwrite("Command not found\n");
  }
}

/* Array with built-in command names and respective function pointers.
   Function prototypes are in kernel.h. */

struct cmd_t cmds[] =
    {
        {"help", f_help}, /* Print a help message.       */
        {"quit", f_quit}, /* Exit TyDOS.                 */
        {"hello", f_hello}, /* Execute an example program. */
        {"list", fs_list},
        {0, 0}};

/* Build-in shell command: help. */

void f_help()
{
  kwrite("...me, Obi-Wan, you're my only hope!\n\n");
  kwrite("   But we can try also some commands:\n");
  kwrite("      hello   (to run a sample user program\n");
  kwrite("      quit    (to exit TyDOS)\n");
}

void f_quit()
{
  kwrite("Program halted. Bye.\r\n");
  go_on = 0;
}

/* Built-in shell command: example. */
extern int main();
void f_hello()
{
  main(); /* Call the user program's 'main' function. */
}

void kputc(char c)
{
  __asm__ __volatile__ (
    "mov $0x0E, %%ah\n\t"   // Função 0Eh - escrever caractere em modo teletipo
    "mov %0, %%al\n\t"      // Caractere a ser impresso
    "int $0x10\n\t"         // Chama BIOS para imprimir
    :
    : "r"(c)
    : "ax"
  );
}



#define SECTOR_SIZE 512
#define START_SECTOR 12
#define ENTRY_SIZE 32
#define NAME_SIZE 20

// Implementada em ASM
extern void bios_read(void *buffer, unsigned short sector_lba, unsigned short count);



void fs_list() {
    kwrite("You are in flist section !\n\n");
    unsigned char buffer[SECTOR_SIZE * 3];
    bios_read(buffer, START_SECTOR, 3);

    for (int i = 0; i < (SECTOR_SIZE * 2) / ENTRY_SIZE; i++) {
        unsigned char *entry = buffer + i * ENTRY_SIZE;

        char dbg[32];
        dbg[0] = 'E'; dbg[1] = ':'; dbg[2] = ' ';
        dbg[3] = "0123456789ABCDEF"[(entry[0] >> 4) & 0xF];
        dbg[4] = "0123456789ABCDEF"[entry[0] & 0xF];
        dbg[5] = '\n';
        dbg[6] = '\0';
        kwrite(dbg);

        if (entry[0] == 0x00)
            break;

        char name[NAME_SIZE + 1];
        for (int j = 0; j < NAME_SIZE; j++) {
            name[j] = entry[j];
        }
        name[NAME_SIZE] = '\0';

        kwrite("Arquivo: ");
        kwrite(name);
        kwrite("\n");
    }
}

