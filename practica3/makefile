#-----------------------
# Se generan los ejecutables de los ejercicios 2, 5 y 6
# Realizado por: Víctor García victor.garciacarrera@estudiante.uam.es Alfonso Carvajal alfonso.carvajal@estudiante.uam.es
#-----------------------

CC = gcc
CFLAGS = -g -w -std=c99
EXE = ejercicio2

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

$(EXE) : % : %.o semaforos.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o semaforos.o
