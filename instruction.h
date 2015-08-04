#ifndef INSTRUCTION_H
#define INSTRUCTION_H

/* An alternate C design: don't expose internal data structure.
 * Previously (stack and table), I showed the data structure and
 * prototyped the methods.
 * Here, we keep the structure hidden inside the .c file and only expose
 * the methods.  (You might be thinking "hidden?" how is it hidden since
 * you can just look at it.  But consider that I could deliver the header
 * and an object file or library where you couldn't see it.)
 */

#define MAX_PROGRAM_SIZE 250  // Smallish WIC programs please
#define EOP -1                // Program Counter value indicating to HALT
                              //  (returned by halt method)

#define OPERAND_SIZE 20
#define OP_SIZE 100

#include "table.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
  char opcode[OP_SIZE + 1];
  char operand[OP_SIZE + 1];
} opEntry;

typedef struct {
  opEntry table[MAX_PROGRAM_SIZE];
  int numberOfItems;
} instructionTable;

instructionTable iTable;
tableType jumpTable;
tableType symbolTable;
stackType stack;

/* Prepares instruction structures for execution. */
void initialize();

/* Called from within parser as instructions are parsed/encountered.
 *  All of these parameters are "in" parameters.
 * As instructions are inserted, we must also do any other duties:
 * + "label instructions" must add to the "jump table"
 * + "label instructions" are "backwards" with operand (label) first
 *     and opcode (the word "label" itself) second
 */
void insertInstruction(int address, char *opcode, char *operand);

/* Instructions are fetched during "execution" of WIC program.
 * address is an "in" parameter,
 * opcode and operand are "out" parameters
 * operand may be NULL for opcode-only instructions like "add"
 */
void fetchInstruction(int address, char *opcode, char *operand);

/* Using a lexer/parser, we don't need the helper hasOperand function,
 * but if you wrote it, it is okay.  The book's way of reading input file
 * needed it because it needed to know if next thing read (next token) was
 * operand or next instruction.  Our grammar imparts this structure/knowledge.
 *
 * int hasOperand(char *opcode);
 */

/* Usual debugging method... */
void printInstructions();

/* Lastly, we need prototypes for methods that will perform/execute the
 * WIC instructions.  As a first test, we'll work for the simplest WIC
 * program (a single halt instruction).  As you increase your testing by
 * adding additional instructions, you'll need to add to this list of
 * prototypes.
 *
 * Note that there are some name conflicts with pre-defined functions.  For
 * example, "div" is in a library somewhere.  So try a div prototype, see the
 * error notice, then rename your method (maybe something like "divide" or
 * "wic_div" or whatever.
 */
int halt(int pc);
int get(int pc, char *operand);
int put(int pc, char *operand);
int push(int pc, char *operand);
int pop(int pc, char *operand);
int add(int pc);
int sub(int pc);
int divide(int pc);
int mul(int pc);
int tstlt(int pc);
int tstgt(int pc);
int tstne(int pc);
int tstle(int pc);
int tstge(int pc);
int tsteq(int pc);
int j(int pc, char *operand);
int jf(int pc, char *operand);
int label(int pc);


#endif
