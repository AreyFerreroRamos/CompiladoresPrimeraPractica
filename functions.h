#ifndef FUNCTIONS_H

#define FUNCTIONS_H

#include "tipus.h"
#include "utils.h"
#include "support.h"

// FUNCIONES DE UTILIDAD

/**
 * Almacena un nuevo resultado a ser impreso al final de la compilación
 */
void writeResult(char *result);

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

void printVector(int numElem,char *type,void *elems);

/**
 * Dado un tensor, el nombre de la variable i si se quiere imprimir en el archivo de salida o en consola
 * imprime los valores almacenados dentro del tensor de forma continua (como si fuese un vector de una dimensión).
 */
char *printTensor(char *nameVar, sym_value_type tensor);

/**
 * Dado un sym_value_type imprime su valor.
 */
void printSymValueType(sym_value_type entry);

/**
 * Borra todas las variables temporales que se hubiesen creado hasta el momento de ejecutar la función
 * y reinicia el numero del ID temporal
 */
void clearTmpTensorId();

/**
 * Dada una lista de parametros, el numero de parametros que hay y un nuevo parametro a
 * añadir reserva el espacio necesario e introduce el nuevo elemento.
 */
value_info *addValueInfoBase(value_info *list, int numElem, value_info toAdd);

/**
 * Dado un tensor, una posicion, dos operadores y un operando, se encarga de realizar la operación
 * e introducir el valor en la posición indicada del tensor.
 */
void asignacionTensor(sym_value_type *result, int posicion, value_info v1, value_info v2, char *op);

// FUNCIONES PARA REALIZAR OPERACIONES

/**
 * Dados dos operadores y un operando realiza la operacion y la almacena
 * en finish_val.
 */
void doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val);

/**
 * Dados dos numeros y un operando relacional
 * devuelve el resultado de la expresión "num1 op num2".
 */
char *doRelationalOperation(value_info v1, char *op, value_info v2);

/**
 * Dados dos nombres de tensores y una operación (suma o resta) realiza la operación y la almacena en un entrada de la symtab
 * con el nombre "tmp_for_tesor_result". Si algo va mal se ejecutara la función yyerror().
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
void doNumberProductTensor(char *number, char *type, char *nameTensor, sym_value_type *tmp);

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
void doTensorProductTensor(char *nameVar1, char *nameVar2, sym_value_type *tmp);

/**
 * Dado el nombre de una función y los parámetros se encarga de ejecutar la función y devolver el resultado
 */
value_info classifyFunction(char *nameFunc, elements_list params);

#endif
