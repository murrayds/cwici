/**
 * lexer.c
 *
 * Author: Dakota Murray
 * Ver   : 11 October, 2014
 *
 * Implements all logic for the Wic lexer.
 */
#include "lexer.h"

/**
 * function initLexer
 *
 * initializes the lexer struct which contains information for the lexeme,
 * the current index into the lexeme, the file pointer, and the line number
 * of the file.
 *
 * param fileName - The name of the file to be parsed
 */
void initLexer(char *fileName) {
  FILE *fp;
  fp = fopen(fileName, "r");

  lex.fp = fp;
  lex.lineNumber = 0;
  lex.index = 0;
  holdNewline = -1;
}

/**
 * function getLexeme
 *
 * returns the lexeme buffer containing the token
 *
 * return - the lexeme
 */
char* getLexeme() {
  char* toReturn;
  toReturn = (char*) malloc(MAX_FIELD + 1);
  lex.lexeme[lex.index] = 0;
  strncpy(toReturn, lex.lexeme, MAX_FIELD);
  toReturn[lex.index] = 0;
  return toReturn;
}

/**
 * function getLineNumber
 *
 * returns the current line number the file pointer is at in the file
 *
 * return - returns the current line number in the file
 */
int getLineNumber() {
  return lex.lineNumber;
}

/**
 * function getToken
 *
 * Parses for a token in the wic file. Continues until it either:
 *  a) finds a valid token
 *  b) reaches the end of file
 *  c) parses an unidentifiable token
 *
 * return - returns the parsed token
 */
int getToken()
{
  //printf("Inside getToken\n");
  lex.index = 0;
  int state = 0;

  if (holdNewline == 0) {
    holdNewline = -1;
    return EOL_TOK;
  }

  int c;
  int tok;
  while ( (c = fgetc(lex.fp)) != EOF) {
    //printf("char: %c\tstate= %d\tline: %d\n", c, state, lex.lineNumber);
    switch (state) {
      //START CASE
      case 0:
        if (c == '%') { //comment
          state = 1;
        } else if (c == '\n') { //newline
          state = 5;
          lex.lineNumber++;
          return EOL_TOK;
        } else if(c == '\t' || c == ' ') {  //start of instruction
          state = 2;
        } else if (isalpha(c)) {  //word token
          state = 4;
          tok = WORD_TOK;
          addToLexeme(c);
        } else if (isdigit(c) || c == '-') {  //numer token
          state = 3;
          tok = NUM_TOK;
          addToLexeme(c);
        } else {
          return UNK_TOK;
        }
        break;
      //COMMENTS
      case 1:
        if (c == '\n') {
          state = 5;
          lex.lineNumber++;
          return EOL_TOK;
        } else {
          state = 1;
        }
        break;

      //Start of Instruction
      case 2:
        if (c == '\t' || c == ' ') { //stay in same state
          state = 2;
        } else if (c == '%') {
          state = 1;
        } else if (isalpha(c)) {  //word token
          tok = WORD_TOK;
          addToLexeme(c);
          state = 4;
        } else if (isdigit(c) || c == '-') {  //num token
          tok = NUM_TOK;
          addToLexeme(c);
          state = 3;
        } else if (c == '\n') {
          state = 5;
          lex.lineNumber++;
          return EOL_TOK;
        } else {
          return UNK_TOK;
        }
        break;

      //Number Token
      case 3:
        if (isdigit(c)) {  //Part of the number
          state = 3;
          addToLexeme(c);
          tok = NUM_TOK;
        } else if (c == '\t' || c == ' ') { //end of token
          state = 6;
          return tok;
        } else if (c == '\n') {  //End of line
          state = 7;
          lex.lineNumber++;
          holdNewline = 0;
          return tok;
        } else {
          return UNK_TOK;
        }
        break;

      //word token
      case 4:
        if (isalpha(c) || isdigit(c)) { //Part of the word
          state = 4;
          addToLexeme(c);
          tok = WORD_TOK;
        } else if ( c == '\t' || c == ' ') { //tab, end of token
          state = 6;
          return tok;
        }
        else if (c == '\n') { //End of line
          state = 7;
          lex.lineNumber++;
          holdNewline = 0;
          return tok;
        } else {
          return UNK_TOK;
        }
        break;

      case 5:
        return EOL_TOK;
        break;

      case 6:
        return tok;
        break;

      case 7:
        return tok;
        break;
    } // END SWITCH

  } // END WHILE

  if ( c == EOF) {
    return EOP_TOK;
  }
  return UNK_TOK;

}

/**
 * function addToLexeme
 *
 * Adds a character to the lexeme
 *
 * param c - the character to add to the lexeme
 */
void addToLexeme(char c) {
  if ( lex.index >= MAX_FIELD) {
    printf("Token too large for lexeme: terminating interpreter\n");
    exit(1);
  } else {
    //printf("adding character %c to lexeme\n", c);
    lex.lexeme[lex.index] = c;
    lex.index++;
  }
}
