#-----------------------
# Se generan los ejecutables de los ejercicios 4(apartados a,b), 5(apartados a,b), 6, 8 y 9 de la forma e4a, e4b, e5a, e5b, e6, e8 y e9
# Realizado por: Víctor García victor.garciacarrera@estudiante.uam.es Alfonso Carvajal alfonso.carvajal@estudiante.uam.es
#-----------------------

CC = gcc
CFLAGS = -g -Wall -w -pedantic -std=c99
EXE = e1 e2 e3a e3b e4 ejercicio6 ejercicio8 ejercicio10

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

%.o: %.c *.h
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

$(EXE) : % : %.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o
