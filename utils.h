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

/**
 * Dado los campos necesarios para gestionar un tensor durante su definición como variable del programa,
 * se genera una estructura que contiene dichos campos y se devuelve.
 */
tensor_ini_info createTensorIniInfo(int dim, char *type, value_info *elements, int num_elem);

/**
 * Dados los campos necesarios para crear una entrada en la tabla de símbolos,
 * se genera una estructura (sym_value_type) que contiene dichos campos y se devuele.
 */
sym_value_type createSymValueType(char *type,char *value, int size, int numDim, int *elemDims, void *elements);

/**
 * Dado un texto con "%s donde quiera poner un argumento, el numero de argumentos, y los argumentos
 * devuelve un string con los argumentos dentro del mensaje.
 */
char *generateString(char *message, int nArgs, ...);

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

/**
 * La función busca en la symtab la variable y la devuelve. Si se produce algún error
 * se lanzará un yyerror().
 */
sym_value_type getEntry(char* key);

/**
 * La función añade o actualiza (si ya existía) una entrada en la symtab. Si se produce algún error
 * se lanzará un yyerror().
 */
void addOrUpdateEntry(char* key, sym_value_type entry);

#endif
