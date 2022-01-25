#ifndef TIPUS_H

#define TIPUS_H

#define STR_MAX_LENGTH 200
#define INT_MAX_LENGTH_STR 10
#define FLOAT_MAX_LENGTH_STR 20

#define INT32_T "Int32"
#define FLOAT64_T "Float64"
#define BOOLEAN_T "Bool"
#define STRING_T "String"

#define IDENT_T "Ident"

#define TRUE_VALUE "true"
#define FALSE_VALUE "false"

#define OP_ARIT_SUMA "+"
#define OP_ARIT_RESTA "-"
#define OP_ARIT_MULT "*"
#define OP_ARIT_DIV "/"
#define OP_ARIT_MOD "%"
#define OP_ARIT_POTENCIA "^"

#define OP_REL_HIGH ">"
#define OP_REL_LESS "<"
#define OP_REL_HE ">="
#define OP_REL_LE "<="
#define OP_REL_EQUAL "=="
#define OP_REL_DIFF "!="

#define TMP_BASE_ID "_tmp_"
#define TMP_ID_MAX_LENGTH 10

typedef struct
{
    char *value;    // Valor del elemento.
	char *type;     // Tipo del elemenento.
	char *lexema;   // Nombre de var en caso de ser ID_ARIT.
} value_info;

typedef struct
{
	int index_dim;  // Posición actual del vector de dimensiones del tensor.
	int calcIndex;  // Variable sobre la que se van acumulando los cálculos parciales para acceder a la posición del tensor demandada.
	char *lexema;   // Nombre de la variable tensor.
} tensor_info;

typedef struct
{
	int dim;        // Dimension concreta que se esta evaluando actaulmente.
	char *type;     // Tipo concreto que se esta evaluando actualmente.
	void *elements; // Valores dentro del componente.
	int num_elem;   // Número de elementos del tensor.
} tensor_ini_info;

#endif
