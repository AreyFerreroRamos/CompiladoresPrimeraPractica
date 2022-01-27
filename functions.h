#ifndef FUNCTIONS_H

#define FUNCTIONS_H

#include "tipus.h"
#include "utils.h"
#include "support.h"

// FUNCIONES DE UTILIDAD

/**
 * Dada una cadena de carácteres, se retiran las comillas que
 * la delimitan para facilitar la operación de concatenación.
 */
char *removeQuotationMarks(char *string1);

/**
 * Dado un booleano (0/1) devuelve su contrario.
 */
char *negateBoolean(char *boolean);

/**
 * Dados los datos necesarios para crear una entrada en la symtab,
 * genera un objeto que contiene todos ellos y lo devuele.
 */
value_info createValueInfo(char *value, char *type, char *lexema);

/**
 * Dados los parámetros necesarios para crear una estructura de tipo tensor_info (dim,
 * calcIndex y lexema) se crea dicha estructura.
 */
tensor_info createTensorInfo(int dim, int calcIndex, char *lexema);

/**
 * Dado un string que contiene el id, posibles espacios y el símbolo "="
 * devuelve otro string con solo la parte del id.
 */
char *getIdName(char *idWithAssign);

/**
 * Dada el nombre de una variable (de tipo Tensor) y una dimensión (posición en el vector
 * de dimensiones) devuelve cuantos elementos de la siguiente dimensión contiene dicha
 * dimensión (contenido de dicha posición en el vector de dimensiones).
 */
int getDim(char *key, int num_dims);

/**
 * Dados dos tipos númericos, se devuelve el tipo que resulta de operar ambos números.
 */
char *getNewType(char *type1, char *type2);

/**
 * Dado un vector, se invierte el orden de los elementos que lo componen.
 */
void invertVector(int *vector, int dim);

/**
 * Dado un valor en formato caracter y el tipo de este valor lo convierte a
 * un void* interpretado como el tipo.
 */
void *initializeTensorElements(char *value, char *type);

/**
 * Dadas dos listas  y el tipo de cada una las une en una sola lista que tendrá el tipo
 * prioritario (1.Float 2.Int).
 */
void *joinTensorElements(void *elems1, char *type1, int nElem1, void *elems2, char *type2, int nElem2);

void printfTensorBase(int numElem,char *type,void *elems);

/**
 * Dado un tensor, el nombre de la variable i si se quiere imprimir en el archivo de salida o en consola
 * imprime los valores almacenados dentro del tensor de forma continua (como si fuese un vector de una dimensión).
 */
void printTensor(char *nameVar, sym_value_type tensor, int inFile);

/**
 * Dado un sym_value_type imprime su valor.
 */
void printSymValueType(sym_value_type entry);

/**
 *
 */
void saveTmpTensorInSymTab(value_info *val, char *type1, char *type2, sym_value_type entry);

/**
 *
 */
char *generateTmpTensorId();

/**
 *
 */
void clearTmpTensorId();

/**
 *
 */
void asignacionTensor(sym_value_type *result, int posicion, value_info v1, value_info v2, char *op);

// FUNCIONES PARA REALIZAR OPERACIONES

/**
 * Dados dos operadores y un operando realiza la operacion y la almacena
 * en finish_val.
 */
int doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val);

/**
 * Dados dos numeros y un operando relacional
 * devuelve el resultado de la expresión "num1 op num2".
 */
char *doRelationalOperation(value_info v1, char *op, value_info v2);

/**
 * Dado un string comprueba si es una key de la tabla de simbolos.
 * Si es una key devuelve el size guardado en la variable, sino devuelve
 * el strlen() del string pasado como parámetro. En el caso que al buscar
 * en la tabla de símbolos de un error distinto a no encontrado se devolverá -1.
 */
int lenght(char *key);

/**
 * Dados dos nombres de tensores y una operación (suma o resta) realiza la operación y la almacena en un entrada de la symtab
 * con el nombre "tmp_for_tesor_result". Si algo va mal devolverá alguno de los mensajes de error.
 *  0 -> EJECUCIÓN CORRECTA, SE HAN CALCULADO LAS MATRICES Y LA ALMACENA EN SYMTAB
 * -1 -> ERROR, SE INTENTA SUMAR TENSOR CON VALOR
 * -2 -> NO SON TENSORES NINGUNO DE LOS DOS
 * -3 -> ERROR, ALGUN TENSOR NO ESTA DEFINIDO EN LA SYMTAB
 * -4 -> ERROR, LOS TENSORES SON DE DIFERENTE DIMENSION
 */
int doTensorCalcs(char *nameVar1, char *nameVar2, char *operation, sym_value_type *tmp);

/**
 * Dados dos nombres de tensores y una operación (solo se admite producto) realiza la operación y la almacena en un entrada de la symtab
 * con el nombre "tmp_for_tesor_result". Si algo va mal devolverá alguno de los mensajes de error.
 */
int doTensorProductInit(char *nameVar1, char *nameVar2, sym_value_type *tmp);

/**
 * Realiza el producto de un número por un tensor.
 */
int doNumberProductTensor(char *number, char *type, char *nameTensor, sym_value_type *tmp);

/**
 * Realiza el producto de dos tensores.
 * Teniendo en cuenta la dificultad del diseño de la operación,
 * únicamente se pueden multiplicar los tensores de una y de dos dimensiones.
 * En consecuencia, hay cuatro posibilidades:
 * 	- Producto de dos vectores.
 * 	- Producto de un vector por un matriz.
 * 	- Producto de una matriz por un vector.
 * 	- Producto de dos matrices.
 */
int doTensorProductTensor(char *nameVar1, char *nameVar2, sym_value_type *tmp);

#endif
