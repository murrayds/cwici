#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "instruction.h"

int current;
int state;

char buffer[MAX_FIELD + 1];

void parse();
void match(int expectedToken);
void error();

void P();
void IL();
void IL2();
void EOLS();
void I();
void I2();

#endif
