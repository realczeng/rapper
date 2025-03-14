/**************************************************************************
 *   usage.c  --  This file is part of rapper.                            *
 *                                                                        *
 *   Copyright (C) 2025 Chen Zeng                                         *
 *                                                                        *
 *   Rapper is free software: you can redistribute it and/or modify it    *
 *   under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or    *
 *   (at your option) any later version.                                  *
 *                                                                        *
 *   Rapper is distributed in the hope that it will be useful, but        *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU     *
 *   General Public License for more details.                             *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>

void usage(int print_header) {
  if (print_header == 1) {
    puts("rapper version 0.1 - wrapping plain text");
    puts("");
  }
  puts("Usage: rapper [options...] <inputfile>");
  puts("  -n <N>          Wrap text at N columns (defualt: 72)");
  puts("  -o <file>       Write output to file instead of terminal");
  puts("  -v              Print diagnostic information in verbose mode");
  puts("");
}
