/***************** ESPECIFICACIO LEX ***************/

/*
 
*/

/***************************************************/

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>

  #include "bison.tab.h"
  #include "tipus.h"
  #include "utils.h"
  #include "functions.h"

  extern void yyerror(char *);
  bool assignFounded = false;
  bool corcheteOpenFounded = false;
  extern int inFunction;
%}

%option yylineno
%option noyywrap

lletra	[a-zA-Z]
digit	[0-9]
id      {lletra}({lletra}|{digit}|_({lletra}|{digit}))*


%%


\n  {
		simpleDebug("\n\n", 0);
		assignFounded = false;
		corcheteOpenFounded = false;
    }


({digit})+	{
                simpleDebug("INTEGER ", 0);
                yylval.enter = atoi(yytext);
                return INTEGER;
            }

({digit})+"."({digit})+(e(-)?({digit})+)?	{
                                                simpleDebug("FLOAT ", 0);
                                                yylval.real = atof(yytext);
                                                return FLOAT;
                                            }

(true|false)	{
                    simpleDebug("BOOLEAN ", 0);
                    yylval.cadena = strdup(yytext);
                    return BOOLEAN;
                }

(#(.)*|#=(.|\n)*=#) 	{
                            simpleDebug("COMMENT", 0);
                        }

"^" {
		simpleDebug("OP_ARIT_P1 ", 0);
		yylval.cadena = strdup(yytext);
		return OP_ARIT_P1;
	}

("/"|"%")	{
                simpleDebug("OP_ARIT_P2 ", 0);
                yylval.cadena = strdup(yytext);
                return OP_ARIT_P2;
            }

"*"	{
		simpleDebug("ASTERISCO ", 0);
		yylval.cadena = strdup(yytext);
		return ASTERISCO;
	}

("+")   {
            simpleDebug("SUMA ", 0);
            yylval.cadena = strdup(yytext);
            return SUMA;
        }

("-")   {
            simpleDebug("RESTA ", 0);
            yylval.cadena = strdup(yytext);
            return RESTA;
        }

(">"|">="|"=="|"<="|"<"|"!=")	{
                                    simpleDebug("OP_RELACIONAL ", 0);
                                    yylval.cadena = strdup(yytext);
                                    return OP_RELACIONAL;
                                }

"!"	{
		simpleDebug("NEGACION ", 0);
		return NEGACION;
	}

&&  {
        simpleDebug("OP_BOOL_AND ", 0);
        return OP_BOOL_AND;
    }

"||"	{
            simpleDebug("OP_BOOL_OR ", 0);
            return OP_BOOL_OR;
        }

"("	{
        simpleDebug("PARENTESIS_ABIERTO ", 0);
		return PARENTESIS_ABIERTO;
	}

")"	{
        simpleDebug("PARENTESIS_CERRADO ", 0);
		return PARENTESIS_CERRADO;
	}

,	{
		simpleDebug("COMA ", 0);
		return COMA;
	}

"div"	{
            simpleDebug("DIV ", 0);
            inFunction++;
            return DIV;
        }

"length"	{
                simpleDebug("LENGTH ", 0);
                inFunction++;
                return LENGTH;
            }

"size"  {
            simpleDebug("SIZE ", 0);
            inFunction++;
            return SIZE;
        }

"zeros" {
            simpleDebug("ZEROS ", 0);
            inFunction++;
            return ZEROS;
        }

"ones"  {
            simpleDebug("ONES ", 0);
            inFunction++;
            return ONES;
        }

"transpose" {
                simpleDebug("TRANSPOSE ", 0);
                inFunction++;
                return TRANSPOSE;
            }

"="	{
		simpleDebug("ASSIGN ", 0);
		assignFounded = true;
		return ASSIGN;
	}

"["	{
		simpleDebug("CORCHETE_ABIERTO ", 0);
		corcheteOpenFounded = true;
		return CORCHETE_ABIERTO;
	}

"]"	{
		simpleDebug("CORCHETE_CERRADO ", 0);
		return CORCHETE_CERRADO;
	}

;	{
		simpleDebug("PUNTO_Y_COMA ", 0);
		return PUNTO_Y_COMA;
	}

{id}	{
            if (assignFounded == false && corcheteOpenFounded == false && inFunction == 0)
            {
                simpleDebug("ID ", 0);
                yylval.ident_info.lexema = strdup(yytext);
                yylval.ident_info.lenght = strlen(yytext);
                yylval.ident_info.line = yylineno;
                return ID;
            }
            else
            {
                simpleDebug("ID_ARIT ", 0);
                sym_value_type entry;
                int response = sym_lookup(yytext, &entry);
                if (response == SYMTAB_OK)
                {
                    value_info v;
                    if (entry.value == NULL)
                    {   /* SI EL ID_ARIT ES TENSOR. */
                        v = createValueInfo(NULL, entry.type, yytext);
                    }
                    else
                    {    /* SI EL ID_ARIT ES DE DIM 0. */
                        v = createValueInfo(entry.value, entry.type, NULL);
                    }
                    if (isSameType(v.type, INT32_T) || isSameType(v.type, FLOAT64_T))
                    {
                        yylval.operand = v;
                        return ID_ARIT;
                    }
                    else if (isSameType(v.type, STRING_T))
                    {
                        yylval.cadena = v.value;
                        return STRING;
                    }
                    else
                    {
                        yylval.cadena = v.value;
                        return BOOLEAN;
                    }
                }
                else
                {
                    char *error = allocateSpaceForMessage();
                    sprintf(error, "The id %s is not initialized.", yytext);
                    yyerror(error);
                }
            }
        }

\"(\\.|[^\"])*\"	{
                        simpleDebug("STRING ", 0);
                        yylval.cadena = removeQuotationMarks(yytext);
                        return STRING;
                    }

.	{}


%%