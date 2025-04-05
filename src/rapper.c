/**************************************************************************
 *   rapper.c  --  This file is part of rapper.                           *
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

#include "rapper.h"

static int vflag = 0;
static int hflag = 0;
static char* nvalue = NULL;
static char* ovalue = NULL;
static long MAXLINELEN = DEFAULT_MAX_LINE_LEN;

static char* filepath = NULL;

static void handle_opt(int, char**);

int main(int argc, char** argv) {
  handle_opt(argc, argv);
  if (hflag == 1)
    return wrap_dumbhyph(filepath, ovalue, MAXLINELEN, vflag);
  else
    return wrap_simple(filepath, ovalue, MAXLINELEN, vflag);
}

static void handle_opt(int argc, char** argv) {
  if (argc == 1) {
    usage(1);
    exit(EXIT_FAILURE);
  }

  int c;
  // int opterr = 0;

  while ((c = getopt(argc, argv, "hvn:o:")) != -1) {
    switch (c) {
    case 'h':
      hflag = 1;
      break;
    case 'v':
      vflag = 1;
      break;
    case 'n':
      nvalue = optarg;
      break;
    case 'o':
      ovalue = optarg;
      break;
    case '?':
      if (optopt == 'n' || optopt == 'o')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      exit(EXIT_FAILURE);
    default:
      abort();
    }
  }

  if (nvalue != NULL) {
    MAXLINELEN = strtol(nvalue, NULL, 10);
  }

  if (optind >= argc) {
    puts("ERROR: no input file.");
    usage(0);
    exit(EXIT_FAILURE);
  }
  filepath = argv[optind];

  if (vflag == 1) {
    puts("Verbose mode is on.");
    if (nvalue == NULL) {
      printf("All lines are wrapped at the default %d columns.\n",
             DEFAULT_MAX_LINE_LEN);
    } else {
      printf("All lines are wrapped at %li columns.\n", MAXLINELEN);
    }
    printf("Input file: %s\n", filepath);
    if (ovalue == NULL) {
      puts("Output will be written to STDIN.");
    } else {
      printf("Output will be saved to %s.\n", ovalue);
    }
    printf("%s mode is on.", hflag == 1 ? "Dumb hyphenation" : "Simple");
    puts("");
    puts("===== START =====");
  }
}
