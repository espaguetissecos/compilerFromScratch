%{
#include <stdio.h>
#include "generacion.h"
#include "alfa.h"

	extern int lin,col,errorMorfo;
	SYM* sym;
	ELELIST* elem;

	FILE *generaCodigo;
	extern FILE *yyout;
	extern int yyleng;
	
	int i, entero, negar;
	int etiqueta=1;
	int tipo_actual;
	int clase_actual;
	int tamanio_vector_actual;
	int num_variables_locales_actual;
	int pos_variable_local_actual;
	int num_parametros_actual;
	int pos_parametros_actual;
	int num_parametros_llamada_actual;
	int pos_parametros_llamada_actual;
	int en_explist;
	int fn_return;
	int retornofuncion;
	int es_funcion=FALSE;
	char funcion_actual[MAX_LONG_ID];
	char vector_actual[MAX_LONG_ID];
	int num_vectores_inicializados_actual;
	int operacion_vector;

	/* Prototipos de la función "yyerror" */
	void yyerror();
	int yylex();
%}

%union {
Tipo_atributos atributos;
}

/* Palabras reservadas */ 
%token TOK_MAIN
%token TOK_INT
%token TOK_BOOLEAN
%token TOK_ARRAY
%token TOK_FUNCTION
%token TOK_IF
%token TOK_ELSE
%token TOK_WHILE
%token TOK_SCANF
%token TOK_PRINTF
%token TOK_RETURN
%token TOK_COMPARE
%token TOK_WITH
%token TOK_LESS
%token TOK_EQUAL
%token TOK_GREATER
%token TOK_INIT

/* Simbolos */
%token TOK_PUNTOYCOMA
%token TOK_COMA
%token TOK_PARENTESISIZQUIERDO
%token TOK_PARENTESISDERECHO
%token TOK_CORCHETEIZQUIERDO
%token TOK_CORCHETEDERECHO
%token TOK_LLAVEIZQUIERDA
%token TOK_LLAVEDERECHA
%token TOK_ASIGNACION
%token TOK_MAS
%token TOK_MENOS
%token TOK_DIVISION
%token TOK_ASTERISCO
%token TOK_EXPONENTE
%token TOK_AND
%token TOK_OR
%token TOK_NOT
%token TOK_IGUAL
%token TOK_DISTINTO
%token TOK_MENORIGUAL
%token TOK_MAYORIGUAL
%token TOK_MENOR
%token TOK_MAYOR
%token TOK_MODULO
%token TOK_MODULO_VECTOR
%token TOK_TRUE
%token TOK_FALSE

%token <atributos> TOK_IDENTIFICADOR
%token <atributos> TOK_CONSTANTE_ENTERA
%token TOK_ERROR

%type <atributos> exp;
%type <atributos> constante;
%type <atributos> constante_logica;
%type <atributos> constante_entera;
%type <atributos> fn_declaracion;
%type <atributos> fn_name;
%type <atributos> idf_llamada_funcion;
%type <atributos> comparacion;
%type <atributos> elemento_vector;
%type <atributos> condicional;
%type <atributos> if_exp_sentencias;
%type <atributos> if_exp;
%type <atributos> bucle;
%type <atributos> while_exp;
%type <atributos> while;
%type <atributos> identificador;
%type <atributos> compare;
%type <atributos> compare_less;
%type <atributos> compare_equal;

%start programa

%left TOK_COMA
%right TOK_ASIGNACION
%left TOK_OR
%left TOK_AND
%left TOK_IGUAL TOK_DISTINTO
%left TOK_MENOR TOK_MENORIGUAL TOK_MAYOR TOK_MAYORIGUAL
%left TOK_MAS TOK_MENOS
%left TOK_ASTERISCO TOK_DIVISION TOK_MODULO TOK_MODULO_VECTOR
%right TOK_NOT MENOS_UNARIO TOK_EXPONENTE
%left TOK_PARENTESISIZQUIERDO TOK_PARENTESISDERECHO TOK_CORCHETEIZQUIERDO TOK_CORCHETEDERECHO
%%


 /* Reglas de producción */
programa:	inicioTabla TOK_MAIN TOK_LLAVEIZQUIERDA declaraciones escritura_TS funciones escritura_main sentencias TOK_LLAVEDERECHA finTabla {
				fprintf(yyout, ";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n"); 
			};

inicioTabla:	{	sym = iniSym(); };

escritura_TS:	{	segmentoBSS(generaCodigo, sym);
					segmentoDATA(generaCodigo);
					segmentoTEXT(generaCodigo);
				};

escritura_main: {	gc_main(generaCodigo); };

finTabla:	{ cerrarAmbito(sym);
			  gc_fin_programa(generaCodigo);
			};

