/**
 * instruction.c
 *
 * Author: Dakota Murray
 * Version: 13 October, 2014
 *
 * A file containing all functions dealing with loading, fetching, and executing
 * WIC instructions. Implements the Symbol Table, Jump Table, and Instruction
 * table.
 */

#include "instruction.h"

/**
 * function initiailize
 *
 * initiailizes structs necessary for computation
 */
void initialize() {
    initTable(&jumpTable);
    initTable(&symbolTable);
    iTable.numberOfItems = 0;
    initStack(&stack);
}

/**
 * function insertInstruction
 *
 * use: insertInstruction(int addredd, char *opcode, char *operand);
 *
 * Inserts an instruction into the instruction table. Each entry of the
 * instruction table contains an operand and an opcode. The special case of
 * the label instruction must be accounted for. The Label instruction has the
 * operand and operation reversed and adds an address to the jump table.
 *
 * param address - the address of the instruction, it really is just a useless
 *                parameter at the moment but I don't feel like changing it
 *
 * param opcode  - A pointer to a buffer containing the opcode string
 * param operand - A pointer to a buffer containing the operand string
 */
void insertInstruction(int address, char *opcode, char *operand) {
  opEntry newEntry;
  if(strcmp(operand, "label") == 0) {
    //opcode & operand are swapped
    store(&jumpTable, opcode, iTable.numberOfItems);
    strncpy(newEntry.opcode, operand, OP_SIZE + 1);
    strncpy(newEntry.operand, opcode, OP_SIZE + 1);
  } else {
    strncpy(newEntry.opcode, opcode, OP_SIZE + 1);
    strncpy(newEntry.operand, operand, OP_SIZE + 1);
  }

  //Store the instruction into the instruction table
  iTable.table[iTable.numberOfItems] = newEntry;
  iTable.numberOfItems++;
}

/**
 * function fetchInstruction
 *
 * use: fetchInstruction(int addredd, char *opcode, char *operand)
 *
 * Fetches an instruction at a certain address from the instruction array.
 * The address is the index into the array.
 *
 * param address - the address of the instruction to fetch
 * param opcode  - a pointer to the buffer to place the opcode string in
 * param operand - a pointer to the buffer to place the operand string in
 */
void fetchInstruction(int address, char *opcode, char *operand) {
    //printf("Instruction Being Fetched: %s\n", iTable.table[address].opcode);
    strncpy(opcode, iTable.table[address].opcode, OP_SIZE);
    strncpy(operand, iTable.table[address].operand, OP_SIZE);
}

/**
 * function printInstructions
 *
 * use: printInstructions();
 *
 * Prints out a lits of all the stored instructions
 */
void printInstructions() {
  int i;
  printf("address\topcode\toperand\n");
  for(i = 0; i < iTable.numberOfItems; i++) {
    opEntry entry = iTable.table[i];
    printf("%d\t%s\t%s\n", i, entry.opcode, entry.operand);
  }
}

/**
 * function halt
 *
 * use: int halt(int pc)
 *
 * Implements the halt instruction from the wic language. Stops program
 * execution
 *
 * param pc  - the program counter
 *
 * returns the new instruction counter, in this case EOP
 */
int halt(int pc) {
  return EOP;
}

/**
 * function get
 *
 * use: int get(int pc, char *operand)
 *
 * Implements the get instruction from the wic langauge. Gets user input and
 * places it into a provided variable. If variable is stored in symbol table
 * already then it is overwritten. Otherwise it is stored.
 *
 * param pc  - the program counter
 * param operand - the vairable to store user input in
 *
 * returns   - the new progra counter (pc + 1)
 */
 int get(int pc, char* operand) {
   char * valbuff;
   valbuff = malloc(OPERAND_SIZE + 1);
   printf("Enter value %s: ", operand);
   fgets(valbuff, OPERAND_SIZE, stdin);
   int val = (int) strtol(valbuff, NULL, 10);

   int errsv = errno;
   errno = 0;
   if (val == 0 && errsv == EINVAL) {
     printf("Error: Invalid Input. Terminating Interpreter\n");
     exit(0);
   }
   store(&symbolTable, operand, val);
   return pc + 1;
 }

 /**
  * function put
  *
  * use: int put(int pc, char *operand);
  *
  * Implements the put instruction from the wic language. Prints out the value
  * of a vairable.
  *
  * param pc   - the program counter
  * param operand - the vairable to print out
  *
  * returns the new program counter (pc + 1)
  */
  int put(int pc, char * operand) {
    int val = retrieve(&symbolTable, operand);
    printf("%d\n", val);
    return pc + 1;
  }


