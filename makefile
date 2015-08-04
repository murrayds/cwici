
default:
	@echo "usage: make <target>"
	@echo "available targets: stack, table, lexer, parser, cwici"

parser: parser_test.c parser.h parser.c lexer.h lexer.c instruction.h instruction.c table.c table.h
	gcc table.c instruction.c lexer.c parser_test.c parser.c -o testparser

stack: teststack.c stack.h stack.c
	gcc teststack.c stack.c -o teststack

table: testtable.c table.h table.c
	gcc testtable.c table.c -o testtable

lexer: lexer_test.c lexer.h lexer.c parser.c parser.h
	gcc lexer_test.c lexer.c parser.c -o testlexer

cwici: instruction.h instruction.c parser.h parser.c lexer.h lexer.c table.h table.c cwici.c stack.c stack.h
	gcc table.c stack.c instruction.c lexer.c parser.c cwici.c -o cwici