declaraciones: declaracion {
					fprintf(yyout, ";R2:\t<declaraciones> ::= <declaracion>\n"); 
				}
	| declaracion declaraciones {
		fprintf(yyout, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n");
	};
				
declaracion:	clase identificadores TOK_PUNTOYCOMA {
					fprintf(yyout, ";R4:\t<declaracion> ::= <clase> <identificadores> ;\n");
				};

clase:	clase_escalar {
			clase_actual = TIPO_ESCALAR;
			fprintf(yyout, ";R5:\t<clase> ::= <clase_escalar>\n");
		}
		| clase_vector {
			clase_actual = TIPO_VECTOR;
			fprintf(yyout, ";R7:\t<clase> ::= <clase_vector>\n");
		};

clase_escalar:	tipo {
					fprintf(yyout, ";R9:\t<clase_escalar> ::= <tipo>\n");
				};

tipo:	TOK_INT {
			tipo_actual = TIPO_INT;
			fprintf(yyout, ";R10:\t<tipo> ::= int\n");
		}
		| TOK_BOOLEAN {
			tipo_actual = TIPO_LOGICO;
			fprintf(yyout, ";R11:\t<tipo> ::= boolean\n");
		};

clase_vector:	TOK_ARRAY tipo TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO {
					tamanio_vector_actual = $4.valor_entero;
					if(tamanio_vector_actual < 1 || tamanio_vector_actual > MAX_TAMANIO_VECTOR){
						printf("**** Error semantico en [lin %d, col %d]: El tamanyo del vector excede los limites permitidos (1,64)\n", lin, col-yyleng);
						return 0;
					}
					fprintf(yyout, ";R15:\t<clase_vector> ::= array <tipo> [ <constante_entera> ]\n");
				};

identificadores:	identificador {
						fprintf(yyout, ";R18:\t<identificadores> ::= <identificador>\n");
					}
			   		| identificador TOK_COMA identificadores {
			   			fprintf(yyout, ";R19:\t<identificadores> ::= <identificador> , <identificadores>\n");
			   		};

funciones:	funcion funciones {
				fprintf(yyout, ";R20:\t<funciones> ::= <funcion> <funciones>\n");
			}
			| {fprintf(yyout, ";R21:\t<funciones> ::=\n");};
			
funcion:	fn_declaracion sentencias TOK_LLAVEDERECHA {
				if(strcmp(sym->ambito, "local") != 0){
					printf("****Error semantico en [lin %d, col %d]: Ambito de la funcion sin declarar (%s)\n", lin, col-yyleng, $1.lexema);
					return 0;
				}
				cerrarAmbito(sym);
				if(strcmp(sym->ambito, "global") != 0){
					printf("****Error semantico en [lin %d, col %d]: No se cerro el ambito de la funcion (%s)\n", lin, col-yyleng, $1.lexema);
					return 0;
				}
				elem = buscarSimbolo(sym, $1.lexema);
				if (elem == NULL) {
					printf("****Error semantico en [lin %d, col %d]: Funcion sin declarar (%s)\n", lin, col-yyleng, $1.lexema);
					return 0; 
				}
				elem->num_params = num_parametros_actual;
				if(fn_return == 0){
					printf("****Error semantico en [lin %d, col %d]: Funcion no retorna nada (%s)\n", lin, col-yyleng, $1.lexema);
					return 0; 
				}
				fprintf(yyout, ";R22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ) { <declaraciones_funcion> <sentencias> }\n"); 
			};

fn_declaracion:	fn_name TOK_PARENTESISIZQUIERDO parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion {
					if(strcmp(sym->ambito, "local") != 0){
						printf("****Error semantico en [lin %d, col %d]: No existe el ambito de la funcion (%s)\n", lin, col-yyleng, $1.lexema);
						return 0;
					}				
					elem = buscarSimbolo(sym, $1.lexema);
					if (elem == NULL) {
						printf("****Error semantico en [lin %d, col %d]: Funcion sin declarar (%s)", lin, col-yyleng, $1.lexema);
						return 0; 
					}
					/*GENERACION CODIGO*/
					gc_iniCuerpo_funcion (generaCodigo, $1.lexema, num_variables_locales_actual);
					elem->num_params = num_parametros_actual;
					strcpy($$.lexema,$1.lexema);
				};

fn_name:	TOK_FUNCTION tipo TOK_IDENTIFICADOR {
				elem = buscarSimbolo(sym, $3.lexema);
				if (elem != NULL) {
					printf("****Error semantico en [lin %d, col %d]: Nombre de funcion duplicada (%s)", lin, col-yyleng, $3.lexema);
					return 0; 
				}
				if(iniLocal(sym, $3.lexema, TIPO_FUNCION, clase_actual, tipo_actual, 0,  0, 0, 0, 0)==ERROR){
					printf("****Error semantico en [lin %d, col %d]: No se puede crear la funcion (%s)", lin, col-yyleng, $3.lexema);
					return 0;
				}
				num_variables_locales_actual = 0;
				pos_variable_local_actual = 1;
				num_parametros_actual = 0;
				pos_parametros_actual = 0;
				fn_return = 0;
				strcpy($$.lexema, $3.lexema);
				es_funcion = TRUE;
				retornofuncion = tipo_actual;
				strcpy(funcion_actual, $3.lexema);
			};

parametros_funcion:	parametro_funcion resto_parametros_funcion {
						fprintf(yyout, ";R23:\t<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>\n");
					}
					| {fprintf(yyout, ";R24:\t<parametros_funcion> ::=\n");};

resto_parametros_funcion:	TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion {
								fprintf(yyout, ";R25:\t<resto_parametros_funcion> ::= ; <parametro_funcion> <resto_parametros_funcion>\n"); 
							}
							| {fprintf(yyout, ";R26:\t<resto_parametros_funcion> ::=\n");};

parametro_funcion:	tipo idpf {
						fprintf(yyout, ";R27:\t<parametro_funcion> ::= <tipo> <identificador>\n");
					};
					
idpf:	TOK_IDENTIFICADOR {
		elem = buscarSimbolo(sym, $1.lexema);
        if (elem != NULL) {
            printf("****Error semantico en [lin %d, col %d]: Variable declarada (%s)", lin, col-yyleng, $1.lexema);
			return 0; 
        } 
		if (strcmp(sym->ambito, "local") != 0) {
			printf("****Error semantico en [lin %d, col %d]: Declaracion fuera de la funcionh", lin, col-yyleng);
			return 0;
		}
		if (insertaSimbolo(sym->local, $1.lexema, TIPO_PARAMETRO, TIPO_ESCALAR, tipo_actual, 0, 0, 0, 0, pos_parametros_actual) == ERROR) {
			printf("****Error semantico en [lin %d, col %d]: Declaracion dupliacada", lin, col-yyleng);
			return 0;
		}
		pos_parametros_actual++;
		num_parametros_actual++;
		};

declaraciones_funcion:	declaraciones {
							fprintf(yyout, ";R28:\t<declaraciones_funcion> ::= <declaraciones>\n");
						}
						| {fprintf(yyout, ";R29:\t<declaraciones_funcion> ::=\n");};

sentencias:	sentencia {
				fprintf(yyout, ";R30:\t<sentencias> ::= <sentencia>\n");
			}
			| sentencia sentencias {
				fprintf(yyout, ";R31:\t<sentencias> ::= <sentencia> <sentencias>\n");
			};

sentencia:	sentencia_simple TOK_PUNTOYCOMA {
				fprintf(yyout, ";R32:\t<sentencia> ::= <sentencia_simple> ;\n");
			}
			| bloque {
				fprintf(yyout, ";R33:\t<sentencia> ::= <bloque>\n");
			};

sentencia_simple:	asignacion {
				fprintf(yyout, ";R34:\t<sentencia_simple> ::= <asignacion>\n");
			}
			| lectura {
				fprintf(yyout, ";R35:\t<sentencia_simple> ::= <lectura>\n");
			}
			| escritura {
				fprintf(yyout, ";R36:\t<sentencia_simple> ::= <escritura>\n");
			}
			| retorno_funcion {
				fprintf(yyout, ";R38:\t<sentencia_simple> ::= <retorno_funcion>\n");
			}
			| inicializacion_vector {
				fprintf(yyout, ";<sentencia_simple> ::= <inicializacion_vector>\n");
			}
			| sentencia_simple_incremento{};

inicializacion_vector:	idf_inicializacion_vector TOK_LLAVEIZQUIERDA lista_inicializacion TOK_LLAVEDERECHA{
						elem = buscarSimbolo(sym, vector_actual);
						if (elem == NULL) {
							printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, vector_actual);
							return 0;
						}
						if(num_vectores_inicializados_actual > elem->tamanio){
							printf("****Error semantico en [lin %d, col %d]: Lista de inicializacion de longitud incorrecta (%s)\n", lin, col-yyleng, vector_actual);
							return 0;
						}
						fprintf(yyout, "<inicializacion> ::= init <identificador> {<lista_inicializacion>}\n");
					};

