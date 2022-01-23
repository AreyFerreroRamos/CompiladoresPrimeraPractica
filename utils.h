#ifndef UTILS_H

#define UTILS_H

#include "tipus.h"
#include "symtab.h"

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

void yyerror(char *);

// FUNCIONES DE UTILIDAD

/**
 * Función que hace un printf del texto pasado con una variable como parametro.
 * typeFile identificara si pertenece a flex (0) o bison/funciones (1).
 */
void debug(char *text, char *var, int typeFile);

/**
 * Función que hace un printf del texto pasado.
 * typeFile identificara si pertenece a flex (0) o bison/funciones (1).
 */
void simpleDebug(char *text, int typeFile);

/**
 * Convierte un entero a string.
 */
char *itos(int num);

/**
 * Convierte un float a string.
 */
char *ftos(float num);

/**
 * Reserva espacio en una variable char * para 200 carácteres como máximo y la devuleve.
 */
char *allocateSpaceForMessage();

/**
 * Dado un tipo devuelve el tamaño en bytes.
 */
int calculateSizeType(char *type);

// FUNCIONES DE CONTROL DE ERRORES

/**
 * Dados dos tipos comprueba que son iguales y en ese caso devuelve 1,
 * en caso contrario 0.
 */
int isSameType(char *type1, char *type2);

/**
 * Dado un tipo controla si es entero o float y devuelve 1 en caso de serlo
 * o 0 si no lo es.
 */
int isNumberType(char *type);

#endif
