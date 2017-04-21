%{
#include <stdio.h>
#include <string.h>
#include "alfa.h"
#include "y.tab.h"

#define COMENTARIO 0
#define ERROR_COLUM 0

int col = 1, lin = 1;
int errorMorfo = 0, err = 0;
int errorMorfologico(int tipoError); //Realiza las acciones de cada token que produce un ERROR
int retornoToken(int token);
%}

%option noyywrap
%option nounput
%option noinput

MINUS	[a-z]
IDENTIFICADOR	[a-zA-Z][a-zA-Z0-9]*
LETRA	[a-zA-Z]
ENTERO	[0-9]+
COMENTARIO	"//".*
COMENTARIOS	"@".*

%%

{COMENTARIO}	{}
{COMENTARIOS}	{ col+=yyleng; fprintf(yyout, ";COMENTARIO:\t%s\n", yytext);}

"main"	{ return retornoToken(TOK_MAIN);}
"int"		{ return retornoToken(TOK_INT);}
"boolean"	{ return retornoToken(TOK_BOOLEAN);}
"array"	{ return retornoToken(TOK_ARRAY);}
"function"	{ return retornoToken(TOK_FUNCTION);}
"if"		{ return retornoToken(TOK_IF);}
"else"	{ return retornoToken(TOK_ELSE);}
"while"	{ return retornoToken(TOK_WHILE);}
"scanf"	{ return retornoToken(TOK_SCANF);}
"printf"	{ return retornoToken(TOK_PRINTF);}
"return"	{ return retornoToken(TOK_RETURN);}
"compare"	{ return retornoToken(TOK_COMPARE);}
"with"	{ return retornoToken(TOK_WITH);}
"less"	{ return retornoToken(TOK_LESS);}
"equal"	{ return retornoToken(TOK_EQUAL);}
"greater"	{ return retornoToken(TOK_GREATER);}
"init"	{ return retornoToken(TOK_INIT);}

";"	{ return retornoToken(TOK_PUNTOYCOMA);}
","	{ return retornoToken(TOK_COMA);}
"("	{ return retornoToken(TOK_PARENTESISIZQUIERDO);}
")"	{ return retornoToken(TOK_PARENTESISDERECHO);}
"["	{ return retornoToken(TOK_CORCHETEIZQUIERDO);}
"]"	{ return retornoToken(TOK_CORCHETEDERECHO);}
"{"	{ return retornoToken(TOK_LLAVEIZQUIERDA);}
"}"	{ return retornoToken(TOK_LLAVEDERECHA);}
"="	{ return retornoToken(TOK_ASIGNACION);}
"+"	{ return retornoToken(TOK_MAS);}
"-"	{ return retornoToken(TOK_MENOS);}
"/"	{ return retornoToken(TOK_DIVISION);}
"*"	{ return retornoToken(TOK_ASTERISCO);}
"^"	{ return retornoToken(TOK_EXPONENTE);}
"&&"	{ return retornoToken(TOK_AND);}
"||"	{ return retornoToken(TOK_OR);}
"!"	{ return retornoToken(TOK_NOT);}
"=="	{ return retornoToken(TOK_IGUAL);}
"!="	{ return retornoToken(TOK_DISTINTO);}
"<="	{ return retornoToken(TOK_MENORIGUAL);}
">="	{ return retornoToken(TOK_MAYORIGUAL);}
"<"	{ return retornoToken(TOK_MENOR);}
">"	{ return retornoToken(TOK_MAYOR);}
"%"	{ return retornoToken(TOK_MODULO);}
".%"	{ return retornoToken(TOK_MODULO_VECTOR);}

"true"	{ return retornoToken(TOK_TRUE);}
"false"	{ return retornoToken(TOK_FALSE);}

"\r"	{} // Para Windows
"\n"	{ col=1;lin++;} // Para Linux
[ \t]	{ col+=yyleng;}

{IDENTIFICADOR}	{ return retornoToken(TOK_IDENTIFICADOR);}
{ENTERO}	{ return retornoToken(TOK_CONSTANTE_ENTERA);}

.	{ return retornoToken(TOK_ERROR);}

%%
int errorMorfologico (int tipoError) {
	errorMorfo = 1; //Activo la bandera de error morfológico
	if (tipoError == ERROR_COLUM) {  
		printf("****Error en [lin %d, col %d]: simbolo no permitido (%s)\n", lin, col, yytext);
	} else {
		printf("****Error en [lin %d, col %d]: identificador demasiado largo (%s)\n", lin, col, yytext);
	}
	col+=yyleng; //Aumento el número de columnas(avanzo dentro de la fila)
	return TOK_ERROR;
}

int retornoToken(int token) {
	if(token == TOK_ERROR){
		token = errorMorfologico(ERROR_COLUM);
	}else if(token == TOK_IDENTIFICADOR){
		col+=yyleng; 
		strcpy(yylval.atributos.lexema, yytext);
		fprintf(yyout, ";D:\t%s\n", yytext);
	}else if(token == TOK_CONSTANTE_ENTERA){
		col+=yyleng; 
		yylval.atributos.valor_entero = atoi(yytext);
		fprintf(yyout, ";D:\t%s\n", yytext);
	}else{
		col+=yyleng; //Aumento el número de columnas(avanzo dentro de la fila)
		fprintf(yyout, ";D:\t%s\n", yytext);
	}
	return token;
}
