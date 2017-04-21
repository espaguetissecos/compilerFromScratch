#ifndef generacion
#define generacion
#include "tablaSimbolos.h"
#include "alfa.h"

void segmentoBSS(FILE *generaCodigo, SYM* sym);
void segmentoDATA(FILE *generaCodigo);
void segmentoTEXT(FILE *generaCodigo);

void gc_main(FILE *generaCodigo);
void gc_suma_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_resta_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_modulo_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
//void gc_modulo_vector(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_division_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_multiplicacion_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_exponente_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_negacion_enteros(FILE *generaCodigo, const int es_direccion_op1);
void gc_and_booleans(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_or_booleans(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2);
void gc_negacion_booleans(FILE *generaCodigo, const int es_direccion, const int etiqueta);

/*		Funciones para COMPARACIONES	*/
void gc_comparacion_igual(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_comparacion_distinto(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_comparacion_menorigual(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_comparacion_mayorigual(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_comparacion_menor(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_comparacion_mayor(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta);
void gc_comprobacion_vector(FILE *generaCodigo, const char *lex, const int es_direccion, const int tam, const int param);

/*		Funciones para ASIGNACION	*/
void gc_incremento_identificador(FILE *generaCodigo, const char *lex, const int es_direccion);
void gc_asignacion_identificador(FILE *generaCodigo, const char *lex, const int es_direccion);
void gc_inicializar_vector(FILE *generaCodigo, const int es_direccion, const char *lex, const int inicializados);
void gc_incremento_vector(FILE *generaCodigo, const int es_direccion);
void gc_asignacion_vector(FILE *generaCodigo, const int es_direccion);

/*		Funciones para LECTURA y ESCRITURA	*/
void gc_scanf(FILE *generaCodigo, const char *lex, const int tipo);
void gc_printf(FILE *generaCodigo, const char *lex, const int es_direccion, const int tipo);

/*		Comparaciones		*/
void gc_abrir_comparacion(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta);
void gc_cerrar_comparacion(FILE *generaCodigo, const int etiqueta);

/*		Funciones para IF	*/
void gc_abrir_if(FILE *generaCodigo, const int es_direccion, const int etiqueta);
void gc_cerrar_if(FILE *generaCodigo, const int etiqueta);
void gc_if_exp_sentencias(FILE *generaCodigo, const int etiqueta);
void gc_cerrar_else(FILE *generaCodigo, const int etiqueta);

/*		Funciones para WHILE	*/
void gc_abrir_while(FILE *generaCodigo, const int etiqueta);
void gc_while_exp(FILE *generaCodigo, const int es_direccion, const int etiqueta);
void gc_cerrar_while(FILE *generaCodigo, const int etiqueta);

/*		Funciones para FUNCIONES	*/
void gc_llamadas_funciones(FILE *generaCodigo, const char *lex, const int varLocal);
void gc_iniCuerpo_funcion (FILE *generaCodigo, const char *lex, const int numVar);
void gc_cuerpo_funcion (FILE *generaCodigo, const char *lex, const int numArg, const int posPar, const int posVar);
void gc_retorno_funcion (FILE *generaCodigo, const int es_direccion);
void gc_finCuerpo_funcion (FILE *generaCodigo);
void gc_expresion_parametro(FILE *generaCodigo, const int numPar, const int posPar);
void gc_lectura_parametro(FILE *generaCodigo, const int tipo, const int numPar, const int posPar);
void gc_incremento_parametro(FILE *generaCodigo, const int es_direccion, const int numPar, const int posPar);
void gc_asignacion_parametro(FILE *generaCodigo, const int es_direccion, const int numPar, const int posPar);
void gc_localizacion_parametro(FILE *generaCodigo, const int numPar, const int posPar);
void gc_expresion_local(FILE *generaCodigo, const int posVar);
void gc_lectura_local(FILE *generaCodigo, const int tipo, const int posVar);
void gc_incremento_local(FILE *generaCodigo, const int es_direccion, const int posVar);
void gc_asignacion_local(FILE *generaCodigo, const int es_direccion, const int posVar);
void gc_localizacion_locales(FILE *generaCodigo, const int posVar);
void gc_direccion_varGlobal_parametros(FILE *generaCodigo, const char *nombreVar);

void gc_fin_programa(FILE *generaCodigo);
void funcionErrores(FILE *generaCodigo);

#endif
