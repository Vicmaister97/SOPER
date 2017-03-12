#-----------------------
# Se generan los ejecutables de los ejercicios 4(apartados a,b), 5(apartados a,b), 6, 8 y 9 de la forma e4a, e4b, e5a, e5b, e6, e8 y e9
# Realizado por: Víctor García victor.garciacarrera@estudiante.uam.es Alfonso Carvajal alfonso.carvajal@estudiante.uam.es
#-----------------------

CC = gcc -ansi -pedantic
CFLAGS = -Wall -g -std=c99
EXE = e4a e4b e5a e5b e6 e8 e9

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
