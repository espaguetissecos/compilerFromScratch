/***********************************************/
/* Fichero: tablaSimbolos.c   Fecha:25-10-2015 */
/* Autores:Laura Cerdá y Carla Civantos        */
/*                                             */
/* Fichero de gestion de la tabla donde se     */
/*   almacenan los simbolos                    */
/***********************************************/

#include "tablaSimbolos.h"

/*-----------------------------------------------------------------------
 Función: iniSym                               
 Descripción: inicializa la estructura SYM que contiene los ambitos       
 Salida: la estructura Sym si todo va bien y NULL en otro caso
-----------------------------------------------------------------------*/
SYM *iniSym() {
    SYM* sym;
    sym = (SYM *) malloc(sizeof (SYM));
    if (sym == NULL) {
        return NULL;
    }
    sym->global = creaVector();
    sym->local = NULL;
    strcpy(sym->ambito, "global");
    return sym;
}

/*-----------------------------------------------------------------------
 Función: iniLocal                               
 Descripción: Abre un nuevo ámbito local(con     
              todas sus consecuencias)           
 Entrada: puntero a la estructura SYM, identificador de la función 
            del ámbito local a abrir, campo tipo de la función         
 Salida: ERR si ha habido error o OK si ha ido bien    
-----------------------------------------------------------------------*/
STATUS iniLocal(SYM *sym, const char* id, const int categoria, const int clase, const int tipo, const int tamanio, const int num_vars_locales, const int pos_vars_locales, const int num_params, const int pos_params) {
    /*Si ya hay un ámbito local abierto,no se puede abrir otro*/
    if (strcmp(sym->ambito, "local") == 0) {
        return ERROR;
    }
    /*Inserción de la función y sus campos en la tabla global*/
    if (insertaSimbolo(sym->global, id, categoria, clase, tipo, tamanio, num_vars_locales, pos_vars_locales, num_params, pos_params) == ERROR) {
        return ERROR;
    }

    /*Cambio el ámbito actual al local*/
    sym->local = creaVector();
    strcpy(sym->ambito, "local");
    
    /*Inserción de la función y sus campos en la tabla local*/
    if (insertaSimbolo(sym->local, id, categoria, clase, tipo, tamanio, num_vars_locales, pos_vars_locales, num_params, pos_params) == ERROR) {
        return ERROR;
    }
    return OK;
}

/*-----------------------------------------------------------------------
 Función: insertaSimbolo                               
 Descripción: Inserta un nuevo símbolo en el ámbito que corresponda           
 Entrada: puntero a la tabla hash, identificador del elemento a insertar 
            y tipo del elemento a insertar      
 Salida: ERR si ha habido error o OK si ha ido bien    
-----------------------------------------------------------------------*/
STATUS insertaSimbolo (HASH *tabla, const char* id, const int categoria, const int clase, const int tipo, const int tamanio, const int num_vars_locales, const int pos_vars_locales, const int num_params, const int pos_params) {
    ELELIST *buscar, elem;    

    /*Comprobamos si el simbolo esta ya en la tabla*/
    buscar = buscVector(tabla, id);
    if (buscar == NULL) {
        iniEleList(&elem, id, categoria, clase, tipo, tamanio, num_vars_locales, pos_vars_locales, num_params, pos_params);
        if (insVector(&elem, tabla) != ERROR) {
            freeEleList(&elem);
            return OK;
        }
    }
    return ERROR;
}

/*-----------------------------------------------------------------------
 Función: buscarSimbolo                               
 Descripción: Busca un símbolo en el ámbito que corresponda           
 Entrada: puntero a la estructura SYM, identificador del elemento a buscar 
 Salida: el elemento en caso de encontrarlo y NULL caso contrario   
-----------------------------------------------------------------------*/
ELELIST *buscarSimbolo(SYM *sym, const char *id) {
    ELELIST* elem;    
    if (!strcmp(sym->ambito, "local")) {/*Primero busco en el ámbito local*/
        elem = buscVector(sym->local, id);
        if (elem != NULL && !strcmp(getId(elem), id)) {
            return elem;
        }
    }
    elem = buscVector(sym->global, id);
    if (elem != NULL && !strcmp(getId(elem), id)) {
        return elem;
    }

    return NULL;
}

/*-----------------------------------------------------------------------
 Función: cerrarAmbito                               
 Descripción: libera el ambito          
 Entrada: puntero a la estructura SYM
-----------------------------------------------------------------------*/
void cerrarAmbito(SYM* sym) {
    if (strcmp(sym->ambito, "local") == 0) {
        borraVector(sym->local);
        sym->local = NULL;
        strcpy(sym->ambito, "global");
    }else if (strcmp(sym->ambito, "global") == 0) {
        borraVector(sym->global);
        sym->global = NULL;
    }
}

/*-----------------------------------------------------------------------
 Función: freeSym                               
 Descripción: libera el todos los ambitos abiertos          
 Entrada: puntero a la estructura SYM
-----------------------------------------------------------------------*/
void freeSym(SYM* sym) {
    if (sym->local != NULL) {
        cerrarAmbito(sym);
    }
    if (sym->global != NULL) {
        cerrarAmbito(sym);
    }
    free(sym);
}