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

/* Built-in shell command: list tyFS files. */
void fs_list()
{
  kwrite("Listing files in TyFS...\n");

  char buffer[512];
  unsigned short segment = ((unsigned int)buffer) >> 4;
  unsigned short offset = ((unsigned int)buffer) & 0xF;

  int sector, i, j;

  // Diretório do TyFS: setores 1 a 5
  for (sector = 1; sector <= 5; sector++)
  {
    // Cálculo de CHS (Cilindro, Cabeça, Setor) para disquete 1.44MB padrão
    unsigned char c = (sector / 36);               // cilindro = setor / (18 setores por trilha * 2 cabeças)
    unsigned char h = (sector / 18) % 2;           // cabeça = (setor / 18) % 2
    unsigned char s = (sector % 18) + 1;           // setor começa em 1

    // Leitura via INT 13h
    __asm__ __volatile__ (
      "push %%es\n\t"
      "mov %0, %%ax\n\t"
      "mov %%ax, %%es\n\t"
      "mov %1, %%bx\n\t"
      "mov $0x02, %%ah\n\t"      // função 02h - ler setores
      "mov $0x01, %%al\n\t"      // número de setores a ler = 1
      "mov %2, %%ch\n\t"         // cilindro
      "mov %3, %%cl\n\t"         // setor (bits 5-0)
      "mov %4, %%dh\n\t"         // cabeça
      "mov $0x00, %%dl\n\t"      // unidade A (floppy = 0)
      "int $0x13\n\t"
      "pop %%es\n\t"
      :
      : "r"(segment), "r"(offset), "r"(c), "r"(s), "r"(h)
      : "ax", "bx", "cx", "dx"
    );

    // Cada entrada de diretório ocupa 32 bytes
    for (i = 0; i < 512; i += 32)
    {
      char first = buffer[i];
      if (first == 0x00 || first == 0xE5)
        continue;

      // Nome
      for (j = 0; j < 8; j++)
      {
        char c = buffer[i + j];
        if (c != ' ')
          kputc(c);
      }

      
      kputc('.');

      // Extensão
      for (j = 8; j < 11; j++)
      {
        char c = buffer[i + j];
        if (c != ' ')
          kputc(c);
      }

      kwrite("\n");
    }
  }
}

