
/*includes, defines y/o typedef que necesitesis */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

/**------------------------------------------------------------------
Nombre: getNode
Descripcion: Inicializa un nodo
Salida: el nodo inicializado
------------------------------------------------------------------*/
NODE *getNode() {
    NODE *n;
    n = (NODE *) malloc(sizeof (NODE));
    if (n == NULL) {
        return NULL;
    }
    n->next = NULL;
    return n;
}

/**------------------------------------------------------------------
Nombre: freeNode
Descripcion: Libera un nodo de la lista
Entrada: la lista 
------------------------------------------------------------------*/
void freeNode(NODE *node) {
    free(node);
    node = NULL;
}

/**------------------------------------------------------------------
Nombre: iniList
Descripcion: Inicializa una lista
Entrada: la lista a inicializar
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS iniList(LIST *list) {
    if (list == NULL) {
        return ERROR;
    }
    *list = NULL;
    return OK;
}

/**------------------------------------------------------------------
Nombre: insertFirstEleList
Descripcion: inserta un elemento al principio de la lista
Entrada: un elemento y la lista donde insertarlo
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS insertFirstEleList(LIST *list, const ELELIST *pElem) {
    NODE *n;
    if (list == NULL || pElem == NULL || (n = getNode()) == NULL) {
        return ERROR;
    }
    if (iniEleList(&n->data, pElem->id, pElem->categoria, pElem->clase, pElem->tipo, pElem->tamanio, pElem->num_variables_locales, pElem->pos_var_local, pElem->num_params, pElem->pos_param) == ERROR) {
        return ERROR;
    }
    n->next = *list;
    *list = n;
    return OK;
}

/**------------------------------------------------------------------
Nombre: insertLastEleList
Descripcion: inserta un elemento al final de la lista
Entrada: un elemento y la lista donde insertarlo
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS insertLastEleList(LIST *list, const ELELIST *pElem) {
    NODE *n;
    NODE *qn;
    if (list == NULL || pElem == NULL || (n = getNode()) == NULL) {
        return ERROR;
    }
    if (iniEleList(&n->data, pElem->id, pElem->categoria, pElem->clase, pElem->tipo, pElem->tamanio, pElem->num_variables_locales, pElem->pos_var_local, pElem->num_params, pElem->pos_param) == ERROR) {
        return ERROR;
    }
    if (isEmptyList(list) == TRUE) {
        *list = n;
        return OK;
    }
    for (qn = *list; qn->next != NULL; qn = qn->next);
    qn->next = n;
    return OK;

}

/**------------------------------------------------------------------
Nombre: extractFirstEleList
Descripcion: Extrae primer elemento de la lista
Entrada: un elemento y la lista de donde extraerlo
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS extractFirstEleList(LIST *list, ELELIST *pElem) {
    NODE *n;
    if (list == NULL || pElem == NULL || isEmptyList(list) == TRUE) {
        return ERROR;
    }
    n = *list;
    iniEleList(pElem, n->data.id, n->data.categoria, n->data.clase, n->data.tipo, n->data.tamanio, n->data.num_variables_locales, n->data.pos_var_local, n->data.num_params, n->data.pos_param);
    freeEleList(&n->data);
    freeEleList(pElem);
    *list = n->next;
    freeNode(n);
    return OK;
}

/**------------------------------------------------------------------
Nombre: extractLastEleList
Descripcion: Extrae el último elemento de la lista
Entrada: un elemento y la lista de donde extraerlo
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS extractLastEleList(LIST *list, ELELIST *pElem) {
    if (list == NULL || pElem == NULL || isEmptyList(list) == TRUE) {
        return ERROR;
    } else if ((*list)->next == NULL) {
        NODE *n;
        n = *list;
        if(iniEleList(pElem, n->data.id, n->data.categoria, n->data.clase, n->data.tipo, n->data.tamanio, n->data.num_variables_locales, n->data.pos_var_local, n->data.num_params, n->data.pos_param) == ERROR){
            return ERROR;
        }
	freeEleList(pElem);
        freeList(list);
        return OK;
    } else {
        NODE *n, *aux;
        for (n = *list; n->next != NULL; n = n->next) {
            aux = n;
        }
        if(iniEleList(pElem, n->data.id, n->data.categoria, n->data.clase, n->data.tipo, n->data.tamanio, n->data.num_variables_locales, n->data.pos_var_local, n->data.num_params, n->data.pos_param) == ERROR){
            return ERROR;
        }
        freeEleList(&n->data);
	freeEleList(pElem);
        freeNode(n);
        aux->next = NULL;
        return OK;
    }
}

/**------------------------------------------------------------------
Nombre: freeList
Descripcion: Libera la lista
Entrada: la lista
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS freeList(LIST *list) {
    ELELIST pElem;
    if (list == NULL) {
        return ERROR;
    }
    while (isEmptyList(list) == FALSE) {
        extractFirstEleList(list, &pElem);
    }
    *list = NULL;
    return OK;
}

/**------------------------------------------------------------------
Nombre: isEmptyList
Descripcion: Comprueba si la lista está vacía
Entrada: la lista
Salida: TRUE si la lista está vacía y FALSE en caso contrario
------------------------------------------------------------------*/
BOOL isEmptyList(const LIST *list) {
    NODE *n;
    n = *list;
    if (n == NULL) {
        return TRUE;
    }
    return FALSE;
}

/*BUSCAR*/
ELELIST *buscarElelist(const LIST *list, const char* id) {
    NODE *n;
    if(list == NULL){
        return NULL;
    }
    for(n = *list; n != NULL; n = n->next){
        if(!strcmp(n->data.id, id)){
            return&n->data;
        }
    }
    return NULL;
}

/**------------------------------------------------------------------
Nombre: numElemsList
Descripcion: Devuelve el número de elementos de la lista
Entrada: la lista
Salida: el número de elementos de la lista
------------------------------------------------------------------*/
int numElemsList(const LIST *list) {
    NODE *n;
    int num = 0;
    if (list == NULL) {
        return -1;
    }
    for (n = *list; n != NULL; n = n->next) {
        num++;
    }
    return num;
}

/**------------------------------------------------------------------
Nombre: printList
Descripcion: imprime toda la lista
Entrada: lista a imprimir y fichero donde imprimirla
------------------------------------------------------------------*/
void printList(FILE *fd, const LIST *list) {
    NODE *n;
    for (n = *list; n != NULL; n = n->next) {
        printEleList(fd, &n->data);
    }
}
