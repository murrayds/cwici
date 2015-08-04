/*
 * cwici.c A WIC interpreter written in C.
 *
 * This version differs from textbook description in that we use
 * a lexer/parser to handle structured input.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "instruction.h"

void execute();

int main(int argc, char *argv[]) {
  char *wicFilename;

  if (argc == 1) {
    // no wic file on command line
    fprintf(stderr, "usage: %s <wic filename>\n",argv[0]);
    exit(1);
  }
  else wicFilename = argv[1];

  initLexer(wicFilename);
  parse();
  printf("Congrats! Looks like that file parsed successfully.\n");
  printf("Here is the instruction list constructed...\n");
  printInstructions();

  printf("\n\nNow attempting to execute the program...\n\n");
  execute();
}

void execute() {
  int pc = 0;
  char opcode[MAX_FIELD];
  char operand[MAX_FIELD];

  while (pc != EOP) { // halt instruction sets program counter to EOP
    fetchInstruction(pc, opcode, operand);

    if (strcmp(opcode,"halt")==0) {
      pc = halt(pc);
    } else if (strcmp(opcode, "get") == 0) {
      pc = get(pc, operand);
    } else if (strcmp(opcode, "put") == 0) {
      pc = put(pc, operand);
    } else if (strcmp(opcode, "push") == 0) {
      pc = push(pc, operand);
    } else if (strcmp(opcode, "pop") == 0) {
      pc = pop(pc, operand);
    } else if (strcmp(opcode, "add") == 0) {
      pc = add(pc);
    } else if (strcmp(opcode, "sub") == 0) {
      pc = sub(pc);
    } else if (strcmp(opcode, "mul") == 0) {
      pc = mul(pc);
    } else if (strcmp(opcode, "div") == 0) {
      pc = divide(pc);
    } else if (strcmp(opcode, "tsteq") == 0) {
      pc = tsteq(pc);
    } else if (strcmp(opcode, "tstne") == 0) {
      pc = tstne(pc);
    } else if (strcmp(opcode, "tstlt") == 0) {
      pc = tstlt(pc);
    } else if (strcmp(opcode, "tstle") == 0) {
      pc = tstle(pc);
    } else if (strcmp(opcode, "tstgt") == 0) {
      pc = tstgt(pc);
    } else if (strcmp(opcode, "tstge") == 0) {
      pc = tstge(pc);
    } else if (strcmp(opcode, "j") == 0) {
      pc = j(pc, operand);
    } else if (strcmp(opcode, "jf") == 0) {
      pc = jf(pc, operand);
    } else if (strcmp(opcode, "label") == 0) {
      pc = label(pc);
    }
    else {
      printf("unknown instruction %s %s\n",opcode, operand);
      exit(1);
    }
  }
}
