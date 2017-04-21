
#include <stdio.h>
#include <string.h>
#include "hash.h"

/**------------------------------------------------------------------
Nombre: creaVector
Descripcion: Inicializa una tabla hash
Salida: la tabla inicializada
------------------------------------------------------------------*/
HASH *creaVector() {
    HASH *vec;
    int i;

    vec = (HASH*) malloc(sizeof (HASH));
    if (!vec)
        return NULL;
    for (i = 0; i < TAMVECTOR; i++) {
        iniList(&(vec->vect[i]));
    }
    return vec;
}

/**------------------------------------------------------------------
Nombre: borraVector
Descripcion: Libera la tabla hash
Entrada: puntero a la tabla hash
------------------------------------------------------------------*/
void borraVector(HASH *vec) {
    int i;
    for (i = 0; i < TAMVECTOR; i++) {
        freeList(&(vec->vect[i]));
    }
    free(vec);
}

/**------------------------------------------------------------------
Nombre: insVector
Descripcion: inserta un elemento en la tabla hash
Entrada: un elemento y la tabla hash donde insertarlo
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS insVector(ELELIST *elem, HASH *vec) {
    int modulo;
    int num;
    if (elem->tipo < 0) {
        num = (elem->tipo * -1);
        modulo = num % (TAMVECTOR);
    } else {
        modulo = elem->tipo % (TAMVECTOR);
    }
    /* Intentamos añadir en la tabla */
    if (insertLastEleList(&vec->vect[modulo], elem) == ERROR) {
        printf("Error añadiendo en la lista\n");
        return ERROR;
    }
    return OK;

}

/**------------------------------------------------------------------
Nombre: buscVector
Descripcion: Comprueba existe en la tabla hash un elemento con un 
            identificador igual a id
Entrada: la tabla hash y el id a buscar
Salida: el elemento si se encuentra o NULL en caso contrario
------------------------------------------------------------------*/
ELELIST *buscVector(HASH *vec, const char* id) {
    int i;
    ELELIST *elem;
    if (vec == NULL) {
        return NULL;
    }
    for (i = 0; i < TAMVECTOR; i++) {
        if (isEmptyList(&vec->vect[i]) == FALSE) {
            elem = buscarElelist(&vec->vect[i], id);
            if (elem != NULL && !strcmp(getId(elem), id)) {
                return elem;
            }
        } else {
            continue;
        }
    }
    return NULL;
}

/**------------------------------------------------------------------
Nombre: imprimeVector
Descripcion: imprime toda la tabla hash
Entrada: tablas hash a imprimir y fichero donde imprimirla
------------------------------------------------------------------*/
void imprimeVector(FILE *f, const HASH *vec) {
    int i;
    for (i = 0; i < TAMVECTOR; i++) {
        printList(f, &(vec->vect[i]));
    }
}

