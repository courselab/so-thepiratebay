/*
 *    SPDX-FileCopyrightText: 2025 Rodrigo Valeretto <rodrigoaugustovaleretto@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  Rodrigo Valeretto <rodrigoaugustovaleretto@gmail.com>
 */

#ifndef E8_H
#define E8_H

void __attribute__((fastcall, naked)) printf(const char*);
unsigned short get_conv_mem_size(void);

#endif	
