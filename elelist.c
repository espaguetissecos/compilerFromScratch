
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "elelist.h"

/*includes, defines y/o typedef que necesitesis */

/**------------------------------------------------------------------ 
Nombre: iniEleList 
Descripcion: inicializa un elemento de la lista
Entrada: el elemento a inicializar y con qué inicializarlo
Salida: ok si todo va bien y error si no se ha conseguido inicializar
------------------------------------------------------------------*/
STATUS iniEleList(ELELIST *pe,const char* name, const int cate, const int clas, const int tip, const int tam, const int nvl, const int pvl, const int np, const int pp){
    if (pe==NULL||name ==NULL)
        return ERROR;
    pe->id = (char*) malloc(sizeof (char)*(strlen(name)+1));
    strcpy (pe->id, name);
    pe->categoria = cate;
    pe->clase = clas;
    pe->tipo = tip;
    pe->tamanio = tam;
    pe->num_variables_locales = nvl;
    pe->pos_var_local = pvl;
    pe->num_params = np;
    pe->pos_param = pp;
    if (strcmp(pe->id, name)!=0||pe==NULL || pe->categoria!=cate || pe->clase!=clas || pe->tipo!=tip || pe->tamanio!=tam || pe->num_variables_locales!=nvl || pe->pos_var_local!=pvl || pe->num_params!=np || pe->pos_param!=pp)
        return ERROR;
    return OK;
    
}

/**------------------------------------------------------------------ 
Nombre: getId 
Descripcion: consigue el identificador del elemento
Entrada: el puntero al elemento del que se quiere saber su id
Salida: el identificador del elemento
------------------------------------------------------------------*/
char* getId(ELELIST *elem){
    return elem->id;
}

/**------------------------------------------------------------------
Nombre: copyEleList
Descripcion: Copia el elemento
Entrada: elemento a copiar y elemento que va a ser copiado
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS copyEleList (ELELIST *pDest, const ELELIST *pOrigin){
    if (pDest == NULL || pOrigin == NULL){
        return ERROR;
    }
    strcpy(pDest->id, pOrigin->id);
    pDest->categoria = pOrigin->categoria;
    pDest->clase = pOrigin->clase;
    pDest->tipo = pOrigin->tipo;
    pDest->tamanio = pOrigin->tamanio;
    pDest->num_variables_locales = pOrigin->num_variables_locales;
    pDest->pos_var_local = pOrigin->pos_var_local;
    pDest->num_params = pOrigin->num_params;
    pDest->pos_param = pOrigin->pos_param;
    return OK;
}  

/**------------------------------------------------------------------
Nombre: printEleList
Descripcion: Imprime en un fichero el elemento
Entrada: Fichero en el que se imprime y el elemento a imprimir
------------------------------------------------------------------*/
void printEleList (FILE *pf, const ELELIST *pe){
    fprintf (pf, "%s\t%d\n", pe->id, pe->tipo);
}

/**------------------------------------------------------------------
Nombre: freeEleList
Descripcion: libera la memoria reservada para un elemento
Entrada: el elemento a liberar
------------------------------------------------------------------*/
void freeEleList(ELELIST *element){
    if (element==NULL){
        return;
    }
    free (element->id);
    return;
}
