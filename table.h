/**
 * table.h
 *
 * @author Dr. Fenwick and class
 * @version 3/4/2014
 */

#ifndef TABLE_H_
#define TABLE_H_

#define TABLE_SIZE 50
#define KEY_SIZE 25

//Define the structs used for the tables.
typedef struct{
  char key[KEY_SIZE+1];
  int value;
}tableEntry;

//table contains entries and the number of elements
typedef struct {
  int numItemsInUse;
  tableEntry entry[TABLE_SIZE];
} tableType;

//Function prototypes.
void store(tableType *Xtable, char *key, int val);
int retrieve(tableType *Xtable, char *key);
void initTable(tableType *Xtable);
void printTable(tableType *Xtable);

#endif /* TABLE_H_ */
