%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>

  #include "tipus.h"
  #include "utils.h"
  #include "functions.h"

  #define YYLMAX 100

  extern FILE *yyout;
  extern int yylex();
  extern void yyerror(char *);

  	/* Variables necesarias para la inicialización de un tensor. */
  int *vector_dims_tensor;	// Vector con el número de elementos de cada dimensión del tensor.
  int num_dims_tensor = 0;	// Número de dimensiones del tensor.
  bool *ampliar_vector_dims; 	// Vector de booleanos para limitar la ampliación de memoria del vector de dimensiones a una sola por dimensión.

  extern int inFunction;
%}

%code requires
{
  #include "tipus.h"
  #include "utils.h"
  #include "functions.h"
}

%union
{
	int enter;
	float real;
	char *cadena;
	value_info operand;
	ident_info ident_info;
	elements_list elements_list;
	tensor_info tensor_info;
	tensor_ini_info tensor_ini_info;
	sym_value_type sym_value_type;
	void *no_definit;
}

%token <no_definit> ASSIGN OP_BOOL_AND OP_BOOL_OR NEGACION COMA PUNTO_Y_COMA PARENTESIS_ABIERTO PARENTESIS_CERRADO CORCHETE_ABIERTO CORCHETE_CERRADO DIV LENGTH SIZE ZEROS ONES TRANSPOSE
%token <enter> INTEGER
%token <real> FLOAT
%token <cadena> STRING OP_ARIT_P1 OP_ARIT_P2 ASTERISCO SUMA RESTA OP_RELACIONAL BOOLEAN
%token <ident_info> ID
%token <operand> ID_ARIT

%type <operand> expresion_aritmetica lista_sumas lista_productos lista_potencias terminal_aritmetico id_arit expresion_booleana lista_or lista_and expresion_booleana_base expresion_relacional terminal_booleano funcion param
%type <tensor_info> id lista_indices lista_indices_arit
%type <tensor_ini_info> tensor componente lista_componentes lista_valores
%type <cadena> op_arit_p3 op_arit_p2 concatenacion id_func
%type <elements_list> lista_params

%start programa


%%


programa : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia | sentencia

sentencia : asignacion
	| expresion_aritmetica 	{
					if ($1.value != NULL)
					{
						writeResult(generateString("La expresion aritmética es %s.", 1, $1.value));
					}
					else
					{
						sym_value_type entry = getEntry($1.lexema);
						writeResult(printTensor($1.lexema, entry));
						clearTmpTensorId();
					}
				}
	| expresion_booleana 	{
					writeResult(generateString("La expresion booleana es %s.", 1, $1.value));
				}
	| ID	{
			sym_value_type entry = getEntry($1.lexema);
			if (entry.num_dim > 0)
			{
				writeResult(printTensor($1.lexema, entry));
			}
			else
			{
				writeResult(generateString("%s actualmente vale %s.", 2, $1.lexema, (char *) entry.value));
			}
		}

asignacion : ID ASSIGN expresion_aritmetica	{
							if ($3.value != NULL)
							{
								sym_value_type entry = createSymValueType($3.type, $3.value, calculateSizeType($3.type), 0, NULL, NULL);
								addOrUpdateEntry($1.lexema, entry);
								writeResult(generateString("%s = %s", 2, $1.lexema, entry.value));
							}
							else
							{	/* Si la expresion aritmética es un tensor. */
								sym_value_type entry = getEntry($3.lexema);
								addOrUpdateEntry($1.lexema, entry);
								writeResult(printTensor($1.lexema, entry));
								clearTmpTensorId();
							}
						}
	| id ASSIGN expresion_aritmetica	{
							if ($3.value != NULL)
							{
								sym_value_type entry = getEntry($1.lexema);
								if (isSameType(entry.type, INT32_T))
								{
									if (isSameType($3.type, INT32_T))
									{
										((int *) entry.elements)[$1.calcIndex] = atoi($3.value);
									}
									else
									{
										((int *) entry.elements)[$1.calcIndex] = (int) atof($3.value);
									}
								}
								else if (isSameType(entry.type, FLOAT64_T))
								{
									if (isSameType($3.type, INT32_T))
									{
										((float *) entry.elements)[$1.calcIndex] = atoi($3.value);
									}
									else
									{
										((float *) entry.elements)[$1.calcIndex] = atof($3.value);
									}
								}
								addOrUpdateEntry($1.lexema, entry);
								writeResult(generateString("%s[%i] = %s", 3, $1.lexema, $1.calcIndex, $3.value));
							}
							else
							{
								yyerror("No se puede asignar un tensor a un indice de un tensor");
							}
						}
	| ID ASSIGN expresion_booleana	{
						sym_value_type entry = createSymValueType($3.type, $3.value, strlen($3.value), 0, NULL, NULL);
						addOrUpdateEntry($1.lexema, entry);
						writeResult(generateString("%s = %s", 2, $1.lexema, (char *) entry.value));
					}
	| ID ASSIGN concatenacion	{
						sym_value_type entry = createSymValueType(STRING_T, $3, strlen($3), 0, NULL, NULL);
						addOrUpdateEntry($1.lexema, entry);
						writeResult(generateString("%s = \"%s\"", 2, $1.lexema, (char *) entry.value));
					}
	| ID ASSIGN tensor	{
					invertVector(vector_dims_tensor, $3.dim);
					sym_value_type entry = createSymValueType($3.type, NULL, calculateSizeType($3.type) * $3.num_elem, $3.dim, vector_dims_tensor, $3.elements);
					addOrUpdateEntry($1.lexema, entry);
					writeResult(printTensor($1.lexema, entry));
					vector_dims_tensor = NULL;
					ampliar_vector_dims = NULL;
					num_dims_tensor = 0;
				}