idf_inicializacion_vector:	TOK_INIT TOK_IDENTIFICADOR {
				elem = buscarSimbolo(sym, $2.lexema);
				if (elem == NULL) {
					printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $2.lexema);
					return 0;
				}
				if(elem->clase != TIPO_VECTOR){
					printf("****Error semantico en [lin %d, col %d]: Intento de inicializacion de una variable que no es de tipo vector\n", lin, col-yyleng);
					return 0;
				}
				num_vectores_inicializados_actual = 0;
				strcpy(vector_actual, $2.lexema);
				tamanio_vector_actual = elem->tamanio;
			};

lista_inicializacion: elemento resto_lista_inicializacion{fprintf(yyout, "<lista_inicializacion> ::= <elemento_init> <resto_lista_inicializacion>\n");};

resto_lista_inicializacion:	TOK_PUNTOYCOMA elemento resto_lista_inicializacion{fprintf(yyout, "<resto_lista_inicializacion> ::= ; <exp> <resto_lista_inicializacion>\n");}
	|	{};

elemento:	exp {
				if(num_vectores_inicializados_actual >= tamanio_vector_actual){
				printf("****Error semantico en [lin %d, col %d]: Lista de inicialización de longitud incorrecta.\n", lin, col-yyleng);
				return 0;
			}
			if(tipo_actual != $1.tipo){
				printf("****Error semantico en [lin %d, col %d]: Lista de inicialización con expresion de tipo incorrecto.\n", lin, col-yyleng);
				return 0;
			}
			/*GENERACION CODIGO*/
			gc_inicializar_vector(generaCodigo, $1.es_direccion, vector_actual, num_vectores_inicializados_actual);

			num_vectores_inicializados_actual++;
};

sentencia_simple_incremento:	TOK_IDENTIFICADOR TOK_MAS TOK_ASIGNACION exp{
					elem = buscarSimbolo(sym, $1.lexema);/* Buscar en tabla simbolos */
					if(elem == NULL){printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $1.lexema);return 0;}
					if(elem->categoria == TIPO_FUNCION){printf("****Error semantico en [lin %d, col %d]: El incremento debe ser de una variable, no funciones\n", lin, col-yyleng);return 0;}
					if(elem->tipo != TIPO_INT || $4.tipo != TIPO_INT) {printf("****Error semantico en [lin %d, col %d]: Incremento incompatible, solo posible de enteros\n", lin, col-yyleng);return 0;}
					if(elem->clase == TIPO_ESCALAR) {
						/*GENERACION CODIGO*/
						if(elem->categoria == TIPO_PARAMETRO){
							gc_incremento_parametro(generaCodigo, $4.es_direccion, num_parametros_actual, elem->pos_param);
						}else if(elem->pos_var_local == 0){
							gc_incremento_identificador(generaCodigo, $1.lexema, $4.es_direccion);
						}else {
							gc_incremento_local(generaCodigo, $4.es_direccion, elem->pos_var_local);
						}
					}else if(elem->clase == TIPO_VECTOR){
						/*GENERACION CODIGO*/
						for(i=0; i<elem->tamanio; i++){
							fprintf(generaCodigo, "\tpop dword ecx\n");
							fprintf(generaCodigo, "\tpush dword %d\n", i);
							gc_comprobacion_vector(generaCodigo, $1.lexema, 0, elem->tamanio, en_explist);
							gc_incremento_vector(generaCodigo, $4.es_direccion);
							fprintf(generaCodigo, "\tpush dword %d\n", entero);
							if(negar == 1){
								gc_negacion_enteros(generaCodigo, $4.es_direccion);
							}
						}
						fprintf(generaCodigo, "\tpop dword ecx\n");
						negar = 0;
					}else{printf("****Error semantico en [lin %d, col %d]: Incremento incompatible\n", lin, col-yyleng);return 0;}
	};

