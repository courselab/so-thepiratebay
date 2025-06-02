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

#include <stdio.h>

void uitoa(unsigned int value, char* str) {
  char buffer[10];
  int i = 0;

  if (value == 0) {
    str[i++] = '0';
  }
  else {
    while (value > 0) {
      buffer[i++] = '0' + (value % 10);
      value /= 10;
    }
  }

  // Inverter a string
  int j;
  for (j = 0; j < i; j++) {
    str[j] = buffer[i - j - 1];
  }

  str[j] = '\0';
}


int main(void)
{
  unsigned short mem_size = get_conv_mem_size();

  char mem_str[10];
  uitoa(mem_size, mem_str);

  printf(mem_str);

  return 0;
}





