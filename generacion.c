#include <stdio.h>
#include "generacion.h"

void segmentoBSS(FILE *generaCodigo, SYM* sym){

    int  i;
    NODE *n;

    fprintf(generaCodigo, ";DECLARACION DE VARIABLES GLOBALES SIN INICIALIZAR\n");
    fprintf(generaCodigo, "segment .bss \n");

    for(i=0; i<TAMVECTOR; i++){/*recorremos la tabla*/
        for (n = sym->global->vect[i]; n != NULL; n = n->next){
            
            if(n->data.clase == TIPO_ESCALAR){/*Si es un escalar, reservamos para tamaño 1*/
               fprintf(generaCodigo,"\t_%s resd 1\n", n->data.id);

            } else if(n->data.clase == TIPO_VECTOR) {/*Si es un vector, reservamos el tamaño recibido de la estructura ELELIST*/
               fprintf(generaCodigo, "\t_%s resd %d \n", n->data.id, n->data.tamanio);
            }
        }
    }
    fprintf(generaCodigo, "\t__esp resd 1\n");//Variable global que guardará el valor inicial de esp
}

void segmentoDATA(FILE *generaCodigo) {
    fprintf(generaCodigo, "\n;DECLARACION DE VARIABLES GLOBALES PARA ERRORES INICIALIZADAS\n");
    fprintf(generaCodigo, "segment .data\n"); //En esta sección se declaran los mensajes de error de ejecución
    fprintf(generaCodigo, "\tmensaje_1 db \"****Error de ejecucion: Indice fuera de rango.\", 0\n");
    fprintf(generaCodigo, "\tmensaje_2 db \"****Error de ejecucion: Division por cero.\", 0\n");
    fprintf(generaCodigo, "\tmensaje_3 db \"****Error de ejecucion: Exponente negativo.\", 0\n\n");
}

void segmentoTEXT(FILE *generaCodigo){
    fprintf(generaCodigo, "\n;COMIENZO DEL PROGRAMA\n");
    fprintf(generaCodigo, "segment .text\n");
    fprintf(generaCodigo, "\tglobal main\n");
    fprintf(generaCodigo, "\textern scan_int, scan_boolean\n");
    fprintf(generaCodigo, "\textern print_int, print_boolean, print_string, print_blank, print_endofline\n\n");
}

void gc_main(FILE *generaCodigo){
       
    fprintf(generaCodigo, ";------------Funcion Principal------------\n");
    fprintf(generaCodigo, "main:\n");
}

