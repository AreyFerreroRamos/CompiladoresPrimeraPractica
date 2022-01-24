#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "utils.h"

extern int yylineno;

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

void yyerror(char *explanation)
{
    fprintf(stderr, "Error: %s, in line %d\n", explanation, yylineno);
    exit(EXIT_FAILURE);
}

// FUNCIONES DE UTILIDAD

void debug(char *text, char *var, int typeFile)
{
    // flex
    if (typeFile == 0)
    {
        //printf(text, var);
    }
        // bison
    else
    {
        // printf(text, var);
    }
}

void simpleDebug(char *text, int typeFile)
{
    // flex
    if (typeFile == 0)
    {
        // printf(text);
    }
        // bison
    else
    {
        // printf(text);
    }
}

char *itos(int num)
{
    char *string = (char *) malloc(sizeof(char) * STR_MAX_LENGTH);
    sprintf(string, "%i", num);
    return string;
}

char *ftos(float num)
{
    char *string = (char *) malloc(sizeof(char) * STR_MAX_LENGTH);
    sprintf(string, "%f", num);
    return string;
}

char *allocateSpaceForMessage()
{
    char *message = (char *) malloc(sizeof(char) * STR_MAX_LENGTH);
    return message;
}

int calculateSizeType(char *type)
{
    if (isSameType(type, FLOAT64_T))
    {
        return 8;
    }
    else if (isSameType(type, INT32_T))
    {
        return 4;
    }
    else
    {
        return 1;
    }
}

char *generateString(char *message, int nArgs, ...)
{
    va_list ap;
    va_start(ap, nArgs);
    char **params = malloc(sizeof(char *) * nArgs);
    for (int i = 0; i < nArgs; i++)
    {
        params[i] = va_arg(ap, char *);
    }
    va_end(ap);

    char *string = allocateSpaceForMessage();
    switch (nArgs)
    {
        case 0:
            sprintf(string, message);
            break;
        case 1:
            sprintf(string, message, params[0]);
            break;
        case 2:
            sprintf(string, message, params[0], params[1]);
            break;
        case 3:
            sprintf(string, message, params[0], params[1], params[2]);
            break;
        case 4:
            sprintf(string, message, params[0], params[1], params[2], params[3]);
            break;
        case 5:
            sprintf(string, message, params[0], params[1], params[2], params[3], params[4]);
            break;
        case 6:
            sprintf(string, message, params[0], params[1], params[2], params[3], params[4], params[5]);
            break;
        case 7:
            sprintf(string, message, params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
            break;
        default:
            yyerror("Estas usando mal la función generateString(). nArgs debe estar entre 0 y 7.");
            break;
    }
    return strdup(string);
}

// FUNCIONES DE CONTROL DE ERRORES

int isSameType(char *type1, char *type2)
{
    return (strcmp(type1, type2) == 0);
}

int isNumberType(char *type)
{
    return (strcmp(type, INT32_T) == 0 || strcmp(type, FLOAT64_T) == 0);
}


