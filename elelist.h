
#ifndef ELELIST_H
#define	ELELIST_H
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct elemento{
    char* id; // lexema, nombre de la variable
    int categoria;
    int clase;
    int tipo;
    int tamanio;
    int num_variables_locales;
    int pos_var_local;
    int num_params;
    int pos_param;
}ELELIST;
/*includes, defines y/o typedef que necesitesis */

/**------------------------------------------------------------------ 
Nombre: iniEleList 
Descripcion: inicializa un elemento de la lista
Entrada: el elemento a inicializar y con qué inicializarlo
Salida: ok si todo va bien y error si no se ha conseguido inicializar
------------------------------------------------------------------*/
STATUS iniEleList(ELELIST *pe,const char* name, const int cate, const int clas, const int tip, const int tam, const int nvl, const int pvl, const int np, const int pp);

/**------------------------------------------------------------------ 
Nombre: getId 
Descripcion: consigue el identificador del elemento
Entrada: el puntero al elemento del que se quiere saber su id
Salida: el identificador del elemento
------------------------------------------------------------------*/
char* getId(ELELIST *elem);

/**------------------------------------------------------------------
Nombre: copyEleList
Descripcion: Copia el elemento
Entrada: elemento a copiar y elemento que va a ser copiado
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS copyEleList (ELELIST *pDest, const ELELIST *pOrigin);  

/**------------------------------------------------------------------
Nombre: printEleList
Descripcion: Imprime en un fichero el elemento
Entrada: Fichero en el que se imprime y el elemento a imprimir
------------------------------------------------------------------*/
void printEleList (FILE *pf, const ELELIST *pe);

/**------------------------------------------------------------------
Nombre: freeEleList
Descripcion: libera la memoria reservada para un elemento
Entrada: el elemento a liberar
------------------------------------------------------------------*/
void freeEleList(ELELIST *element);

#endif	/* ELELIST_H */

