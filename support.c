#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "support.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;

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
        if (params.elements[0].value == NULL)
        {
            for (int i = 1; i < params.numElem; i++)
            {
                if (isSameType(params.elements[i].type, INT32_T) && params.elements[i].value != NULL)
                {
                    if (atoi(params.elements[i].value) <= 0) {
                        yyerror("La dimensión ha de ser mayor que 0.");
                    }
                }
                else
                {
                    yyerror("Los dimensiones del tensor tienen que ser números enteros.");
                }
            }
        }
        else
        {
            yyerror(generateString("El primer parámetro de la función %s() tiene que ser un tensor", 1, nameFunc));
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
            if (entry.num_dim > 2)
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