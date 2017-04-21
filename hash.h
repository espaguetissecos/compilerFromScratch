#ifndef hash
#define hash

#include "list.h"

#define TAMVECTOR 11 /* Tamaño para la tabla */

typedef struct {
	LIST vect[TAMVECTOR];
} HASH;

/**------------------------------------------------------------------
Nombre: creaVector
Descripcion: Inicializa una tabla hash
Salida: la tabla inicializada
------------------------------------------------------------------*/
HASH *creaVector();

/**------------------------------------------------------------------
Nombre: borraVector
Descripcion: Libera la tabla hash
Entrada: puntero a la tabla hash
------------------------------------------------------------------*/
void borraVector(HASH *vec);

/**------------------------------------------------------------------
Nombre: insVector
Descripcion: inserta un elemento en la tabla hash
Entrada: un elemento y la tabla hash donde insertarlo
Salida: ERR si ha habido error o OK si ha ido bien
------------------------------------------------------------------*/
STATUS insVector(ELELIST *elem, HASH *vec);

/**------------------------------------------------------------------
Nombre: buscVector
Descripcion: Comprueba existe en la tabla hash un elemento con un 
			identificador igual a id
Entrada: la tabla hash y el id a buscar
Salida: el elemento si se encuentra o NULL en caso contrario
------------------------------------------------------------------*/
ELELIST *buscVector(HASH *lista, const char* id);

/**------------------------------------------------------------------
Nombre: imprimeVector
Descripcion: imprime toda la tabla hash
Entrada: tablas hash a imprimir y fichero donde imprimirla
------------------------------------------------------------------*/
void imprimeVector(FILE *f, const HASH *vec);


#endif