id : lista_indices CORCHETE_CERRADO	{
						$$ = createTensorInfo($1.index_dim, $1.calcIndex, $1.lexema);
					}
   
lista_indices : lista_indices COMA expresion_aritmetica	{
								if (isSameType($3.type, INT32_T))
								{
									int dim = getDim($1.lexema, $1.index_dim);
									if (dim != -1)
									{
										$$ = createTensorInfo($1.index_dim + 1, $1.calcIndex * dim + atoi($3.value), $1.lexema);
									}
									else
									{
										yyerror("Valor de indice fuera de los limites del tensor");
									}
								}
								else
								{
									yyerror(generateString("El indice %s no es de tipo INT32_T", 1, $3.value));
								}
							}
		| ID CORCHETE_ABIERTO expresion_aritmetica	{
									if (isSameType($3.type, INT32_T))
									{
										$$ = createTensorInfo(1, atoi($3.value), $1.lexema);
									}
									else
									{
										yyerror(generateString("El indice %s no es de tipo INT32_T", 1, $3.value));
									}
								}

concatenacion : concatenacion ASTERISCO STRING 	{
							$$ = generateString("%s%s", 2, $1, $3);
						}
		| STRING 	{
					$$ = strdup($1);
				}

expresion_aritmetica :lista_sumas

lista_sumas : lista_sumas op_arit_p3 lista_productos	{
								if (isNumberType($3.type))
								{	
									sym_value_type tmp;
									if (doTensorCalcs($1.lexema, $3.lexema, $2, &tmp) == 0)
									{
										$$ = saveTmpTensorInSymTab(getNewType($1.type, $3.type), tmp);
									}
									else
									{
										doAritmeticOperation($1, $2, $3, &$$);
									}
								}
								else
								{
									yyerror(generateString("No se puede hacer operaciones aritmeticas con el tipo %s", 1, $3.type));
								}
							}	
		| lista_productos	{
						if (isNumberType($1.type))
						{
							$$ = createValueInfo($1.value, $1.type, $1.lexema);
						}
						else
						{
							yyerror(generateString("No se puede hacer operaciones aritmeticas con el tipo %s", 1, $1.type));
						}
					}

op_arit_p3 : SUMA	{
				$$ = strdup($1);
			}
	| RESTA	{
			$$ = strdup($1);
		}

lista_productos : lista_productos op_arit_p2 lista_potencias 	{
									if (isNumberType($3.type))
									{	/* Incializar response en calculo de enteros y reales. */
										int response = -1;
										sym_value_type tmp;
										if (isSameType($2, OP_ARIT_MULT))
										{
											response = doTensorProductInit($1.lexema, $3.lexema, &tmp);
										}
										if (response == 0)
										{
											doTensorProductTensor($1.lexema, $3.lexema, &tmp);
											$$ = saveTmpTensorInSymTab(getNewType($1.type, $3.type), tmp);
										}
										else if (response == -2)
										{
											if ($1.value != NULL)
											{
												doNumberProductTensor($1.value, $1.type, $3.lexema, &tmp);
											}
											else
											{
												doNumberProductTensor($3.value, $3.type, $1.lexema, &tmp);
											}
											$$ = saveTmpTensorInSymTab(getNewType($1.type, $3.type), tmp);
										}
										else if (response == -1)
										{
											if ($1.lexema == NULL && $3.lexema == NULL)
											{
												doAritmeticOperation($1, $2, $3, &$$);
											}
											else
											{
												yyerror("Los tensores no admiten la división");
											}
										}
									}
									else
									{
										yyerror(generateString("No se puede hacer operaciones aritmeticas con el tipo %s", 1, $3.type));
									}
								}
		| lista_potencias	{
						if (isNumberType($1.type))
						{
							$$ = createValueInfo($1.value, $1.type, $1.lexema);
						}
						else
						{
							yyerror(generateString("No se puede hacer operaciones aritmeticas con el tipo %s", 1, $1.type));
						}
					}

