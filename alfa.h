
#ifndef _ALFA_H
#define _ALFA_H

#define MAX_LONG_ID 100
#define MAX_TAMANIO_VECTOR 64

#define TIPO_VARIABLE	1
#define TIPO_FUNCION	2
#define TIPO_PARAMETRO	3

#define TIPO_INT	1
#define TIPO_LOGICO	2

#define TIPO_ESCALAR	1
#define TIPO_VECTOR	2
/* otros defines */

typedef struct{
	char lexema[MAX_LONG_ID+1]; /* nuestro id */
	int tipo;
	int clase;					
	int valor_entero;
	int es_direccion; 
	int etiqueta;
}Tipo_atributos;

#endif
