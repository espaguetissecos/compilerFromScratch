/***********************************************/
/* Fichero: tablaSimbolos.h   Fecha:25-10-2015 */
/* Autores:Laura Cerdá y Carla Civantos        */
/*                                             */
/* Fichero de gestion de la tabla donde se     */
/*   almacenan los simbolos                    */
/***********************************************/

#ifndef _tablaSimbolos_H
#define	_tablaSimbolos_H

#include "hash.h"

/*#define TIPO_VARIABLE	1
#define TIPO_FUNCION	2
#define TIPO_PARAMETRO	3

#define TIPO_INT	1
#define TIPO_LOGICO	2

#define TIPO_ESCALAR	1
#define TIPO_VECTOR	2*/

/* Variables externas: accesibles desde el programa de prueba */
typedef struct{
    HASH* global;
    HASH* local;
    char ambito[10]; //Flag que indica el ámbito actual("global" o "local")
}SYM;

/*-----------------------------------------------------------------------
 Función: iniSym                               
 Descripción: inicializa la estructura SYM que contiene los ambitos       
 Salida: la estructura Sym si todo va bien y NULL en otro caso
-----------------------------------------------------------------------*/
SYM *iniSym();

/*-----------------------------------------------------------------------
 Función: iniLocal                               
 Descripción: Abre un nuevo ámbito local(con     
              todas sus consecuencias)           
 Entrada: puntero a la estructura SYM, identificador de la función 
 			del ámbito local a abrir, campo tipo de la función         
 Salida: ERR si ha habido error o OK si ha ido bien    
-----------------------------------------------------------------------*/
STATUS iniLocal(SYM *sym, const char* id, const int categoria, const int clase, const int tipo, const int tamanio, const int num_vars_locales, const int pos_vars_locales, const int num_params, const int pos_params);

/*-----------------------------------------------------------------------
 Función: insertaSimbolo                               
 Descripción: Inserta un nuevo símbolo en el ámbito que corresponda           
 Entrada: puntero a la tabla hash, identificador del elemento a insertar 
 			y tipo del elemento a insertar      
 Salida: ERR si ha habido error o OK si ha ido bien    
-----------------------------------------------------------------------*/
STATUS insertaSimbolo(HASH *tabla, const char* id, const int categoria, const int clase, const int tipo, const int tamanio, const int num_vars_locales, const int pos_vars_locales, const int num_params, const int pos_params);

/*-----------------------------------------------------------------------
 Función: buscarSimbolo                               
 Descripción: Busca un símbolo en el ámbito que corresponda           
 Entrada: puntero a la estructura SYM, identificador del elemento a buscar 
 Salida: el elemento en caso de encontrarlo y NULL caso contrario   
-----------------------------------------------------------------------*/
ELELIST *buscarSimbolo(SYM *sym, const char *id);

/*-----------------------------------------------------------------------
 Función: cerrarAmbito                               
 Descripción: libera el ambito          
 Entrada: puntero a la estructura SYM
-----------------------------------------------------------------------*/
void cerrarAmbito(SYM* sym);

/*-----------------------------------------------------------------------
 Función: freeSym                               
 Descripción: libera el todos los ambitos abiertos          
 Entrada: puntero a la estructura SYM
-----------------------------------------------------------------------*/
void freeSym(SYM* sym);
#endif
