#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "tipus.h"
#include "symtab.h"

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();
void debug(char *, char *);
void simpleDebug(char *);
int doOperationAritmetic(value_info v1, char *operand, value_info v2, value_info *finish_val);
int isNumberType(char *type);
int intOperations(int num1, int num2, char *operand, int *res);
float floatOperations(float num1, float num2, char *operand, float *res);
char *iota(int num);
char *fota(float num);
char *strncpyWithSentinel(int length, char *toCopy);
value_info createValueInfo(int length, char *value, char *type);
int isSameType(char *type1, char *type2);
int doRelationalOperation(float num1, char *op, float num2);
char *allocateSpaceForMessage();
int negateBoolean(int boolean);
int isTrue(int boolean);
int isFalse(int boolean);
char *getIdName(char *idWithAssign);

#endif
