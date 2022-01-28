#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "support.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;

char **list_tmp_variables_symtab;
int num_tmp_variable;

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

int init_analisi_lexic(char *file_name)
{
    int error;
    yyin = fopen(file_name, "r");
    if (yyin == NULL)
    {
        error = EXIT_FAILURE;
    }
    else
    {
        error = EXIT_SUCCESS;
    }
    return error;
}

int init_analisi_sintactic(char *filename)
{
    int error = EXIT_SUCCESS;
    yyout = fopen(filename, "w");
    if (yyout == NULL)
    {
        error = EXIT_FAILURE;
    }
    return error;
}

int analisi_semantic()
{
    int error;
    if (yyparse() == 0)
    {
        error = EXIT_SUCCESS;
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
}

int end_analisi_lexic()
{
    int error;
    error = fclose(yyin);
    if (error == 0)
    {
        error = EXIT_SUCCESS;
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
}

int end_analisi_sintactic()
{
    int error;
    error = fclose(yyout);
    if (error == 0)
    {
        error = EXIT_SUCCESS;
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
}

// FUNCIONES DE UTILIDAD

void isPossibleTensorProduct(int *elemDims1, int numDims1, int *elemDims2, int numDims2)
{
    if (numDims1 <= 2 && numDims2 <= 2)
    {	// Si son matrices o vectores.
        int nColsMatrix1, nRowsMatrix2;
        if (numDims1 == 1)
        {
            nColsMatrix1 = elemDims1[0];
        }
        else
        {
            nColsMatrix1 = elemDims1[1];
        }
        nRowsMatrix2 = elemDims2[0];
        if (nColsMatrix1 != nRowsMatrix2)
        {
            yyerror("Los indices de los tensores no son compatibles y no se puede realizar el producto.");
        }

    }else{
        yyerror("No esta permitido multiplicar tensores de dimensión superior a 2.");
    }
}

int maxNum(float a, float b)
{
    if (a > b)
    {
        return 1;
    }
    else if (a == b)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int getAcumElemDim(int *elem_dim, int num_dim)
{
    int acum = 1;
    for (int i = 0; i < num_dim; i++)
    {
        acum *= elem_dim[i];
    }
    return acum;
}

char *generateTmpTensorId()
{
    char *id;
    if (list_tmp_variables_symtab == NULL)
    {
        list_tmp_variables_symtab = malloc(sizeof(char *));
    }
    else
    {
        list_tmp_variables_symtab = realloc(list_tmp_variables_symtab, (num_tmp_variable + 1) * sizeof(char *));
    }
    id = (char *) malloc(sizeof(char) * TMP_ID_MAX_LENGTH);
    sprintf(id, "%s%i", TMP_BASE_ID, num_tmp_variable);
    list_tmp_variables_symtab[num_tmp_variable] = id;
    num_tmp_variable++;
    return id;
}

// FUNCIONES PARA REALIZAR OPERACIONES

void intOperations(int num1, int num2, char *operand, int *res)
{
    if (strcmp(operand, OP_ARIT_SUMA) == 0)
    {
        simpleDebug("Estoy en suma\n", 1);
        *res = num1 + num2;
    }
    else if (strcmp(operand, OP_ARIT_RESTA) == 0)
    {
        simpleDebug("Estoy en resta\n", 1);
        *res = num1 - num2;
    }
    else if (strcmp(operand, OP_ARIT_MULT) == 0)
    {
        simpleDebug("Estoy en producto\n", 1);
        *res = num1 * num2;
    }
    else if (strcmp(operand, OP_ARIT_DIV) == 0)
    {
        simpleDebug("Estoy en division\n", 1);
        if (num2 != 0)
        {
            *res = num1 / num2;
        }
        else
        {
            yyerror("No se puede hacer la división si el divisor es 0");
        }
    }
    else if (strcmp(operand, OP_ARIT_MOD) == 0)
    {
        simpleDebug("Estoy en modulo\n", 1);
        if (num2 != 0)
        {
            *res = num1 % num2;
        }
        else
        {
            yyerror("No se puede hacer el módulo si el divisor es 0");
        }
    }
    else if (strcmp(operand, OP_ARIT_POTENCIA) == 0)
    {
        simpleDebug("Estoy en la potencia\n", 1);
        *res = (int) pow((double) num1, (double) num2);
    }
}

void floatOperations(float num1, float num2, char *operand, float *res)
{
    if (strcmp(operand, OP_ARIT_SUMA) == 0)
    {
        *res = num1 + num2;
    }
    else if (strcmp(operand, OP_ARIT_RESTA) == 0)
    {
        *res = num1 - num2;
    }
    else if (strcmp(operand, OP_ARIT_MULT) == 0)
    {
        *res = num1 * num2;
    }
    else if (strcmp(operand, OP_ARIT_DIV) == 0)
    {
        if (num2 != 0)
        {
            *res = num1 / num2;
        }
        else
        {
            yyerror("No se puede hacer la división si el divisor es 0");
        }
    }
    else if (strcmp(operand, OP_ARIT_POTENCIA) == 0)
    {
        *res = (float) pow((double) num1, (double) num2);
    }
}

int intRelationalOperation(int num1, char *op, int num2)
{
    if (isSameType(op, OP_REL_HIGH))
    {
        return num1 > num2;
    }
    if (isSameType(op, OP_REL_HE))
    {
        return num1 >= num2;
    }
    if (isSameType(op, OP_REL_LESS))
    {
        return num1 < num2;
    }
    if (isSameType(op, OP_REL_LE))
    {
        return num1 <= num2;
    }
    if (isSameType(op, OP_REL_EQUAL))
    {
        return num1 == num2;
    }
    if (isSameType(op, OP_REL_DIFF))
    {
        return num1 != num2;
    }
}

int floatRelationalOperation(float num1, char *op, float num2)
{
    if (isSameType(op, OP_REL_HIGH))
    {
        return num1 > num2;
    }
    if (isSameType(op, OP_REL_HE))
    {
        return num1 >= num2;
    }
    if (isSameType(op, OP_REL_LESS))
    {
        return num1 < num2;
    }
    if (isSameType(op, OP_REL_LE))
    {
        return num1 <= num2;
    }
    if (isSameType(op, OP_REL_EQUAL))
    {
        return num1 == num2;
    }
    if (isSameType(op, OP_REL_DIFF))
    {
        return num1 != num2;
    }
}

void controlParamsDiv(elements_list params)
{
    if (params.numElem == 2)
    {
        if (isSameType(params.elements[0].type, INT32_T) && isSameType(params.elements[1].type, INT32_T))
        {
            if (params.elements[0].value == NULL && params.elements[1].value == NULL)
            {
                yyerror("No se admiten tensores como parámetros de la función div()");
            }
        }
        else
        {
            yyerror("La función div() sólo puede dividir números enteros");
        }
    }
    else
    {
        yyerror("El número de parámetros de la función div() es 2");
    }
}

void controlParamsLength(elements_list params)
{
    if (params.numElem == 1)
    {
        if ( !isSameType(params.elements[0].type, STRING_T) && params.elements[0].value != NULL)
        {
            yyerror("La función length sólo puede calcular la longitud de una cadena o el número de elementos de un tensor.");
        }
    }
    else
    {
        yyerror("El número de parámetros de la función length() es 1");
    }
}

void controlParamsSize(elements_list params)
{
    if (params.numElem == 1)
    {
        if (params.elements[0].value != NULL)
        {
            yyerror("La función size sólo puede retornar las dimensiones de un tensor.");
        }
    }
    else
    {
        yyerror("El número de parámetros de la función size() es 1");
    }
}


void controlParamsZerosOnes(char *nameFunc, elements_list params)
{
    if (params.numElem < 2)
    {
        if (isSameType(params.elements[0].type, INT32_T) || isSameType(params.elements[0].type, FLOAT64_T))
        {
            for (int i = 1; i < params.numElem; i++)
            {
                if (isSameType(params.elements[i].type, INT32_T) && params.elements[i].value != NULL)
                {
                    if (atoi(params.elements[i].value) <= 0) {
                        yyerror("Las dimensiones del tensor han de ser números mayores que 0");
                    }
                }
                else
                {
                    yyerror("Los dimensiones del tensor tienen que ser números enteros");
                }
            }
        }
        else
        {
            yyerror("El tipo de los elementos del tensor tiene que ser entero o real");
        }
    }
    else
    {
        yyerror(generateString("El número de parámetros de la función %s() ha de ser igual o superior a 2", 1, nameFunc));
    }
}

void controlParamsTranspose(elements_list params)
{
    if (params.numElem == 1)
    {
        if (params.elements[0].value == NULL)
        {
            sym_value_type entry = getEntry(params.elements[0].lexema);
            if (entry.num_dim != 2)
            {
                yyerror("Sólo se pueden transponer matrices");
            }
        }
        else
        {
            yyerror("Sólo se pueden transponer matrices");
        }
    }
    else
    {
        yyerror("El número de parámetros de la función transpose() es 1");
    }
}

value_info calculateFunctionLength(value_info element)
{
    if (isSameType(element.type, STRING_T))
    {
        return createValueInfo(itos(strlen(element.value)),INT32_T,NULL);
    }
    else
    {
        sym_value_type entry = getEntry(element.lexema);
        return createValueInfo(itos(entry.size / calculateSizeType(entry.type)),INT32_T,NULL);
    }
}

value_info calculateFunctionSize(value_info element)
{
    sym_value_type entry = getEntry(element.lexema);
    int *elems = malloc(entry.num_dim*4);
    for(int i = 0; i < entry.num_dim; i++)
    {
        elems[i] = entry.elem_dims[i];
    }
    sym_value_type newEntry = createSymValueType(INT32_T,NULL, entry.num_dim * 4, 1, &entry.num_dim,elems);
    char *tmp = generateTmpTensorId();
    addOrUpdateEntry(tmp,newEntry);
    return createValueInfo(NULL,INT32_T,tmp);
}

value_info calculateFunctionZerosOnes(elements_list params, char *value)
{
    int *elem_dims = malloc((params.numElem - 1) * 4);
    for (int i = 1; i < params.numElem; i++)
    {
        elem_dims[i] = atoi(params.elements[i].value);
    }
    int numElem = getAcumElemDim(elem_dims, params.numElem - 1);
    void *elements = malloc(numElem * atoi(params.elements[0].value));
    for (int i = 0; i < numElem; i++)
    {
        if (isSameType(params.elements[0].value, INT32_T))
        {
            ((int *) elements)[i] = atoi(value);
        }
        else
        {
            ((float *) elements)[i] = atof(value);
        }
    }
    sym_value_type entry = createSymValueType(params.elements[0].value, NULL, numElem * atoi(params.elements[0].value), params.numElem - 1, elem_dims, elements);
    char *tmp = generateTmpTensorId();
    addOrUpdateEntry(tmp, entry);
    return createValueInfo(NULL, entry.type, tmp);
}

value_info calculateFunctionTranspose(value_info matriz)
{
    sym_value_type entry = getEntry(matriz.lexema);
    sym_value_type newEntry = createSymValueType(entry.type, NULL, entry.size, entry.num_dim, entry.elem_dims, entry.elements);
    newEntry.elem_dims = malloc(2 * sizeof(int));
    newEntry.elem_dims[0] = entry.elem_dims[1];
    newEntry.elem_dims[1] = entry.elem_dims[0];
    newEntry.elements = malloc(entry.size);
    for (int i = 0; i < entry.elem_dims[0]; i++)
    {
        for (int j = 0; j < entry.elem_dims[1]; j++)
        {
            if (isSameType(entry.type, INT32_T))
            {
                ((int *) newEntry.elements)[j * newEntry.elem_dims[1] + i] = ((int *) entry.elements)[i * entry.elem_dims[1] + j];
            }
            else if (isSameType(entry.type, FLOAT64_T))
            {
                ((float *) newEntry.elements)[i * entry.elem_dims[1] + j] = ((float *) entry.elements)[j * entry.elem_dims[1] + i];
            }
        }
    }
    char *tmp = generateTmpTensorId();
    addOrUpdateEntry(tmp,newEntry);
    return createValueInfo(NULL,newEntry.type,tmp);
}