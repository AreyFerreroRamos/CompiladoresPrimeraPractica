#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"

extern FILE *yyout;

extern char **list_tmp_variables_symtab;
extern int num_tmp_variable;
int inFunction = 0;
extern int lengthResults;
extern char **results;

/* FUNCIONES DE UTILIDAD */

void writeResult(char *result)
{
    if (results == NULL)
    {
        results = malloc(sizeof(char *));
    }
    else
    {
        results = realloc(results, (lengthResults + 1) * sizeof(char *));
    }
    results[lengthResults++] = strdup(result);
}

char *removeQuotationMarks(char *string1)
{
    char *string2 = allocateSpaceForMessage(strlen(string1) - 1);
    strlcpy(string2, &string1[1], strlen(string1) - 1);
    return string2;
}

char *negateBoolean(char *boolean)
{
	if (isSameType(boolean, TRUE_VALUE))
	{
		return FALSE_VALUE;
	}
	return TRUE_VALUE;
}

char *getIdName(char *idWithAssign)
{
	int sentinel, i;
	for (i = 0; i < strlen(idWithAssign); i++)
	{
		if (idWithAssign[i] == ' ' || idWithAssign[i] == '\t' || idWithAssign[i] == '=')
		{
			sentinel = i;
			break;
		}
	}
	char *var = allocateSpaceForMessage(STR_MAX_LENGTH);
	strncpy(var, idWithAssign, sentinel);
	return var;
}

