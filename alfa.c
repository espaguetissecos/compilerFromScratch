#include <stdio.h>
#include "hash.h"

extern int yyparse();

FILE* generaCodigo; //Fichero de sal
extern FILE* yyin;
extern FILE* yyout; //Fichero de salida sintáctica


int main(int argc, char **argv) {
  if (argc != 3) { //Comprobación del número de argumentos
    puts("Número de parámetros inválido.");
    puts("\tModo de ejecución: ./a.out <nombre del fichero alfa de entrada> <nombre del fichero nasm de salida>");
    return ERROR;
  }

  yyin = fopen(argv[1], "r"); //Abrir el fichero de entrada para lectura
  if (yyin == NULL) {
    puts("Error al abrir el fichero de entrada");
    return ERROR;
  }
  generaCodigo = fopen(argv[2], "w"); //Abrir el fichero de salida para generación de código
  if (generaCodigo == NULL) {
    puts("Error al crear el fichero de salida para generación de código");
    return ERROR;
  }
  yyout = fopen("sintactico.txt", "w"); //Abrir el fichero de salida sintáctica
  if (yyout == NULL) {
    puts("Error al crear el fichero de salida sintáctica");
    return ERROR;
  }

  yyparse();

  fclose(yyin); //Cerrar el fichero de entrada
  fclose(generaCodigo); //Cerrar el fichero de salida
  fclose(yyout); //Cerrar el fichero de salida
  return OK;
}