bloque:	condicional {
			fprintf(yyout, ";R40:\t<bloque> ::= <condicional>\n");	
		}
	  	| bucle {
	  		fprintf(yyout, ";R41:\t<bloque> ::= <bucle>\n");
	  	};

asignacion:	TOK_IDENTIFICADOR TOK_ASIGNACION exp {
				elem = buscarSimbolo(sym, $1.lexema);/* Buscar en tabla simbolos */
				if(elem == NULL){printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $1.lexema);return 0;}
				if(elem->categoria == TIPO_FUNCION){printf("****Error semantico en [lin %d, col %d]: Asignacion incompatible\n", lin, col-yyleng);return 0;}
				if(elem->clase == TIPO_VECTOR) {printf("****Error semantico en [lin %d, col %d]: Asignacion incompatible\n", lin, col-yyleng);return 0;}
				if(elem->tipo != $3.tipo) {printf("****Error semantico en [lin %d, col %d]: Asignacion incompatible\n", lin, col-yyleng);return 0;}
				
				/*GENERACION CODIGO*/
				if(elem->categoria == TIPO_PARAMETRO){
					gc_asignacion_parametro(generaCodigo, $3.es_direccion, num_parametros_actual, elem->pos_param);
				}else if(elem->pos_var_local == 0){
					gc_asignacion_identificador(generaCodigo, $1.lexema, $3.es_direccion);
				}else {
					gc_asignacion_local(generaCodigo, $3.es_direccion, elem->pos_var_local);
				}
				fprintf(yyout, ";R43:\t<asignacion> ::= <identificador> = <exp>\n");
			}
			| elemento_vector TOK_ASIGNACION exp {
				if($1.tipo != $3.tipo){
					printf("**** Error semantico en [lin %d, col %d]: Se esta intentando asignar a un vector un tipo de dato incompatible\n", lin, col-yyleng);
					return 0;
				}
				operacion_vector = 0;
				/*GENERACION CODIGO*/
				gc_asignacion_vector(generaCodigo, $3.es_direccion);
				fprintf(yyout, ";R44:\t<asignacion> ::= <elemento_vector> = <exp>\n");
			};

elemento_vector:	TOK_IDENTIFICADOR TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO {
						elem = buscarSimbolo(sym, $1.lexema);/* Buscar en tabla simbolos */
						if(elem == NULL){printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $1.lexema);return 0;}
						if(elem->clase != TIPO_VECTOR){printf("****Error semantico en [lin %d, col %d]: Intento de indexacion de una variable que no es de tipo vector (%s)\n", lin, col-yyleng, $1.lexema);return 0;}
						if($3.tipo != TIPO_INT){printf("****Error semantico en [lin %d, col %d]: El indice en una operacion de indexacion tiene que ser de tipo entero (%s)\n", lin, col-yyleng, $1.lexema);return 0;}
						$$.tipo = elem->tipo;
						$$.es_direccion = 1;
						operacion_vector = 1;
						$$.clase = elem->clase;
						$$.valor_entero = elem->tamanio;
						/*GENERACION CODIGO*/
						gc_comprobacion_vector(generaCodigo, $1.lexema, $3.es_direccion, elem->tamanio, en_explist);
						fprintf(yyout, ";R48:\t<elemento_vector> ::= <identificador> [ <exp> ]\n");
					};

condicional:	if_exp sentencias TOK_LLAVEDERECHA {
					/*GENERACION CODIGO*/
					gc_cerrar_if(generaCodigo, $1.etiqueta);
					fprintf(yyout, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> }\n");
				}
				| if_exp_sentencias TOK_ELSE TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA {
			   		/*GENERACION CODIGO*/
					gc_cerrar_else(generaCodigo, $1.etiqueta);
					fprintf(yyout, ";R51:\t<condicional> ::= if ( <exp> ) { <sentencias> } else { <sentencias> }\n");
				}
				| compare_equal TOK_GREATER sentencias TOK_LLAVEDERECHA {
					/*GENERACION CODIGO*/
					gc_cerrar_comparacion(generaCodigo, $1.etiqueta);
					fprintf(yyout, ";\t<condicional> ::= compare <exp> with <exp> {less <sentencias> equal <sentencias> greater <sentencias>}\n");
				};

compare_equal:	compare_less TOK_EQUAL sentencias {$$.etiqueta = $1.etiqueta;/*GENERACION DE CODIGO*/fprintf(generaCodigo,"\tjmp near fin_compara%d", $$.etiqueta);fprintf(generaCodigo,"\ngreater%d:", $$.etiqueta);};

compare_less:	compare TOK_LESS sentencias {$$.etiqueta = $1.etiqueta;/*GENERACION DE CODIGO*/fprintf(generaCodigo,"\tjmp near fin_compara%d", $$.etiqueta);fprintf(generaCodigo,"\nequal%d:", $$.etiqueta);};