int getDim(char *key, int index_dim)
{
	sym_value_type entry;
	int error = sym_lookup(key, &entry);
	if (error == SYMTAB_OK)
	{
		if (index_dim < entry.num_dim)
		{
			return entry.elem_dims[index_dim];
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

char *getNewType(char *type1, char *type2)
{
    if (isSameType(type1, INT32_T) && isSameType(type2, INT32_T))
    {
        return INT32_T;
    }
    else
    {
        return FLOAT64_T;
    }
}

void invertVector(int *vector, int dim)
{
	int aux;
	int i;
	for (i = 0; i < dim / 2; i++)
	{	/* Invertir el vector. */
		aux = vector[i];
		vector[i] = vector[dim - i - 1];
		vector[dim - i - 1] = aux;
	}
}

void *initializeTensorElements(char *value, char *type)
{
    void *list;
	if (isSameType(type, INT32_T))
	{
        list = malloc(calculateSizeType(type));
		((int *) list)[0] = atoi(value);
	}
	else if (isSameType(type, FLOAT64_T))
	{
        list = malloc(calculateSizeType(type));
		((float *) list)[0] = atof(value);
	}
    return list;
}

void *joinTensorElements(void *elems1, char *type1, int nElem1, void *elems2, char *type2, int nElem2)
{
	char *typefinal = getNewType(type1, type2);
    void *aux = realloc(elems1, calculateSizeType(typefinal) * (nElem1 + nElem2));
	if (isSameType(typefinal, INT32_T))
	{	/* Si la lista final se trata como entera. */
		int i = nElem1;
		for (int j = 0; j < nElem2; j++)
		{
			((int *) aux)[i++] = ((int *) elems2)[j];
		}
	}
	else
	{	/* Si la lista final se trata como float. */
		int i = nElem1;
		if (isSameType(type1, INT32_T))
		{
			for (int j = 0; j < i; j++)
			{
				((float *) aux)[j] = ((int *) aux)[j];
			}
		}
		if (isSameType(type2, INT32_T))
		{	/* Si los valores se tratan como enteros. */
			for (int j = 0; j < nElem2; j++)
			{
				((float *) aux)[i++] = ((int *) elems2)[j];
			}
		}
		else if (isSameType(type2, FLOAT64_T))
		{	/* Si los valores se tratan como float. */
			for (int j = 0; j < nElem2; j++)
			{
				((float *) aux)[i++] = ((float *) elems2)[j];
			}
		}
	}
    return aux;
}

void printVector(int numElem, char *type, void *elems){
    printf("[");
    for (int i = 0; i < numElem; i++)
    {
        if (isSameType(type, INT32_T))
        {
            printf("%i,", ((int *) elems)[i]);
        }
        else if (isSameType(type, FLOAT64_T))
        {
            printf("%f,", ((float *) elems)[i]);
        }
    }
    printf("]\n");
}

char *printTensor(char *nameVar, sym_value_type tensor)
{
    char *res = printTensorRec(tensor.elements,tensor.elem_dims,tensor.type,tensor.num_dim,0,0);
    return generateString("%s = %s",2, nameVar, res);
}

void printSymValueType(sym_value_type entry)
{
	printf("------------------------\n");
	printf("TYPE: %s\n", (char *) entry.type);
	printf("VALUE: %s\n", (char *) entry.value);
	printf("SIZE: %i\n", entry.size);
	printf("NUM DIM: %i\n", entry.num_dim);
	if (entry.num_dim > 0)
	{
		printf("ELEM DIMS: ");
		for (int i = 0; i < entry.num_dim; i++)
		{
			printf("%i ", entry.elem_dims[i]);
		}
		printf("\n");
	}
	printf("------------------------\n\n");
}

void clearTmpTensorId()
{
	for (int i = 0; i < num_tmp_variable; i++)
	{
		int response = sym_remove(list_tmp_variables_symtab[i]);
		if (response != SYMTAB_OK)
		{
			yyerror("ALGO MALO HA PASADO BORRANDO LAS VARIABLES");
		}
	}
	free(list_tmp_variables_symtab);
	list_tmp_variables_symtab = NULL;
	num_tmp_variable = 0;
}

value_info *addValueInfoBase(value_info *list, int numElem, value_info toAdd)
{
    value_info *aux;
    if (numElem == 0)
    {
        aux = malloc(sizeof(value_info));
    }
    else
    {
        aux = realloc(list, sizeof(value_info) * (numElem + 1));
    }
    aux[numElem] = toAdd;
    return aux;
}


void asignacionTensor(sym_value_type *result, int posicion, value_info v1, value_info v2, char *op)
{
    sym_value_type entry1, entry2;
    if (v1.lexema != NULL)
    {
        sym_lookup(v1.lexema, &entry1);
        int aux = atoi(v1.value);
        v1.type = entry1.type;
        if (isSameType(entry1.type, INT32_T))
        {
            v1.value = itos(((int *) entry1.elements)[aux]);
        }
        else
        {
            v1.value = ftos(((float *) entry1.elements)[aux]);
        }
    }
    if (v2.lexema != NULL)
    {
        sym_lookup(v2.lexema, &entry2);
        int aux = atoi(v2.value);
        v2.type = entry2.type;
        if (isSameType(entry2.type, INT32_T))
        {
            v2.value = itos(((int *) entry2.elements)[aux]);
        }
        else
        {
            v2.value = ftos(((float *) entry2.elements)[aux]);
        }
    }
    value_info finishVal;
    doAritmeticOperation(v1, op, v2, &finishVal);
    if (isSameType(result->type, INT32_T))
    {
        if (isSameType(finishVal.type, INT32_T))
        {
            ((int *) result->elements)[posicion] = atoi(finishVal.value);
        }
        else
        {
            ((int *) result->elements)[posicion] = (int) atof(finishVal.value);
        }
    }
    else
    {
        if (isSameType(finishVal.type, INT32_T))
        {
            ((float *) result->elements)[posicion] = atoi(finishVal.value);
        }
        else
        {
            ((float *) result->elements)[posicion] = atof(finishVal.value);
        }
    }
}

/* FUNCIONES PARA REALIZAR OPERACIONES */

value_info modifySign(value_info v, char *operand)
{
    if (isSameType(operand, "-"))
    {
        if (isSameType(v.type, INT32_T))
        {
            v.value = itos(0 - atoi(v.value));
        }
        else
        {
            v.value = ftos(0 - atof(v.value));
        }
    }
    return v;
}

void doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val)
{
	if (strcmp(v1.type, INT32_T) == 0 && strcmp(v2.type, INT32_T) == 0)
	{
		finish_val->type = INT32_T;
		int resInt;
		intOperations(atoi(v1.value), atoi(v2.value), operand, &resInt);
        finish_val->value = itos(resInt);

	}
	else
	{
		if (strcmp(operand, "%") != 0)
		{
			finish_val->type = FLOAT64_T;
			float resFloat;
			floatOperations(atof(v1.value), atof(v2.value), operand, &resFloat);
			finish_val->value = ftos(resFloat);
		}
		else
		{
            yyerror("No se puede realizar el m??dulo con operandos reales");
		}
	}
}

char *doRelationalOperation(value_info v1, char *op, value_info v2)
{
    int res;
    if (isSameType(v1.type, INT32_T))
    {
        res = intRelationalOperation(atoi(v1.value), op, atoi(v2.value));
    }
    else
    {
        res = floatRelationalOperation(atof(v1.value), op, atof(v2.value));
    }
    if (res == 1)
    {
        return TRUE_VALUE;
    }
    return FALSE_VALUE;
}

int doTensorCalcs(char *nameVar1, char *nameVar2, char *operation, sym_value_type *tmp)
{
	if (nameVar1 != NULL && nameVar2 != NULL)
	{	/* Si las dos variables son tensores. */
		sym_value_type entry1;
		int response1 = sym_lookup(nameVar1, &entry1);
		sym_value_type entry2;
		int response2 = sym_lookup(nameVar2, &entry2);
		if (response1 == SYMTAB_OK && response2 == SYMTAB_OK)
		{	/* Si las dos variables existen. */
			if (entry1.num_dim == entry2.num_dim)
			{	/* Si los dos tensores tienen el mismo numero de dimensiones. */
				for (int i = 0; i < entry1.num_dim; i++)
				{	/* Si los dos tensores tienen el mismo n??mero de elementos en cada dimension. */
					if (entry1.elem_dims[i] != entry2.elem_dims[i])
					{
						return -5;
					}
				}
				if (isSameType(entry1.type, FLOAT64_T) || isSameType(entry2.type, FLOAT64_T))
				{	/* Si alguno de los dos tipos es float. */
					tmp->type = FLOAT64_T;
				}
				else
				{
					tmp->type = INT32_T;
				}
				tmp->value = NULL;
				if (entry1.size == entry2.size)
				{	/* Si tienen el mismo tama??o (son del mismo tipo). */
					tmp->size = entry1.size;
				}
				else
				{
					if (maxNum((float) entry1.size, (float) entry2.size) == 1)
					{
						tmp->size = entry1.size;
					}
					else
					{
						tmp->size = entry2.size;
					}
				}
				tmp->num_dim = entry1.num_dim;
				tmp->elem_dims = entry1.elem_dims;
				tmp->elements = malloc(tmp->size);
				int cont = tmp->size / calculateSizeType(tmp->type);
				for (int i = 0; i < cont; i++)
				{	/* Por cada elemento hacemos operaci??n. */
					value_info v1;
					v1.lexema = nameVar1;
					v1.type = entry1.type;
					v1.value = itos(i);
					value_info v2;
					v2.lexema = nameVar2;
					v2.type = entry2.type;
					v2.value = itos(i);
					asignacionTensor(&(*tmp), i, v1, v2, operation);
				}
				return 0;
			}
			else
			{
                yyerror("No se pueden sumar o restar tensores con diferentes dimensiones");
			}
		}
		else
		{
            yyerror("Ha habido un error buscando una variable en la symtab");
		}
	}
	else if (nameVar1 == NULL && nameVar2 == NULL)
	{	/* Si las dos variables son numeros. */
		return 1;
	}
	else 
	{
        yyerror("No se puede sumar un tensor con un n??mero");
	}
}

int doTensorProductInit(char *nameVar1, char *nameVar2, sym_value_type *tmp)
{
	if (nameVar1 != NULL && nameVar2 != NULL)
	{	/* Si las dos variables son tensores. */
		sym_value_type entry1;
		sym_lookup(nameVar1, &entry1);
		sym_value_type entry2;
		sym_lookup(nameVar2, &entry2);

		isPossibleTensorProduct(entry1.elem_dims, entry1.num_dim, entry2.elem_dims, entry2.num_dim);
        /* Los tensores se pueden multiplicar. */
        if (isSameType(entry1.type, FLOAT64_T) || isSameType(entry2.type, FLOAT64_T))
        {
            tmp->type = FLOAT64_T;
        }
        else
        {
            tmp->type = INT32_T;
        }
        tmp->value = NULL;
        int rowsM1, colsM2;
        if (entry1.num_dim == 1)
        {
            rowsM1 = 1;
        }
        else
        {
            rowsM1 = entry1.elem_dims[0];
        }
        if (entry2.num_dim == 1)
        {
            colsM2 = 1;
        }
        else
        {
            colsM2 = entry2.elem_dims[1];
        }
        tmp->num_dim = rowsM1 == 1 || colsM2 == 1 ? 1 : 2;
        if (tmp->num_dim == 1)
        {
            tmp->elem_dims = malloc(4);
            if (maxNum((float) rowsM1, (float) colsM2) == 1)
            {
                tmp->elem_dims[0] = rowsM1;
            }
            else
            {
                tmp->elem_dims[0] = colsM2;
            }
        }
        else
        {
            tmp->elem_dims = malloc(8);
            tmp->elem_dims[0] = rowsM1;
            tmp->elem_dims[1] = colsM2;
        }
        tmp->size = getAcumElemDim(tmp->elem_dims, tmp->num_dim) * calculateSizeType(tmp->type);
        tmp->elements = malloc(tmp->size);
        /* MUTLIPLICACION DE DOS TENSORES. */
        return 0;
	}
	else if (nameVar1 == NULL && nameVar2 == NULL)
	{	/* Si las dos variables son n??meros. */
		return -1;
	}
	else
	{	/* Si una es valor i la otra es un tensor. HACER VALOR POR CADA UNO DE LOS VALORES DEL TENSOR. */
		return -2;
	}
}

void doNumberProductTensor(char *number, char *type, char *nameTensor, sym_value_type *tmp)
{
	sym_value_type entry = getEntry(nameTensor);
    *tmp = entry;
    for (int i = 0; i < (entry.size / calculateSizeType(entry.type)); i++) {
        value_info v1;
        v1.type = type;
        v1.value = number;
        v1.lexema = NULL;
        value_info v2;
        v2.type = entry.type;
        v2.value = itos(i);
        v2.lexema = nameTensor;
        asignacionTensor(&(*tmp), i, v1, v2, "*");
    }
}

void doTensorProductTensor(char *nameVar1, char *nameVar2, sym_value_type *tmp)
{
	sym_value_type matrix1 = getEntry(nameVar1);
    sym_value_type matrix2 = getEntry(nameVar2);
    int rowsM1, colsM1, colsM2, rFinal, cFinal;
    if (matrix1.num_dim == 1)
    {
        rowsM1 = 1;
        colsM1 = matrix1.elem_dims[0];
    }
    else
    {
        rowsM1 = matrix1.elem_dims[0];
        colsM1 = matrix1.elem_dims[1];
    }
    if (matrix2.num_dim == 1)
    {
        colsM2 = 1;
    }
    else
    {
        colsM2 = matrix2.elem_dims[1];
    }
    sym_value_type aux;
    aux.elements = malloc(calculateSizeType(tmp->type));
    for (int i = 0; i < rowsM1; i++)
    {
        for (int j = 0; j < colsM2; j++)
        {   /* Si transponemos el vector para poder multiplicarlo hay que trasponer los ??ndices. */
            rFinal = colsM2 == 1 ? j : i;
            cFinal = colsM2 == 1 ? i : j;
            if (isSameType(tmp->type, INT32_T))
            {
                ((int *) aux.elements)[0] = 0;
            }
            else
            {
                ((float *) aux.elements)[0] = 0;
            }
            for (int k = 0; k < colsM1; k++)
            {
                value_info v1;
                v1.type = matrix1.type;
                v1.value = itos(i * colsM1 + k);
                v1.lexema = nameVar1;
                value_info v2;
                v2.type = matrix2.type;
                v2.value = itos(k * colsM2 + j);
                v2.lexema = nameVar2;
                asignacionTensor(tmp, rFinal * colsM2 + cFinal, v1, v2, "*");
                if (isSameType(tmp->type, INT32_T))
                {
                    ((int *) aux.elements)[0] += ((int *) tmp->elements)[rFinal * colsM2 + cFinal];
                }
                else
                {
                    ((float *) aux.elements)[0] += ((float *) tmp->elements)[rFinal * colsM2 + cFinal];
                }
            }
            if (isSameType(tmp->type, INT32_T))
            {
                ((int *) tmp->elements)[rFinal * colsM2 + cFinal] = ((int *) aux.elements)[0];
            }
            else
            {
                ((float *) tmp->elements)[rFinal * colsM2 + cFinal] = ((float *) aux.elements)[0];
            }
        }
    }
}

value_info classifyFunction(char *nameFunc, elements_list params)
{
    if (isSameType(nameFunc, FUNC_DIV))
    {
        controlParamsDiv(params);
        value_info v1;
        doAritmeticOperation(params.elements[0], OP_ARIT_DIV, params.elements[1], &v1);
        return v1;
    }
    else if (isSameType(nameFunc, FUNC_LENGTH))
    {
        controlParamsLength(params);
        return calculateFunctionLength(params.elements[0]);
    }
    else if (isSameType(nameFunc, FUNC_SIZE))
    {
        controlParamsSize(params);
        return calculateFunctionSize(params.elements[0]);
    }
    else if (isSameType(nameFunc, FUNC_ZEROS))
    {
        controlParamsZerosOnes(nameFunc, params);
        return calculateFunctionZerosOnes(params, 0);
    }
    else if (isSameType(nameFunc, FUNC_ONES))
    {
        controlParamsZerosOnes(nameFunc, params);
        return calculateFunctionZerosOnes(params, 1);
    }
    else if (isSameType(nameFunc, FUNC_TRANSPOSE))
    {
        controlParamsTranspose(params);
        return calculateFunctionTranspose(params.elements[0]);
    }
}