op_arit_p2: OP_ARIT_P2	{
				$$ = strdup($1);
			} 
	| ASTERISCO	{
				$$ = strdup($1);
			}

lista_potencias : lista_potencias OP_ARIT_P1 terminal_aritmetico	{
										if (isNumberType($3.type))
										{
											doAritmeticOperation($1, $2, $3, &$$);
										}
										else
										{
											yyerror(generateString("No se puede hacer operaciones aritmeticas con el tipo %s", 1, $3.type));
										}
									}
		| terminal_aritmetico	{
						if (isNumberType($1.type))
						{
							$$ = createValueInfo($1.value, $1.type, $1.lexema);
						}
						else
						{
							yyerror(generateString("No se puede hacer operaciones aritmeticas con el tipo %s", 1, $1.type));
						}
					}

terminal_aritmetico : INTEGER	{
					$$ = createValueInfo(itos($1), INT32_T, NULL);
				}
		| FLOAT		{
					$$ = createValueInfo(ftos($1), FLOAT64_T, NULL);
				}
		| id_arit 	{
					$$ = createValueInfo($1.value, $1.type, $1.lexema);
				}
		| PARENTESIS_ABIERTO expresion_aritmetica PARENTESIS_CERRADO	{
											$$ = $2;
										}
		| funcion	{
					$$ = $1;
				}

funcion : id_func PARENTESIS_ABIERTO lista_params PARENTESIS_CERRADO	{
										$$ = classifyFunction($1, $3);
										inFunction--;
									}

id_func : DIV	{
			$$ = strdup(FUNC_DIV);
		}
	| LENGTH	{
				$$ = strdup(FUNC_LENGTH);
			}
	| SIZE	{
			$$ = strdup(FUNC_SIZE);
		}
	| ZEROS	{
			$$ = strdup(FUNC_ZEROS);
		}
	| ONES	{
			$$ = strdup(FUNC_ONES);
		}
	| TRANSPOSE	{
				$$ = strdup(FUNC_TRANSPOSE);
			}

lista_params : lista_params COMA param	{
						$$ = $1;
						$$.elements = addValueInfoBase($1.elements, $1.numElem, $3);
						$$.numElem++;
					}
		| param	{
				$$.numElem = 0;
				$$.elements = addValueInfoBase($$.elements, $$.numElem, $1);
				$$.numElem++;
			}

param : concatenacion	{
				$$ = createValueInfo($1, STRING_T, NULL);
			}
	| expresion_aritmetica	{
					$$ = $1;
				}
	| expresion_booleana	{
					$$ = $1;
				}

id_arit : ID_ARIT	{ 
				$$ = $1; 
			}	
	| lista_indices_arit CORCHETE_CERRADO	{
							sym_value_type res = getEntry($1.lexema);
							if (isSameType(res.type, INT32_T))
							{
								$$.value = itos(((int *) res.elements)[$1.calcIndex]);
							}
							else
							{
								$$.value = ftos(((float *) res.elements)[$1.calcIndex]);
							}
							$$.type = res.type;
							$$.lexema = NULL;
						}

lista_indices_arit : lista_indices_arit COMA expresion_aritmetica	{
										if (isSameType($3.type, INT32_T))
										{
											int dim = getDim($1.lexema, $1.index_dim);
											if (dim != -1)
											{
												$$ = createTensorInfo($1.index_dim + 1, $1.calcIndex * dim + atoi($3.value), $1.lexema);
											}
											else
											{
												yyerror("Se han excedido los límites de las dimensiones del tensor");
											}
										}
										else
										{
											char *error = allocateSpaceForMessage();
											sprintf(error, "Index %s is not type Int32", $3.type);
											yyerror(error);
										}
									}
		| ID_ARIT CORCHETE_ABIERTO expresion_aritmetica	{
									if (isSameType($3.type, INT32_T))
									{
										$$ = createTensorInfo(1, atoi($3.value), $1.lexema);
									}
									else
									{
										char *error = allocateSpaceForMessage();
										sprintf(error, "Index %s is not type Int32", $3.type);
										yyerror(error);
									}
								}