compare:	TOK_COMPARE exp TOK_WITH exp TOK_LLAVEIZQUIERDA {
			if($2.tipo != TIPO_INT || $4.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Se esperaba una expresion de tipo entero\n", lin, col-yyleng);
				return 0;
			}
			$$.etiqueta = etiqueta++;
			/*GENERACION DE CODIGO*/
			gc_abrir_comparacion(generaCodigo, $2.es_direccion, $4.es_direccion, $$.etiqueta);
		};

if_exp_sentencias:	if_exp sentencias TOK_LLAVEDERECHA {
						$$.etiqueta = $1.etiqueta;
						
						/*GENERACION CODIGO*/
						gc_if_exp_sentencias(generaCodigo, $1.etiqueta);
					};
				
if_exp:	TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA {
			if($3.tipo != TIPO_LOGICO){
				printf("****Error semantico en [lin %d, col %d]: Condicional con condicion de tipo int\n", lin, col-yyleng);
				return 0;
			}
			$$.etiqueta = etiqueta++;
				
			/*GENERACION CODIGO*/
			gc_abrir_if(generaCodigo, $3.es_direccion, $$.etiqueta);
		};

bucle:	while_exp sentencias TOK_LLAVEDERECHA {
			/*GENERACION CODIGO*/
			gc_cerrar_while(generaCodigo, $1.etiqueta);
			fprintf(yyout, ";R52:\t<bucle> ::= while ( <exp> ) { <sentencias> }\n");
		};
		
while_exp:	while exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA {
				if($2.tipo != TIPO_LOGICO){
					printf("****Error semantico en [lin %d, col %d]: Bucle con condicion de tipo int\n", lin, col-yyleng);
					return 0;
				}
				$$.etiqueta=$1.etiqueta;
					
				/*GENERACION CODIGO*/
				gc_while_exp(generaCodigo, $2.es_direccion, $$.etiqueta);
			};

while:	TOK_WHILE TOK_PARENTESISIZQUIERDO {
			$$.etiqueta = etiqueta++;
			
			/*GENERACION CODIGO*/
			gc_abrir_while(generaCodigo, $$.etiqueta);
		};

lectura:	TOK_SCANF TOK_IDENTIFICADOR {
				elem = buscarSimbolo(sym, $2.lexema);
				if (elem == NULL) {
					printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $2.lexema);
					return 0;
				}
				if(elem->categoria == TIPO_FUNCION){printf("****Error semantico en [lin %d, col %d]: Asignacion incompatible\n", lin, col-yyleng);return 0;}
				if(elem->clase == TIPO_VECTOR) {printf("****Error semantico en [lin %d, col %d]: Asignacion incompatible\n", lin, col-yyleng);return 0;}
				
				/*GENERACION CODIGO*/
				if(elem->categoria == TIPO_PARAMETRO){
					gc_lectura_parametro(generaCodigo, elem->tipo, num_parametros_actual, elem->pos_param);
				}else if(elem->pos_var_local == 0){
					gc_scanf(generaCodigo, $2.lexema, elem->tipo);
				}else{
					gc_lectura_local(generaCodigo, elem->tipo, elem->pos_var_local);
				}
				fprintf(yyout, ";R54:\t<lectura> ::= scanf <identificador>\n");
			};

escritura:	TOK_PRINTF exp {
				gc_printf(generaCodigo, $2.lexema,  $2.es_direccion, $2.tipo);
				fprintf(yyout, ";R56:\t<escritura> ::= printf <exp>\n");
			};

retorno_funcion:	TOK_RETURN exp {
						if($2.tipo != retornofuncion){
							printf("****Error semantico en [lin %d, col %d]: Retorno incorrecto\n", lin, col-yyleng);
							return 0;
						}
						if(($2.es_direccion != 0) && ($2.es_direccion != 1)){
							printf("****Error semantico en [lin %d, col %d]: Mal asignacion en direccionamiento al retorno\n", lin, col-yyleng);
							return 0;
						}
						
						/*GENERACION CODIGO*/
						gc_retorno_funcion (generaCodigo, $2.es_direccion);
						fn_return++;
						fprintf(yyout, ";R61:\t<retorno_funcion> ::= return <exp>\n");
					}
	|	{if(es_funcion == TRUE && fn_return == 0){
			printf("****Error semantico en [lin %d, col %d]: Funcion <%s> sin sentencia de retorno\n", lin, col-yyleng, funcion_actual);
			return 0;}
		};

