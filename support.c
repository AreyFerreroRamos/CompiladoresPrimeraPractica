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