/*
 * parse_test.c A test program for our parser
 *
 * Notice there are two ways to run it!  With no filename argument
 * it will test a predefined WIC program.  But you can have it parse
 * your own file by giving the filename as an argument.
 * For example:
 * % parse_test
 * % parse_test myfile.wic
 */

#include <stdio.h>

#include "parser.h"
#include "lexer.h"

void createWICfile(char *filename);

int main(int argc, char *argv[]) {
  char *wicFilename;

  if (argc == 1) {
    // no wic file on command line
    wicFilename = "parse_test.wic";
    createWICfile(wicFilename);
  }
  else wicFilename = argv[1];

  initLexer(wicFilename);
  parse();
  printf("Congrats! Looks like that file parsed successfully.\n");
}

/* same WIC file as lexer_test 
*  tests comments (single line, trailing)
*  tests blank lines
*  tests word num
*  tests word word
*  tests word
*  tests different indentations
*/
void createWICfile(char *filename) {
  FILE *file = fopen(filename,"w");

  fprintf(file, "%% leading comment\n");
  fprintf(file, "  %% indented comment\n");
  fprintf(file, "\n");
  fprintf(file, "word 567\n");
  fprintf(file, "%% indent code\n");
  fprintf(file, "   singleword\n");
  fprintf(file, "   lonelyword %% trailing comment\n");
  fprintf(file, "   adam  eve   %% trailing comment\n");
  fprintf(file, "\n");
  fprintf(file, "   \t  columbus 1492\n");

  fclose(file);
}
