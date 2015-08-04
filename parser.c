/**
 * parser.c
 *
 * Author: Dakota Murray
 * version: 13 October, 2014
 *
 * Implements the "parse" function which will make sure that the program
 * contains the proper syntax.
 */

#include "parser.h"


/**
 * driver method, acts as the first production in our grammar. Kicks the whole
 * parse off and makes sure that it ends properly.
 */
void parse() {
  current = getToken();
  P(current);
  if (current == EOP_TOK) {
    printf("Congragulations! Program Parsed Successfully!\n");
  } else {
    printf("Failure! Program Failed to Parse Correctly\n");
  }
}

/**
 * P -> IL | lambda
 */
void P() {
  if (current != EOP_TOK) {
    IL(current);
  } else {
    //lambda
  }
}

/**
 * IL -> EOLS I eol IL2
 */
void IL() {
  if (current == EOL_TOK || current == WORD_TOK) {
    EOLS();
    I();
    match(EOL_TOK);
    IL2();
  } else {
    error();
  }
}

/**
 * IL2 -> IL | lambda
 */
void IL2() {
  if (current != EOP_TOK) {
    IL();
  } else {
    //lambda
  }
}

/**
 * I -> WORD_TOK I2
 *
 * This function copies the current lexeme into a temporary buffer so that
 * the opcode and operand can both be copied in the case of a WORD_TOK WORK_TOK
 * or a WORK_TOK NUM_TOK case.
 */
void I() {
  if (current == WORD_TOK) {
    strncpy(buffer, getLexeme(), MAX_FIELD);
    match(WORD_TOK);
    I2();
  } else {
    error();
  }
}

/**
 * I2 -> WORK_TOK | NUM_TOK | lambda
 *
 * stores the instruction into the instruction table
 */
void I2() {
  if (current == WORD_TOK) {
    insertInstruction(lex.lineNumber, buffer,  getLexeme());
    match(WORD_TOK);
  } else if (current == NUM_TOK) {
    insertInstruction(lex.lineNumber, buffer,  getLexeme());
    match(NUM_TOK);
  } else {
    insertInstruction(lex.lineNumber, buffer,  "");
    //lambda
  }
}

/**
 * EOLS -> EOL_TOK EOLS | lambda
 */
void EOLS() {
  if (current == EOL_TOK) {
    match(EOL_TOK);
    EOLS();
  } else {
    //lambda
  }
}

/**
 * Simple match function. Ensures that the current token matches what is
 * expected and the gets the next token.
 */
void match(int expected) {
  if ( current == expected) {
    current = getToken();
  } else {
    printf("ERROR: Current Token %d did not match expected token %d\n", current, expected);
    error();
  }
}

/**
 * error function used at multiple points in the parser
 */
void error() {
  printf("FAILURE: Parse Failed at line: %d Token: %d\n", lex.lineNumber, current);
  exit(1);
}
