# Ejecutar con valgrind
valgrind: abb_app
	valgrind --leak-check=full --track-origins=yes ./abb_app ../TP1-ENUNCIADO/pokedex.csv cantidad
CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -Wconversion -Wtype-limits -pedantic -Werror -O0 -g
INCLUDES = -Isrc -I.

# Fuentes de la biblioteca (sin mains)
SRCS = src/abb.c src/tp1.c

# Ejecutables
APP_BIN  = abb_app
TEST_BIN = pruebas_alumno

.PHONY: all clean

all: $(APP_BIN) $(TEST_BIN)

$(APP_BIN): main.c $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(TEST_BIN): pruebas_alumno.c $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -f $(APP_BIN) $(TEST_BIN) *.o src/*.o

# Helpers
.PHONY: run-tests run-app valgrind-tests valgrind-app

run-tests: $(TEST_BIN)
	./$(TEST_BIN)

# CSV por defecto, se puede sobreescribir: make run-app CSV=../TP1-ENUNCIADO/pokedex.csv
CSV ?= ../TP1-ENUNCIADO/pokedex.csv
run-app: $(APP_BIN)
	./$(APP_BIN) $(CSV)

valgrind-tests: $(TEST_BIN)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BIN)

valgrind-app: $(APP_BIN)
	valgrind --leak-check=full --show-leak-kinds=all ./$(APP_BIN) $(CSV)

# Ejecutar (alias simples)
.PHONY: run

run: run-tests