/**
 * function push
 *
 * use: int push(int pc, char *operand)
 *
 * Implements the push instruction from the wic language. Pushes value in the
 * provided variable onto the stack. Value can either be an integer or a
 * variable name.
 *
 * param pc    - the program counter
 * param operand - the variable/integer to push
 *
 * returns the program counter (pc + 1)
 */
 int push(int pc, char *operand) {
   int val;
   if(isdigit(operand[0])) {
     //pushing an integer
     val = (int) strtol(operand, NULL, 10);
     int errsv = errno;
     //Make sure value is actually an integer
     if (val == 0 && errsv == EINVAL) {
       printf("Error: Invalid Push, Must be a variable or integer. Terminating Parse\n");
       exit(0);
     }

   } else {
     //value is a variable. Retrieve from symbol table
     val = retrieve(&symbolTable, operand);
   }
   stackPush(&stack, val);
   return pc + 1;
 }

 /**
  * function pop
  *
  * use: int pop(int pc, char *operand)
  *
  * Implements the pop instruction from the wic langauge. Pops value from top
  * of stack and places it in variable provided by the operand.
  *
  * param pc  - the program counter
  * param operand - the variable to store the popped value in
  *
  * returns the new program counter (pc + 1)
  */
  int pop(int pc, char* operand) {
    int val = stackPop(&stack);
    store(&symbolTable, operand, val);
    return pc + 1;
  }

 /**
  * function mul
  *
  * use: int mul(int pc)
  *
  * Implements the mul wic instruction. Pops the top two values off the stack,
  * multiplies them together, and pushes the result.
  *
  * param pc  - the program counter
  *
  * returns the new program counter, (pc + 1)
  */
  int mul(int pc) {
    int right = stackPop(&stack);
    int left = stackPop(&stack);
    stackPush(&stack, left * right);
    return pc + 1;
  }


 /**
  * function sub
  *
  * use: int sub(int pc)
  *
  * Implements the sub wic instruction. Pops the top two values off the stack.
  * The first value becomes the right operand (B) and the second becomes the
  * left operand (A). Evaluates A - B and pushes value onto the stack.
  *
  * param pc  - the program counter
  *
  * returns the new progra, counter (pc + 1)
  */
  int sub(int pc) {
    int right = stackPop(&stack);
    int left = stackPop(&stack);
    stackPush(&stack, left - right);
    return pc + 1;
  }


 /**
  * function add
  *
  * use: int add(int pc)
  *
  * Implements the add wic instruction. Pops the top two values off the stack,
  * adds them together, and pushes the result onto the stack.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int add(int pc) {
    int right = stackPop(&stack);
    int left = stackPop(&stack);
    stackPush(&stack, left + right);
    return pc + 1;
  }

 /**
  * function divide
  *
  * use: int mul(int pc)
  *
  * Implements the div wic instruction. Pops the top two values off the stack.
  * The first value popped is the right operand (B) and the second value popped
  * is the left operand (A). Evaluates A / B and pushes the result.
  *
  * param pc  - the program counter
  *
  * returns the new program counter, (pc + 1)
  */
  int divide(int pc) {
    int right = stackPop(&stack);
    int left = stackPop(&stack);
    stackPush(&stack, left / right);
    return pc + 1;
  }


 /**
  * function tsteq
  *
  * use: int tsteq(int pc)
  *
  * Implements the tsteq wic instruction. Performs a logical == on the top two
  * operands on the stack and pushes a 0 for false or a 1 for true.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int tsteq(int pc) {
    int var = stackPop(&stack);
    int val;
    if (var == 0) {
      val = 1;
    } else {
      val = 0;
    }
    stackPush(&stack, val);
    return pc + 1;
  }

 /**
  * function tstne
  *
  * use: int tstne(int pc)
  *
  * Implements the tstne wic instruction. Performs a logical != on the top two
  * operands on the stack and pushes a 0 for false or a 1 for true.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int tstne(int pc) {
    int var = stackPop(&stack);
    int val;
    if (var != 0) {
      val = 1;
    } else {
      val = 0;
    }
    stackPush(&stack, val);
    return pc + 1;
  }

 /**
  * function tstlt
  *
  * use: int tstlt(int pc)
  *
  * Implements the tstlt wic instruction. Performs a logical < on the top two
  * operands on the stack and pushes a 0 for false or a 1 for true.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int tstlt(int pc) {
    int var = stackPop(&stack);
    int val;
    if (var < 0) {
      val = 1;
    } else {
      val = 0;
    }
    stackPush(&stack, val);
    return pc + 1;
  }

 /**
  * function tstle
  *
  * use: int tstle(int pc)
  *
  * Implements the tstle wic instruction. Performs a logical <= on the top two
  * operands on the stack and pushes a 0 for false or a 1 for true.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int tstle(int pc) {
    int var = stackPop(&stack);
    int val;
    if (var <= 0) {
      val = 1;
    } else {
      val = 0;
    }
    stackPush(&stack, val);
    return pc + 1;
  }
  
 /**
  * function tstgt
  *
  * use: int tstgt(int pc)
  *
  * Implements the tstgt wic instruction. Performs a logical > on the top two
  * operands on the stack and pushes a 0 for false or a 1 for true.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int tstgt(int pc) {
    int var = stackPop(&stack);
    int val;
    if (var > 0) {
      val = 1;
    } else {
      val = 0;
    }
    stackPush(&stack, val);
    return pc + 1;
  }

 /**
  * function tstge
  *
  * use: int tstge(int pc)
  *
  * Implements the tstgt wic instruction. Performs a logical >= on the top two
  * operands on the stack and pushes a 0 for false or a 1 for true.
  *
  * param pc  - the program counter
  *
  * returns the new program counter (pc + 1)
  */
  int tstge(int pc) {
    int var = stackPop(&stack);
    int val;
    if (var >= 0) {
      val = 1;
    } else {
      val = 0;
    }
    stackPush(&stack, val);
    return pc + 1;
  }

 /**
  * function j
  *
  * use: int j(int pc, char *operand)
  *
  * Implements the j wic instruction. Performs an unconditional jump to the
  * label specified in operand.
  *
  * param pc  - the program counter
  * param operand - the label to jump to
  *
  * returns the address of the specified label
  */
  int j(int pc, char *operand) {
    return retrieve(&jumpTable, operand);
  }


 /**
  * function jf
  *
  * use: int = jf(int pc, char *operand)
  *
  * Implements the jf wic instruction. Performs a "Jump if False" operation.
  * Pops the value from the top of the stack. If that value is 0 (false), then
  * the jump is performed.
  *
  * param pc  - the pc counter
  * param operand - the label to jump to if false
  *
  * returns the address of the next program counter (pc + 1) or label
  */
  int jf(int pc, char *operand) {
    int val = stackPop(&stack);
    if (val == 0) {
      //jump taken
      return retrieve(&jumpTable, operand);
    } else {
      //else jump not taken
      return pc + 1;
    }
  }

 /**
  * function label
  *
  * use: int label(int pc)
  *
  * Implements the wic label system. Designates an address as a label. By the
  * time this function is called the jump table is already created. This is
  * simply an empty instruction which the interpreter passes over
  *
  * param pc  - the program counter
  *
  * returns the next program counter (pc + 1)
  */
  int label(int pc) {
    //fall through
    return pc + 1;
  }