expresion_booleana : lista_or	{
					$$ = $1;
				}

lista_or : lista_or OP_BOOL_OR lista_and	{
							if (isSameType($1.value, TRUE_VALUE) || isSameType($3.value, TRUE_VALUE))
							{
								$$ = createValueInfo(TRUE_VALUE, BOOLEAN_T, $1.lexema);
							}
							else
							{
								$$ = createValueInfo(FALSE_VALUE, BOOLEAN_T, $1.lexema);
							}
						}
	| lista_and	{
				$$ = $1;
			}

lista_and : lista_and OP_BOOL_AND expresion_booleana_base	{
									if (isSameType($1.value, FALSE_VALUE) || isSameType($3.value, FALSE_VALUE))
									{
										$$ = createValueInfo(FALSE_VALUE, BOOLEAN_T, $1.lexema);
									}
									else
									{
										$$ = createValueInfo(TRUE_VALUE, BOOLEAN_T, $1.lexema);
									}
								}
	| expresion_booleana_base	{
						$$ = $1;
					}

expresion_booleana_base : NEGACION expresion_relacional	{
                                                                $$ = createValueInfo(negateBoolean($2.value), BOOLEAN_T, $2.lexema);
							}
			| expresion_relacional	{
							$$ = $1;
						}

expresion_relacional : expresion_aritmetica OP_RELACIONAL expresion_aritmetica	{
											if (isSameType($1.type, $3.type))
											{
												$$ = createValueInfo(doRelationalOperation($1, $2, $3), BOOLEAN_T, $1.lexema);
											}
											else
											{
												yyerror(generateString("Cannot do comparation %s %s %s", 3, $1.value, $2, $3.value));
											}
										}
			| terminal_booleano	{
							$$ = $1;
						}

terminal_booleano : BOOLEAN	{
					$$ = createValueInfo($1, BOOLEAN_T, NULL);
				}
		| PARENTESIS_ABIERTO expresion_booleana PARENTESIS_CERRADO	{
											$$ = $2;
										}

tensor : CORCHETE_ABIERTO lista_componentes CORCHETE_CERRADO	{
       									if (ampliar_vector_dims[$2.dim])
									{
										ampliar_vector_dims[$2.dim] = false;
									}
									$2.dim++;
									$$ = $2;
								}

lista_componentes : lista_componentes PUNTO_Y_COMA componente	{	void *elements = joinTensorElements($1.elements, $1.type, $1.num_elem, $3.elements, $3.type, $3.num_elem);
                                                                 	$$ = createTensorIniInfo($1.dim, getNewType($1.type, $3.type), elements, $1.num_elem + $3.num_elem);
									if (ampliar_vector_dims[$1.dim])
									{
										vector_dims_tensor[$1.dim] += 1;
									}
								}
		| componente	{
					$$ = $1;
					if ($1.dim >= num_dims_tensor)
					{
						vector_dims_tensor = realloc(vector_dims_tensor, ++num_dims_tensor * 4);					
						vector_dims_tensor[$1.dim] = 1;
						ampliar_vector_dims = realloc(ampliar_vector_dims, num_dims_tensor);
						ampliar_vector_dims[$1.dim] = true;
					}
				}

componente : lista_valores	{
					$$ = $1;
				}
	| tensor	{
				$$ = $1;
			}

lista_valores : lista_valores COMA expresion_aritmetica	{
								void *elements = joinTensorElements($1.elements, $1.type, $1.num_elem, initializeTensorElements($3.value, $3.type), $3.type, 1);
								$$ = createTensorIniInfo(0, getNewType($1.type, $3.type), elements, $1.num_elem + 1);
								if (ampliar_vector_dims[0])
								{
									vector_dims_tensor[0] += 1;
								}
							}
		| expresion_aritmetica	{
						$$ = createTensorIniInfo(0, $1.type, initializeTensorElements($1.value, $1.type), 1);
						if (ampliar_vector_dims == NULL)
						{
							ampliar_vector_dims = malloc(1);
							ampliar_vector_dims[0] = true;
							vector_dims_tensor = malloc(4);
							vector_dims_tensor[0] = 1;
							num_dims_tensor++;
						}
					}


%%