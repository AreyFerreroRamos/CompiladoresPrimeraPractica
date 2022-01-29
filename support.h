#ifndef SUPORT_H

#define SUPORT_H

#include "tipus.h"
#include "utils.h"

/* FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR */

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();

/* FUNCIONES DE UTILIDAD */

/**
 * La función retorna un string con el tensor (con el formato esperado por entrada).
 */
char *printTensorRec(void *elems, int *tensorDims, char *type, int numDims, int dim, int calcIndex);

/**
 * Dado un vector de elementos, la posición del vector y el tipo del vector(Int o Float)
 * devuelve un string con el valor en la posición indicada.
 */
char *getVectorPosition(void *elems, int pos, char *type);

/**
 * Printea tanto por la consola como en un fichero de salida todas los resultados que se hayan
 * ido almacenando durante la compilación.
 */
void printResults();

/**
 * Comprueba si las dimensiones de los tensores son compatibles para poder hacer el prodcto.
 * Si algo va mal devolverá alguno de los mensajes de error.
 */
void isPossibleTensorProduct(int *elemDims1, int numDims1, int *elemDims2, int numDims2);

/**
 * Dados dos numeros devuelve 1 si el primero es más grande, 0 si son iguales o -1 si es más pequeño.
 */
int maxNum(float a, float b);

/**
 * Dado un vector de enteros que representa las dimensiones de un tensor, devuelve el número
 * de elemntos que hay dentro de él.
 */
int getAcumElemDim(int *elem_dim, int num_dim);

/* FUNCIONES PARA REALIZAR OPERACIONES */

/**
 * Dados dos enteros y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2" dentro de res. Además si la operación no es válida ejecutará un yyerror().
 */
void intOperations(int num1, int num2, char *op, int *res);

/**
 * Dados dos floats y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2" dentro de res. Además si la operación no es válida ejecutará un yyerror().
 */
void floatOperations(float num1, float num2, char *op, float *res);

/**
 * Dados dos enteros y un operando relacional, devuelve el resultado de la expresión
 * "num1 op num2".
 */
int intRelationalOperation(int num1, char *op, int num2);

/**
 * Dados dos floats y un operando relacional, devuelve el resultado de la expresión
 * "num1 op num2".
 */
int floatRelationalOperation(float num1, char *op, float num2);

/**
 * Controla que los parámetros de la función div() sean correctos.
 */
void controlParamsDiv(elements_list params);

/**
 * Controla que los parámetros de la función length() sean correctos.
 */
void controlParamsLength(elements_list params);

/**
 * Controla que los parámetros de la función size() sean correctos.
 */
void controlParamsSize(elements_list params);

/**
 * Controla que los parámetros de la función zeros() o ones() sean correctos.
 */
void controlParamsZerosOnes(char *nameFunc, elements_list params);

/**
 * Controla que los parámetros de la función transpose() sean correctos.
 */
void controlParamsTranspose(elements_list params);

/**
 * Realiza la función length() y deuelve el valor en un value_info.
 */
value_info calculateFunctionLength(value_info element);

/**
 * Realiza la función size() y deuelve el valor en un value_info.
 */
value_info calculateFunctionSize(value_info element);

/**
 * Realiza la función zeros() o ones() y devuelve el valor en un value_info.
 */
value_info calculateFunctionZerosOnes(elements_list params, int value);

/**
 * Realiza la función transpose() y deuelve el valor en un value_info.
 */
value_info calculateFunctionTranspose(value_info matriz);

#endif
