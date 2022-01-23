#ifndef SUPORT_H

#define SUPORT_H

#include "tipus.h"
#include "utils.h"

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();

// FUNCIONES DE UTILIDAD

/**
 * Comprueba si las dimensiones de los tensores son compatibles para poder hacer el prodcto.
 * Si algo va mal devolverá alguno de los mensajes de error.
 * 0 -> EJECUCIÓN CORRECTA, TENSORES MULTIPLICABLES
 * -1 -> ERROR, ESTOS TENSORES NO SE PUEDEN MULTIPLICAR
 * -2 -> ERROR, NO ESTA IMPLEMENTADO EL PRODUCTO DE TENSORES DE MÁS DE 2 DIMENSIONES
 */
int isPossibleTensorProduct(int *elemDims1, int numDims1, int *elemDims2, int numDims2);

/**
 * Dados dos numeros devuelve 1 si el primero es más grande, 0 si son iguales o -1 si es más pequeño.
 */
int maxNum(float a, float b);

/**
 * Dado un vector de enteros que representa las dimensiones de un tensor, devuelve el número
 * de elemntos que hay dentro de él.
 */
int getAcumElemDim(int *elem_dim, int num_dim);

// FUNCIONES PARA REALIZAR OPERACIONES

/**
 * Dados dos enteros y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2" dentro de res. Además si la operación no es válida devuelve 0, o 1 en
 * caso de que si sea correcta.
 */
int intOperations(int num1, int num2, char *op, int *res);

/**
 * Dados dos floats y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2 "dentro de res. Además si la operación no es válida devuelve 0, o 1 en
 * caso de que si sea correcta.
 */
int floatOperations(float num1, float num2, char *op, float *res);

#endif