exp:	exp TOK_MAS exp {
			if($1.tipo == TIPO_INT && $3.tipo == TIPO_INT){
				$$.tipo = TIPO_INT;
				$$.es_direccion = 0;
				/*GENERACION DE CODIGO*/
				gc_suma_enteros(generaCodigo, $1.es_direccion, $3.es_direccion);
			} else {
				$$.tipo = TIPO_LOGICO;
				$$.es_direccion = 0;
				/*GENERACION CODIGO*/
				gc_or_booleans(generaCodigo, $1.es_direccion, $3.es_direccion);
			}
			fprintf(yyout, ";R72:\t<exp> ::= <exp> + <exp>\n");
		}
		| exp TOK_MENOS exp {
			if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operandos boolean\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0;
			
	   		/*GENERACION DE CODIGO*/
			gc_resta_enteros(generaCodigo, $1.es_direccion, $3.es_direccion);
			fprintf(yyout, ";R73:\t<exp> ::= <exp> - <exp>\n");
	   	}
		| exp TOK_MODULO_VECTOR exp {
			if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operandos boolean\n", lin, col-yyleng);
				return 0;
			}
			if(operacion_vector!=1 || $1.clase != TIPO_VECTOR || $3.clase != TIPO_VECTOR){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica (%%) con operandos escalares, deben ser vectores\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0;
	   		
			/*GENERACION DE CODIGO*/
			gc_modulo_enteros(generaCodigo, $1.es_direccion, $3.es_direccion);
			fprintf(yyout, ";<exp> ::= <exp> .%% <exp>\n");
	   	}
		| exp TOK_MODULO exp {
			if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operandos boolean\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0;
	   		
			/*GENERACION DE CODIGO*/
			gc_modulo_enteros(generaCodigo, $1.es_direccion, $3.es_direccion);
			fprintf(yyout, ";<exp> ::= <exp> %% <exp>\n");
	   	}
		| exp TOK_DIVISION exp {
			if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operandos boolean\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0;
	   		
			/*GENERACION DE CODIGO*/
			gc_division_enteros(generaCodigo, $1.es_direccion, $3.es_direccion);
			fprintf(yyout, ";R74:\t<exp> ::= <exp> / <exp>\n");
	   	}
		| exp TOK_ASTERISCO exp {
			if($1.tipo == TIPO_INT && $3.tipo == TIPO_INT){
				$$.tipo = TIPO_INT;
				$$.es_direccion = 0;
				/*GENERACION DE CODIGO*/
				gc_multiplicacion_enteros(generaCodigo, $1.es_direccion, $3.es_direccion);
			} else {
				$$.tipo = TIPO_LOGICO;
				$$.es_direccion = 0;
				/*GENERACION CODIGO*/
				gc_and_booleans(generaCodigo, $1.es_direccion, $3.es_direccion);
			}
			fprintf(yyout, ";R75:\t<exp> ::= <exp> * <exp>\n");
	   	}

		| TOK_EXPONENTE exp {
			if($2.clase != TIPO_VECTOR){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operando escalar\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0; 

			/*GENERACION DE CODIGO*/
			fprintf(generaCodigo, "\n\n;Vaciamos la pila\n\tpop dword eax;Apilamos la longitud del vector\n\tpush dword %d\n", $2.valor_entero);
			
		}

		| exp TOK_EXPONENTE exp {
			if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operandos boolean\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0;

			/*GENERACION DE CODIGO*/
			gc_exponente_enteros(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
			etiqueta++;
			
		}

		| TOK_MENOS exp %prec MENOS_UNARIO {
			if($2.tipo != TIPO_INT){
				printf("****Error semantico en [lin %d, col %d]: Operacion aritmetica con operandos boolean\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_INT;
			$$.es_direccion = 0;
	   		
			/*GENERACION DE CODIGO*/
			negar = 1;
			gc_negacion_enteros(generaCodigo, $2.es_direccion);
			fprintf(yyout, ";R76:\t<exp> ::= - <exp>\n");
	   	}
		| exp TOK_AND exp {
			if($1.tipo != TIPO_LOGICO || $3.tipo != TIPO_LOGICO){
				printf("****Error semantico en [lin %d, col %d]: Operacion logica con operandos int\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_LOGICO;
			$$.es_direccion = 0;
			
			/*GENERACION CODIGO*/
			gc_and_booleans(generaCodigo, $1.es_direccion, $3.es_direccion);
	   		fprintf(yyout, ";R77:\t<exp> ::= <exp> && <exp>\n");
	   	}
		| exp TOK_OR exp {
			if($1.tipo != TIPO_LOGICO || $3.tipo != TIPO_LOGICO){
				printf("****Error semantico en [lin %d, col %d]: Operacion logica con operandos int\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_LOGICO;
			$$.es_direccion = 0;
			
			/*GENERACION CODIGO*/
			gc_or_booleans(generaCodigo, $1.es_direccion, $3.es_direccion);
	   		fprintf(yyout, ";R78:\t<exp> ::= <exp> || <exp>\n");
	   	}
		| TOK_NOT exp {
			if($2.tipo != TIPO_LOGICO){
				printf("****Error semantico en [lin %d, col %d]: Operacion logica con operandos int\n", lin, col-yyleng);
				return 0;
			}
			$$.tipo = TIPO_LOGICO;
			$$.es_direccion = 0;
			
			/*GENERACION CODIGO*/
			gc_negacion_booleans(generaCodigo, $2.es_direccion, etiqueta);
			etiqueta++;
	   		fprintf(yyout, ";R79:\t<exp> ::= ! <exp>\n");
	   	}
		| TOK_IDENTIFICADOR {
			elem = buscarSimbolo(sym, $1.lexema);
            		if (elem == NULL) {
                		printf("****Error semantico en [lin %d, col %d]: Variable no declarada (%s)\n", lin, col-yyleng, $1.lexema);
				return 0; 
           		 } 
			if(elem->categoria == TIPO_FUNCION || elem->clase == TIPO_VECTOR){
				 printf("****Error semantico en [lin %d, col %d]: Variable local de tipo no escalar\n", lin, col-yyleng);
				 return 0;
			}
			/*Buscamos dentro del elemento*/
			$$.tipo = elem->tipo;
			$$.es_direccion = 1;
			
			/*GENERACION DE CODIGO*/
			if(elem->categoria == TIPO_PARAMETRO){
				$$.es_direccion = 0;
				gc_expresion_parametro(generaCodigo, num_parametros_actual, elem->pos_param);
			}else if(elem->pos_var_local == 0){
				fprintf(generaCodigo, "\n");
				if(en_explist == 0){
					$$.es_direccion = 1;
					fprintf(generaCodigo, "\tpush dword _%s\n", $1.lexema);
				}else{
					$$.es_direccion = 0;
					fprintf(generaCodigo, "\tpush dword [_%s]\n", $1.lexema);
				}
			}else{
				$$.es_direccion = 0;
				gc_expresion_local(generaCodigo, elem->pos_var_local);
			}
	   		fprintf(yyout, ";R80:\t<exp> ::= <identificador>\n");
	   	}
		| constante {
			$$.tipo = $1.tipo;
			$$.es_direccion = $1.es_direccion;
	   		fprintf(yyout, ";R81:\t<exp> ::= <constante>\n");
	   	}
		| TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO {
			$$.tipo = $2.tipo;
			$$.es_direccion = $2.es_direccion;
	   		fprintf(yyout, ";R82:\t<exp> ::= ( <exp> )\n");
	   	}
		| TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO {
			$$.tipo = $2.tipo;
			$$.es_direccion = $2.es_direccion;
	   		fprintf(yyout, ";R83:\t<exp> ::= ( <comparacion> )\n");
	   	}
		| elemento_vector {
			$$.tipo = $1.tipo;
			if(en_explist == 0){
				$$.es_direccion = $1.es_direccion;
			}else{
				$$.es_direccion = 0;
			}
			$$.clase = $1.clase;
			$$.valor_entero = elem->tamanio;
	   		fprintf(yyout, ";R85:\t<exp> ::= <elemento_vector>\n");
	   	}
		| idf_llamada_funcion TOK_PARENTESISIZQUIERDO lista_expresiones TOK_PARENTESISDERECHO {
			elem = buscarSimbolo(sym, $1.lexema);
			if (elem == NULL) {
				printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $1.lexema);
				return 0;
			}
			if(elem->num_params != num_parametros_llamada_actual){
				printf("****Error semantico en [lin %d, col %d]: Numero incorrecto de parametros en llamada a funcion (%s)\n", lin, col-yyleng, $1.lexema);
				return 0;
			}
			
			/*GENERACION CODIGO*/
			gc_llamadas_funciones(generaCodigo, $1.lexema, num_parametros_llamada_actual);
			
			en_explist = 0;
			$$.tipo = elem->tipo;
			$$.es_direccion = 0;
	   		fprintf(yyout, ";R88:\t<exp> ::= <identificador> ( <lista_expresiones> )\n");
	   	};

idf_llamada_funcion:	TOK_IDENTIFICADOR {
				elem = buscarSimbolo(sym, $1.lexema);
				if (elem == NULL) {
					printf("****Error semantico en [lin %d, col %d]: Acceso a variable no declarada (%s)\n", lin, col-yyleng, $1.lexema);
					return 0;
				}
				if(elem->categoria != TIPO_FUNCION){
					printf("****Error semantico en [lin %d, col %d]: Asignacion incompatible\n", lin, col-yyleng);
					return 0;
				}
				if(en_explist==1){
					printf("****Error semantico en [lin %d, col %d]: No esta permitido el uso de llamadas a funciones como parametros de otras funciones\n", lin, col-yyleng);
					return 0;
				}
				num_parametros_llamada_actual = 0;
				en_explist = 1;
				strcpy($$.lexema, $1.lexema);
			};

lista_expresiones:	exp resto_lista_expresiones {
				num_parametros_llamada_actual++;
				fprintf(yyout, ";R89:\t<lista_expresiones> ::= <exp> <resto_lista_expresiones>\n");
			}
		| {fprintf(yyout, ";R90:\t<lista_expresiones> ::=\n");};

resto_lista_expresiones:	TOK_COMA exp resto_lista_expresiones {
								num_parametros_llamada_actual++;
								fprintf(yyout, ";R91:\t<resto_lista_expresiones> ::= , <exp> <resto_lista_expresiones>\n");
							}
							| {fprintf(yyout, ";R92:\t<resto_lista_expresiones> ::=\n");};

comparacion:	exp TOK_IGUAL exp {
					if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
						printf("****Error semantico en [lin %d, col %d]: Comparacion con operandos boolean\n", lin, col-yyleng);
						return 0;
					}
					$$.tipo = TIPO_LOGICO;
					$$.es_direccion = 0;
					
					/*GENERACION CODIGO*/
					gc_comparacion_igual(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
					etiqueta++;
					fprintf(yyout, ";R93:\t<comparacion> ::= <exp> == <exp>\n"); }
				| exp TOK_DISTINTO exp {
					if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
						printf("****Error semantico en [lin %d, col %d]: Comparacion con operandos boolean\n", lin, col-yyleng);
						return 0;
					}
					$$.tipo = TIPO_LOGICO;
					$$.es_direccion = 0;
			   		
					/*GENERACION CODIGO*/
					gc_comparacion_distinto(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
					etiqueta++;
					fprintf(yyout, ";R94:\t<comparacion> ::= <exp> != <exp>\n"); 
			   	}
				| exp TOK_MENORIGUAL exp {
					if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
						printf("****Error semantico en [lin %d, col %d]: Comparacion con operandos boolean\n", lin, col-yyleng);
						return 0;
					}
					$$.tipo = TIPO_LOGICO;
					$$.es_direccion = 0;
			   		
					/*GENERACION CODIGO*/
					gc_comparacion_menorigual(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
					etiqueta++;
					fprintf(yyout, ";R95:\t<comparacion> ::= <exp> <= <exp>\n"); 
			   	}
				| exp TOK_MAYORIGUAL exp {
					if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
						printf("****Error semantico en [lin %d, col %d]: Comparacion con operandos boolean\n", lin, col-yyleng);
						return 0;
					}
					$$.tipo = TIPO_LOGICO;
					$$.es_direccion = 0;
			   		
					/*GENERACION CODIGO*/
					gc_comparacion_mayorigual(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
					etiqueta++;
					fprintf(yyout, ";R96:\t<comparacion> ::= <exp> >= <exp>\n"); 
			   	}
				| exp TOK_MENOR exp {
					if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
						printf("****Error semantico en [lin %d, col %d]: Comparacion con operandos boolean\n", lin, col-yyleng);
						return 0;
					}
					$$.tipo = TIPO_LOGICO;
					$$.es_direccion = 0;
			   		
					/*GENERACION CODIGO*/
					gc_comparacion_menor(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
					etiqueta++;
					fprintf(yyout, ";R97:\t<comparacion> ::= <exp> < <exp>\n"); 
			   	}
				| exp TOK_MAYOR exp {
					if($1.tipo != TIPO_INT || $3.tipo != TIPO_INT){
						printf("****Error semantico en [lin %d, col %d]: Comparacion con operandos boolean\n", lin, col-yyleng);
						return 0;
					}
					$$.tipo = TIPO_LOGICO;
					$$.es_direccion = 0;
			   		
					/*GENERACION CODIGO*/
					gc_comparacion_mayor(generaCodigo, $1.es_direccion, $3.es_direccion, etiqueta);
					etiqueta++;
					fprintf(yyout, ";R98:\t<comparacion> ::= <exp> > <exp>\n"); 
			   	};

constante:	constante_logica {
				$$.tipo = $1.tipo;
				$$.es_direccion = $1.es_direccion;
				fprintf(yyout, ";R99:\t<constante> ::= <constante_logica>\n");
			}
			| constante_entera {
				$$.tipo = $1.tipo;
				$$.es_direccion = $1.es_direccion;
				fprintf(yyout, ";R100:\t<constante> ::= <constante_entera>\n");
			};

constante_logica:	TOK_TRUE {
						/*ANALISIS SEMANTICO*/
						$$.tipo = TIPO_LOGICO;
						$$.es_direccion = 0;
						
						/*GENERACION DE CODIGO*/
						fprintf(generaCodigo, "; numero_linea %d\n", lin); /*numero_linea?*/
						fprintf(generaCodigo, "\tpush dword 1\n");
						fprintf(yyout, ";R102:\t<constante_logica> ::= TOK_TRUE\n");
					}
					| TOK_FALSE {						
						/*ANALISIS SEMANTICO*/
						$$.tipo = TIPO_LOGICO;
						$$.es_direccion = 0;
						
						/*GENERACION DE CODIGO*/
						fprintf(generaCodigo, "; numero_linea %d\n", lin); /*numero_linea?*/
						fprintf(generaCodigo, "\tpush dword 0\n");
						fprintf(yyout, ";R103:\t<constante_logica> ::= TOK_FALSE\n");
					};

constante_entera:	TOK_CONSTANTE_ENTERA {
						/*ANALISIS SEMANTICO*/
						$$.tipo = TIPO_INT;
						$$.es_direccion = 0;
												
						/*GENERACION DE CODIGO*/
						fprintf(generaCodigo, "; numero_linea %d\n", lin); /*numero_linea?*/
						fprintf(generaCodigo, "\tpush dword %d\n", $1.valor_entero);
						entero = $1.valor_entero;
						fprintf(yyout, ";R104:\t<constante_entera> ::= TOK_CONSTANTE_ENTERA\n");
					};

identificador:	TOK_IDENTIFICADOR {
					elem = buscarSimbolo(sym, $1.lexema);
					if (elem != NULL) {
						printf("****Error semantico en [lin %d, col %d]: Identificador declarado previamente (%s)\n", lin, col-yyleng, $1.lexema);
						return 0;
					}
					if (strcmp(sym->ambito, "local") == 0) {
						if (insertaSimbolo(sym->local, $1.lexema, TIPO_VARIABLE, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
							printf("****Error semantico en [lin %d, col %d]: Declaracion dupliacada\n", lin, col-yyleng);
							return 0;
						}
					} else if (strcmp(sym->ambito, "global") == 0) {
						if (insertaSimbolo(sym->global, $1.lexema, TIPO_VARIABLE, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
							printf("****Error semantico en [lin %d, col %d]: Declaracion duplicada\n", lin, col-yyleng);
							return 0;
						}
					}
					elem = buscarSimbolo(sym, $1.lexema);
					if (elem == NULL) {
						printf("****Error semantico en [lin %d, col %d]: Identificador no declarado (%s)\n", lin, col-yyleng, $1.lexema);
						return 0;
					}
					if(es_funcion == FALSE){
						elem->tipo = tipo_actual;
						elem->clase = clase_actual;
						if(elem->clase == TIPO_VECTOR){
							elem->tamanio = tamanio_vector_actual;
							strcpy(vector_actual, $1.lexema);
						}
					}else{
						if(clase_actual == TIPO_VECTOR){
							printf("****Error semantico en [lin %d, col %d]: No se puede declarar un vector como variable local(%s)\n", lin, col-yyleng, $1.lexema);
							return 0;
						}
						num_variables_locales_actual++;
						elem->tipo = tipo_actual;
						elem->clase = clase_actual;
						elem->pos_var_local = pos_variable_local_actual;
						pos_variable_local_actual++;
					}
					fprintf(yyout, ";R108:\t<identificador> ::= TOK_IDENTIFICADOR\n");
				};
%%
/* Declaración de yyerror */
void yyerror() {
	if (errorMorfo != 1) { /* solo imprime error sintáctico si no ha habido error morfológico*/
		printf("****Error sintactico en [lin %d, col %d]\n", lin,col-yyleng);
	}
	errorMorfo = 0; /* volvemos a poner a 0 la bandera errorMorfo para futuros errores*/
}