void gc_suma_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar el segundo operando en edx*/
	fprintf(generaCodigo, "\n;Cargar el segundo operando en edx\n");
	fprintf(generaCodigo, "\tpop dword edx\n");
	if (es_direccion_op2 == 1){
        fprintf(generaCodigo, "\tmov dword edx , [edx]\n");
    }
	/*Cargar el primer operando en eax*/
	fprintf(generaCodigo, ";Cargar el primer operando en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
    if (es_direccion_op1 == 1){
        fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
    }
	/*Realizar la operacion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la operacion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\tadd eax, edx\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_resta_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar el segundo operando en edx*/
	fprintf(generaCodigo, "\n;Cargar el segundo operando en edx\n");
	fprintf(generaCodigo, "\tpop dword edx\n");
	if (es_direccion_op2 == 1){
        fprintf(generaCodigo, "\tmov dword edx , [edx]\n");
    }
	/*Cargar el primer operando en eax*/
	fprintf(generaCodigo, ";Cargar el primer operando en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
    if (es_direccion_op1 == 1){
        fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
    }
	/*Realizar la operacion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la operacion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\tsub eax, edx\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_modulo_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar el divisor en ecx*/
	fprintf(generaCodigo, "\n;Cargar el divisor en ecx\n");
	fprintf(generaCodigo, "\tpop dword ecx\n");
	if (es_direccion_op2==1){
		fprintf(generaCodigo, "\tmov dword ecx , [ecx]\n");
	}
	/*Comprobacion de error, division por 0*/
	fprintf(generaCodigo, "\tcmp ecx, 0 \n");
	fprintf(generaCodigo, "\tje error_2\n");

	/*Cargar el dividendo en eax*/
	fprintf(generaCodigo, ";Cargar el dividendo en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if (es_direccion_op1==1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Extender el dividendo a la composicion de registros edx:eax*/
	fprintf(generaCodigo, "\tcdq\n");
	/*Realizar la operacion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la operacion y dejar el resto en edx\n");
	fprintf(generaCodigo, "\tidiv ecx\n");
	fprintf(generaCodigo, ";Apilar el resto\n");
	fprintf(generaCodigo, "\tpush dword edx\n");
}

void gc_division_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar el divisor en ecx*/
	fprintf(generaCodigo, "\n;Cargar el divisor en ecx\n");
	fprintf(generaCodigo, "\tpop dword ecx\n");
	if (es_direccion_op2==1){
		fprintf(generaCodigo, "\tmov dword ecx , [ecx]\n");
	}
	/*Comprobacion de error, division por 0*/
	fprintf(generaCodigo, "\tcmp ecx, 0 \n");
	fprintf(generaCodigo, "\tje error_2\n");

	/*Cargar el dividendo en eax*/
	fprintf(generaCodigo, ";Cargar el dividendo en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if (es_direccion_op1==1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Extender el dividendo a la composicion de registros edx:eax*/
	fprintf(generaCodigo, "\tcdq\n");
	/*Realizar la operacion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la operacion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\tidiv ecx\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_multiplicacion_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar un operando en ecx*/
	fprintf(generaCodigo, "\n;Cargar un operando en ecx\n");
	fprintf(generaCodigo, "\tpop dword ecx\n");
	if (es_direccion_op2==1){
		fprintf(generaCodigo, "\tmov dword ecx , [ecx]\n");
	}
	/*Cargar otro operando en eax*/
	fprintf(generaCodigo, ";Cargar otro operando en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if (es_direccion_op1==1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	fprintf(generaCodigo, "\tcdq\n");
	/*Realizar la operacion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la operacion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\timul ecx\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_exponente_enteros(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2, const int etiqueta){
	/*Cargar el exponente en ecx*/
	fprintf(generaCodigo, "\n;Cargar el exponente en ecx\n");
	fprintf(generaCodigo, "\tpop dword ecx\n");
	if(es_direccion_op2 == 1){
		fprintf(generaCodigo, "\tmov dword ecx , [ecx]\n");
	}
	/*Cargar la base en eax*/
	fprintf(generaCodigo, "\n;Cargar la base en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion_op1 == 1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}

	fprintf(generaCodigo, "\tcmp ecx, 0\n");
	fprintf(generaCodigo, "\tje potenciacero%d\n", etiqueta);
	/*Si el exponente es negativo error*/
	fprintf(generaCodigo, "\tjl error_3\n");

	fprintf(generaCodigo, "\tcmp ecx, 1\n");
	fprintf(generaCodigo, "\tje finpotencia%d\n", etiqueta);
	fprintf(generaCodigo, "\tmov dword ebx, eax\n");
	
	fprintf(generaCodigo, "potencia%d:\n", etiqueta);
	fprintf(generaCodigo, "\timul ebx\n");
	fprintf(generaCodigo, "\tdec ecx\n");
	fprintf(generaCodigo, "\tcmp ecx, 1\n");
	fprintf(generaCodigo, "\tjne potencia%d\n", etiqueta);
	fprintf(generaCodigo, "\tje finpotencia%d\n", etiqueta);

	fprintf(generaCodigo, "potenciacero%d:\n", etiqueta);
	fprintf(generaCodigo, "\tmov eax, 1\n");
	fprintf(generaCodigo, "finpotencia%d:\n", etiqueta);
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_negacion_enteros(FILE *generaCodigo, const int es_direccion_op1){
	/*Cargar el operando en eax*/
	fprintf(generaCodigo, "\n;Cargar el operando en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion_op1 == 1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Realizar la negacion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la negacion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\tneg eax\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_and_booleans(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar el segundo operando en edx*/
	fprintf(generaCodigo, "\n;Cargar el segundo operando en edx\n");
	fprintf(generaCodigo, "\tpop dword edx\n");
	if (es_direccion_op2 == 1){
	fprintf(generaCodigo, "\tmov dword edx , [edx]\n");
	}
	/*Cargar el primer operando en eax*/
	fprintf(generaCodigo, ";Cargar el primer operando en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion_op1 == 1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Realizar la conjuncion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la conjuncion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\tand eax , edx\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_or_booleans(FILE *generaCodigo, const int es_direccion_op1, const int es_direccion_op2){
	/*Cargar el segundo operando en edx*/
	fprintf(generaCodigo, "\n;Cargar el segundo operando en edx\n");
	fprintf(generaCodigo, "\tpop dword edx\n");
	if (es_direccion_op2 == 1){
	fprintf(generaCodigo, "\tmov dword edx , [edx]\n");
	}
	/*Cargar el primer operando en eax*/
	fprintf(generaCodigo, ";Cargar el primer operando en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion_op1 == 1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Realizar la conjuncion y dejar el resultado en eax*/
	fprintf(generaCodigo, ";Realizar la conjuncion y dejar el resultado en eax\n");
	fprintf(generaCodigo, "\tor eax , edx\n");
	fprintf(generaCodigo, ";Apilar el resultado\n");
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_negacion_booleans(FILE *generaCodigo, const int es_direccion, const int etiqueta){
	/*Cargar el operando en eax*/
	fprintf(generaCodigo, "\n;Cargar el operando en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Ver si eax es 0 y en ese caso saltar a negar_falso*/
	fprintf(generaCodigo, ";Ver si eax es 0 y en ese caso saltar a negar_falso\n");
	fprintf(generaCodigo, "\tor eax , eax\n");
	fprintf(generaCodigo, "\tjz near negar_falso%d\n", etiqueta);
	/*Cargar 0 en eax (negacion de verdadero) y saltar al final*/
	fprintf(generaCodigo, ";Cargar 0 en eax (negacion de verdadero) y saltar al final\n");
	fprintf(generaCodigo, "\tmov dword eax,0\n");
	fprintf(generaCodigo, "\tjmp near fin_negacion%d\n", etiqueta);
	/*Cargar 1 en eax (negacion de falso)*/
	fprintf(generaCodigo, ";Cargar 1 en eax (negacion de falso)\n");
	fprintf(generaCodigo, "\nnegar_falso%d:\nmov dword eax,1\n", etiqueta);
	/*Apilar eax*/
	fprintf(generaCodigo, ";Apilar eax\n");
	fprintf(generaCodigo, "\nfin_negacion%d:\npush dword eax\n", etiqueta);
}

void gc_comparacion_igual(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tje near igual%d\n", etiqueta);
	fprintf(generaCodigo,"\tpush dword 0\n");
	fprintf(generaCodigo,"\tjmp near fin_igual%d\n", etiqueta);
	fprintf(generaCodigo,"\nigual%d:\n\tpush dword 1\n", etiqueta);
	fprintf(generaCodigo,"\nfin_igual%d:\n", etiqueta);
}

void gc_comparacion_distinto(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tjne near distinto%d\n", etiqueta);
	fprintf(generaCodigo,"\tpush dword 0\n");
	fprintf(generaCodigo,"\tjmp near fin_distinto%d\n", etiqueta);
	fprintf(generaCodigo,"\ndistinto%d:\n\npush dword 1\n", etiqueta);
	fprintf(generaCodigo,"\nfin_distinto%d:\n", etiqueta);
}

void gc_comparacion_menorigual(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tjle near menorigual%d\n", etiqueta);
	fprintf(generaCodigo,"\tpush dword 0\n");
	fprintf(generaCodigo,"\tjmp near fin_menorigual%d\n", etiqueta);
	fprintf(generaCodigo,"\nmenorigual%d:\n\tpush dword 1\n", etiqueta);
	fprintf(generaCodigo,"\nfin_menorigual%d:\n", etiqueta);
}

void gc_comparacion_mayorigual(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tjge near mayorigual%d\n", etiqueta);
	fprintf(generaCodigo,"\tpush dword 0\n");
	fprintf(generaCodigo,"\tjmp near fin_mayorigual%d\n", etiqueta);
	fprintf(generaCodigo,"\nmayorigual%d:\n\tpush dword 1\n", etiqueta);
	fprintf(generaCodigo,"\nfin_mayorigual%d:\n", etiqueta);
}

void gc_comparacion_menor(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tjl near menor%d\n", etiqueta);
	fprintf(generaCodigo,"\tpush dword 0\n");
	fprintf(generaCodigo,"\tjmp near fin_menor%d\n", etiqueta);
	fprintf(generaCodigo,"\nmenor%d:\n\tpush dword 1\n", etiqueta);
	fprintf(generaCodigo,"\nfin_menor%d:\n", etiqueta);
}

void gc_comparacion_mayor(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tjg near mayor%d\n", etiqueta);
	fprintf(generaCodigo,"\tpush dword 0\n");
	fprintf(generaCodigo,"\tjmp near fin_mayor%d\n", etiqueta);
	fprintf(generaCodigo,"\nmayor%d:\n\tpush dword 1\n", etiqueta);
	fprintf(generaCodigo,"\nfin_mayor%d:\n", etiqueta);
}

void gc_comprobacion_vector(FILE *generaCodigo, const char *lex, const int es_direccion, const int tam, const int param){
	/*Carga del valor del indice en eax*/
	fprintf(generaCodigo, "\n;Carga del valor del indice en eax\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
	}
	/*Si el indice es menor que 0, error en tiempo de ejecucion*/
	fprintf(generaCodigo, ";Si el indice es menor que 0, error en tiempo de ejecucion\n");
	fprintf(generaCodigo, "\tcmp eax,0\n");
	fprintf(generaCodigo, "\tjl near error_1\n");
	/*Si el indice es mayor de lo permitido, error en tiempo de ejecucion*/
	fprintf(generaCodigo, ";Si el indice es mayor de lo permitido, error en tiempo de ejecucion\n");
	fprintf(generaCodigo, "\tcmp eax, %d\n", tam-1);
	fprintf(generaCodigo, "\tjg near error_1\n");
	/*Cargar en edx la direccion de inicio del vector*/
	fprintf(generaCodigo, ";Cargar en edx la direccion de inicio del vector\n");
	fprintf(generaCodigo, "\tmov dword edx,  _%s\n", lex);
	/*Cargar en eax la direccion del elemento indexado*/
	fprintf(generaCodigo, ";Cargar en eax la direccion del elemento indexado\n");
	fprintf(generaCodigo, "\tlea eax, [edx+eax*4]\n");
	/*Apilar la direccion del elemento indexado*/
	fprintf(generaCodigo, ";Apilar la direccion del elemento indexado\n");
	if(param==0){
		fprintf(generaCodigo, "\tpush dword eax\n");
	}else {
		fprintf(generaCodigo, "\tpush dword [eax]\n");
	}
}

void gc_incremento_identificador(FILE *generaCodigo, const char *lex, const int es_direccion){
    /*Cargar en eax la parte derecha de la asignacion*/
	fprintf(generaCodigo, "\n;Cargar en eax la parte derecha de la asignacion\n");
	fprintf(generaCodigo, "\tpop dword eax\n");

    if (es_direccion == 1){
        fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
    }
    /*Hacer el incremento efectivo*/
    fprintf(generaCodigo, ";Hacer el incremento efectivo\n");
    fprintf(generaCodigo, "\tmov dword edx, [_%s]\n", lex);
    fprintf(generaCodigo, "\tadd edx, eax\n");
    fprintf(generaCodigo, "\tmov dword [_%s] , edx\n", lex);
}

void gc_asignacion_identificador(FILE *generaCodigo, const char *lex, const int es_direccion){
    /*Cargar en eax la parte derecha de la asignacion*/
	fprintf(generaCodigo, "\n;Cargar en eax la parte derecha de la asignacion\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
    if (es_direccion == 1){
        fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
    }
    /*Hacer la asignacion efectiva*/
	fprintf(generaCodigo, ";Hacer la asignacion efectiva\n");
    fprintf(generaCodigo, "\tmov dword [_%s] , eax\n", lex);
}

void gc_inicializar_vector(FILE *generaCodigo, const int es_direccion, const char *lex, const int inicializados){
	fprintf (generaCodigo, "\n;Cargamos el valor del vector en eax\n");
	fprintf (generaCodigo, "\tpop dword eax\n");
	if (es_direccion == 1) {
          fprintf (generaCodigo, "\tmov dword eax, [eax]\n");
  	}
  
  	fprintf (generaCodigo, "\tmov dword edx, _%s\n", lex);
  	fprintf (generaCodigo, "\tlea ebx, [edx + %d*4]\n", inicializados); 
  	fprintf (generaCodigo, "\tmov dword [ebx] , eax\n");
}

void gc_incremento_vector(FILE *generaCodigo, const int es_direccion){
	/*Cargar en eax la parte izquierda de la asignacion*/
	fprintf(generaCodigo, "\n;Cargar en eax la parte izquierda de la asignacion\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax, [eax]\n");
	}
	/*Incremento*/
	fprintf(generaCodigo, ";Incremento\n");	
	fprintf(generaCodigo, "\tmov dword edx, [eax]\n");
	fprintf(generaCodigo, "\tadd edx , ecx\n");
	/*Hacer la asignacion efectiva*/
	fprintf(generaCodigo, ";Hacer la asignacion efectiva\n");
	fprintf(generaCodigo, "\tmov dword [eax] , edx\n");
}

void gc_asignacion_vector(FILE *generaCodigo, const int es_direccion){
	/*Cargar en eax la parte derecha de la asignacion*/
	fprintf(generaCodigo, "\n;Cargar en eax la parte derecha de la asignacion\n");
	fprintf(generaCodigo, "\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax, [eax]\n");
	}
	/*Cargar en edx la parte izquierda de la asignacion*/
	fprintf(generaCodigo, ";Cargar en edx la parte izquierda de la asignacion\n");
	fprintf(generaCodigo, "\tpop dword edx\n");
	/*Hacer la asignacion efectiva*/
	fprintf(generaCodigo, ";Hacer la asignacion efectiva\n");
    fprintf(generaCodigo, "\tmov dword [edx] , eax\n");
}

void gc_scanf(FILE *generaCodigo, const char *lex, const int tipo){
	fprintf(generaCodigo, "\n;Lectura\n");
    	fprintf(generaCodigo, "\tpush dword _%s\n", lex);
	if(tipo==TIPO_LOGICO){
		fprintf(generaCodigo, "\tcall scan_boolean\n");
	}else if(tipo==TIPO_INT){
		fprintf(generaCodigo, "\tcall scan_int\n");
	}
	fprintf(generaCodigo, "\tadd esp, 4\n");
}

void gc_printf(FILE *generaCodigo, const char *lex, const int es_direccion, const int tipo){
    fprintf(generaCodigo, "\n;Escritura\n");
    /*Acceso al valor de exp si es distinto de constante*/
	fprintf(generaCodigo, ";Acceso al valor de exp si es distinto de constante\n");
    if(es_direccion == 1){ 
		fprintf(generaCodigo, "\tpop dword eax\n");
        	fprintf(generaCodigo, "\tmov dword eax , [eax]\n");
		fprintf(generaCodigo, "\tpush dword eax\n");
	}
	/*Si la expresion es de tipo entero, real o logico*/
	fprintf(generaCodigo, ";Si la expresion es de tipo entero, real o logico\n");
    if(tipo==TIPO_LOGICO){	
       fprintf(generaCodigo, "\tcall print_boolean ;llamar a la funcion de la libreria alfalib.o\n");
    }else if(tipo == TIPO_INT){
       fprintf(generaCodigo, "\tcall print_int ;llamar a la funcion de la libreria alfalib.o\n");
    }
    /*Restauracion del puntero de pila*/
	fprintf(generaCodigo, ";Restauracion del puntero de pila\n");
    fprintf(generaCodigo, "\tadd esp, 4 \n");
    /*Salto de linea*/
	fprintf(generaCodigo, ";Salto de linea\n");
    fprintf(generaCodigo, "\tcall print_endofline\n");
}

void gc_abrir_comparacion(FILE *generaCodigo, const int es_direccion_exp1, const int es_direccion_exp2, const int etiqueta){
	fprintf(generaCodigo, "\n;Comparacion");
	/*Cargar la segunda expresion en edx*/
	fprintf(generaCodigo, "\n;Cargar la segunda expresion en edx\n");
	fprintf(generaCodigo,"\tpop dword edx\n");
	if(es_direccion_exp2 == 1){
		fprintf(generaCodigo,"\tmov dword edx , [edx]\n");
	}
	/*Cargar la primera expresion en eax*/
	fprintf(generaCodigo, ";Cargar la primera expresion en eax\n");
	fprintf(generaCodigo,"\tpop dword eax\n");
	if(es_direccion_exp1 == 1){
		fprintf(generaCodigo,"\tmov dword eax , [eax]\n");
	}
	/*Comparar y apilar el resultado*/
	fprintf(generaCodigo, ";Comparar y apilar el resultado\n");
	fprintf(generaCodigo,"\tcmp eax, edx\n");
	fprintf(generaCodigo,"\tjl near less%d\n", etiqueta);
	fprintf(generaCodigo,"\tje near equal%d\n", etiqueta);
	fprintf(generaCodigo,"\tjg near greater%d\n", etiqueta);
	fprintf(generaCodigo,"\tjmp near fin_compara%d\n", etiqueta);
	fprintf(generaCodigo,"\nless%d:", etiqueta);
}

void gc_cerrar_comparacion(FILE *generaCodigo, const int etiqueta){
	fprintf(generaCodigo, "\nfin_compara%d:\n", etiqueta);
}

void gc_abrir_if(FILE *generaCodigo, const int es_direccion, const int etiqueta){
    fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion==1){
		fprintf(generaCodigo, "\tmov eax, [eax]\n");
	}
	fprintf(generaCodigo, "\tcmp eax, 0\n");
	fprintf(generaCodigo, "\tje near fin_si%d\n", etiqueta);
}

void gc_cerrar_if(FILE *generaCodigo, const int etiqueta){
	fprintf(generaCodigo, "\nfin_si%d:\n", etiqueta);
}

void gc_if_exp_sentencias(FILE *generaCodigo, const int etiqueta){
	fprintf(generaCodigo, "\n\tjmp near fin_sino%d\n", etiqueta);
	fprintf(generaCodigo, "\nfin_si%d:\n", etiqueta);
}

void gc_cerrar_else(FILE *generaCodigo, const int etiqueta){
	fprintf(generaCodigo, "\nfin_sino%d:\n", etiqueta);
}

void gc_abrir_while(FILE *generaCodigo, const int etiqueta){
	fprintf(generaCodigo, "\ninicio_while%d:\n", etiqueta);
}

void gc_while_exp(FILE *generaCodigo, const int es_direccion, const int etiqueta){
	fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion==1){
		fprintf(generaCodigo, "\tmov eax, [eax]\n");
	}
	fprintf(generaCodigo, "\tcmp eax, 0\n");
	fprintf(generaCodigo, "\tje near fin_while%d\n", etiqueta);
}

void gc_cerrar_while(FILE *generaCodigo, const int etiqueta){
	fprintf(generaCodigo, "\n\n\tjmp near inicio_while%d\n", etiqueta);
	fprintf(generaCodigo, "\nfin_while%d:\n", etiqueta);
}

void gc_fin_programa(FILE *generaCodigo){
    fprintf(generaCodigo, "\n\tjmp near fin\n");

	/*Etiquetas sobre errores de ejecucion*/
    funcionErrores(generaCodigo);

    fprintf(generaCodigo, "\nfin:\n\tret\n");
}

void gc_llamadas_funciones(FILE *generaCodigo, const char *lex, const int varLocal){
	fprintf(generaCodigo, "\n\tcall _%s\n", lex);
	fprintf(generaCodigo, "\tadd esp, 4*%d\n", varLocal);
	fprintf(generaCodigo, "\tpush dword eax\n");
}

void gc_iniCuerpo_funcion (FILE *generaCodigo, const char *lex, const int numVar){
	fprintf(generaCodigo, "\n_%s:\n", lex);
	fprintf(generaCodigo, "\tpush ebp\n");
	fprintf(generaCodigo, "\tmov ebp, esp\n");
	fprintf(generaCodigo, "\tsub esp, 4*%d\n", numVar);

}

void gc_retorno_funcion (FILE *generaCodigo, const int es_direccion){
	fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion==1){
		fprintf(generaCodigo, "\tmov eax, [eax]\n");
	}
	gc_finCuerpo_funcion(generaCodigo);
}

void gc_finCuerpo_funcion (FILE *generaCodigo){
	fprintf(generaCodigo, "\n\tmov dword esp, ebp\n");
	fprintf(generaCodigo, "\tpop dword ebp\n");
	fprintf(generaCodigo, "\tret\n");
}

void gc_expresion_parametro(FILE *generaCodigo, const int numPar, const int posPar){
	fprintf(generaCodigo, "\n;lea eax, [ebp+4+4*%d], eax\n", numPar - posPar);

	int valorPar;
	valorPar = 4 + 4*(numPar - posPar);
	fprintf(generaCodigo, "\tlea eax, [ebp + %d]\n", valorPar);
	fprintf(generaCodigo, "\tpush dword [eax]\n");
}

void gc_lectura_parametro(FILE *generaCodigo, const int tipo, const int numPar, const int posPar){
	fprintf(generaCodigo, "\n;lea eax, [ebp+4+4*%d], eax\n", numPar - posPar);
	int valorPar;
	valorPar = 4 + 4*(numPar - posPar);

	fprintf(generaCodigo, "\tlea eax, [ebp + %d]\n", valorPar);
	fprintf(generaCodigo, "\tpush dword eax\n");
	if(tipo == TIPO_LOGICO){
		fprintf(generaCodigo, "\tcall scan_boolean\n");
	}else if(tipo == TIPO_INT){
		fprintf(generaCodigo, "\tcall scan_int\n");
	}
	fprintf(generaCodigo, "\tadd esp, 4\n");
}

void gc_incremento_parametro(FILE *generaCodigo, const int es_direccion, const int numPar, const int posPar){
	fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax, [eax]\n");
	}

	int valorPar;
	valorPar = 4 + 4*(numPar - posPar);
	fprintf(generaCodigo, "\tmov edx, [ebp + %d]\n", valorPar);
	fprintf(generaCodigo, "\tadd edx, eax\n");
	fprintf(generaCodigo, ";mov [ebp+4+4*%d], edx\n", numPar - posPar);
	fprintf(generaCodigo, "\tmov [ebp + %d],edx\n", valorPar);
}

void gc_asignacion_parametro(FILE *generaCodigo, const int es_direccion, const int numPar, const int posPar){
	fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax, [eax]\n");
	}
	fprintf(generaCodigo, ";mov dword [ebp+4+4*%d], eax\n", numPar - posPar);

	int valorPar;
	valorPar = 4 + 4*(numPar - posPar);
	fprintf(generaCodigo, "\tmov dword [ebp + %d],eax\n", valorPar);
}

void gc_localizacion_parametro(FILE *generaCodigo, const int numPar, const int posPar){
	int valorPar;
	valorPar = 4 + 4*(numPar - posPar);
	fprintf(generaCodigo, "[ebp + %d]\n", valorPar);
}

void gc_expresion_local(FILE *generaCodigo, const int posVar){

	int valorVar;
	valorVar = 4*posVar;

	fprintf(generaCodigo, "\n;lea eax, [ebp-4*%d], eax\n", posVar);
	fprintf(generaCodigo, "\tlea eax, [ebp - %d]\n", valorVar);
	fprintf(generaCodigo, "\tpush dword [eax]\n");
}

void gc_lectura_local(FILE *generaCodigo, const int tipo, const int posVar){

	int valorVar;
	valorVar = 4*posVar;

	fprintf(generaCodigo, "\n;lea eax, [ebp-4*%d], eax\n", posVar);
	fprintf(generaCodigo, "\tlea eax, [ebp - %d]\n", valorVar);
	fprintf(generaCodigo, "\tpush dword eax\n");
	if(tipo == TIPO_LOGICO){
		fprintf(generaCodigo, "\tcall scan_boolean\n");
	}else if(tipo == TIPO_INT){
		fprintf(generaCodigo, "\tcall scan_int\n");
	}
	fprintf(generaCodigo, "\tadd esp, 4\n");
}

void gc_incremento_local(FILE *generaCodigo, const int es_direccion, const int posVar){
	fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax, [eax]\n");
	}

	int valorVar;
	valorVar = 4*posVar;
	fprintf(generaCodigo, "\tmov dword edx, [ebp - %d]\n", valorVar);
	fprintf(generaCodigo, "\tadd edx, eax\n");
	fprintf(generaCodigo, ";mov dword [ebp-4*%d], edx\n", posVar);
	fprintf(generaCodigo, "\tmov dword [ebp - %d], edx\n", valorVar);
}

void gc_asignacion_local(FILE *generaCodigo, const int es_direccion, const int posVar){
	fprintf(generaCodigo, "\n\tpop dword eax\n");
	if(es_direccion == 1){
		fprintf(generaCodigo, "\tmov dword eax, [eax]\n");
	}

	int valorVar;
	valorVar = 4*posVar;

	fprintf(generaCodigo, ";mov dword [ebp-4*%d], eax\n", posVar);
	fprintf(generaCodigo, "\tmov dword [ebp - %d], eax\n", valorVar);
}

void gc_localizacion_locales(FILE *generaCodigo, const int posVar){
	int valorVar;
	valorVar = 4*posVar;
	fprintf(generaCodigo, "[ebp - %d]", valorVar);
}

/*---------------------------------------------------------------------------------*/

void funcionErrores(FILE *generaCodigo){
    /*Error por: Indice fuera de rango*/
    fprintf(generaCodigo, "\n\nerror_1:\n\tpush dword mensaje_1\n");
    fprintf(generaCodigo, "\tcall print_string\n");
    fprintf(generaCodigo, "\tadd esp, 4\n");
    fprintf(generaCodigo, "\ncall print_endofline\n");
    fprintf(generaCodigo, "\tjmp near fin\n");

    /*Error por: division por 0*/
    fprintf(generaCodigo, "error_2:\n\tpop eax\n\tpush dword mensaje_2\n");
    fprintf(generaCodigo, "\tcall print_string\n");
    fprintf(generaCodigo, "\tadd esp, 4\n");
    fprintf(generaCodigo, "\ncall print_endofline\n");
    fprintf(generaCodigo, "\tjmp near fin\n");

    /*Error por: exponente negativo*/
    fprintf(generaCodigo, "error_3:\n\tpush dword mensaje_3\n");
    fprintf(generaCodigo, "\tcall print_string\n");
    fprintf(generaCodigo, "\tadd esp, 4\n");
    fprintf(generaCodigo, "\ncall print_endofline\n");
    fprintf(generaCodigo, "\tjmp near fin\n");
}
