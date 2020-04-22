EXE=alfa
DELETE = *~ core *.yy.* *.tab.* y.output generacionNasm*.*
NASM = nasm
EXE_NASM = generacionNasm

all: clean $(EXE) entrega autores ayuda

y.tab.c:
	@echo " > Generado BISON"
	@ bison -dy alfa.y

lex.yy.c:		
	@ echo " "
	@tput setaf 3
	@echo "~~~ GENERANDO FICHEROS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0
	@echo " > Generando FLEX"
	@ flex alfa.l

$(EXE): lex.yy.c y.tab.c 
	@echo " > Generado C"
	@tput setaf 3
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@ gcc -Wall -o alfa *.c

$(NASM): clear ayuda	
	@tput setaf 3
	@echo "~~~ GENERANDO FICHEROS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0
	@echo " > Generado NASM"	
	@tput setaf 3
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0
	@ $(NASM) -g -o $(EXE_NASM).o -f elf32 $(EXE_NASM).asm
	@ gcc -m32 -o $(EXE_NASM) $(EXE_NASM).o alfalib.o

clean:	clear
	@ rm -f $(EXE) $(DELETE) $(EXE_NASM)
clear:
	@ clear

entrega:	
	@ echo " "
	@tput setaf 6
	@echo "~~~ ENTREGA ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0
	@echo " Análisis Semántico y generacion de código"
	@tput setaf 6
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0

autores:
	@ echo " "
	@tput setaf 2
	@echo "~~~ AUTORES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"	
	@tput sgr0
	@echo " "
	@echo " "
	@echo " Pareja 11"
	@tput setaf 2
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0

ayuda:
	@ echo " "
	@tput setaf 1
	@echo "~~~ AYUDA ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0
	@echo " Para generar el fichero en ensamblador: "
	@echo "	./alfa <nombre fichero entrada> generacionNasm.asm"
	@ echo " "
	@echo " Para compilar el fichero en ensamblador: "
	@echo "	make nasm"
	@ echo " "
	@echo " Para ejecutar el fichero en ensamblador: "
	@echo "	./generacionNasm"
	@ echo " "
	@echo " Para limpiar: "
	@echo "	make clean"
	@tput setaf 1
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@tput sgr0
	@ echo " "

































