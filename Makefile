CC=gcc
CSTD=-std=c99
WARN=-Wall -Wextra -Wconversion -Wtype-limits -pedantic -Werror
DBG=-O0 -g
CFLAGS=$(CSTD) $(WARN) $(DBG)
INCLUDES=-I./src -I.

# Binario de pruebas provisto por la cátedra
SRC=src/abb.c src/pruebas_alumno.c
OBJ=$(SRC:.c=.o)
BIN=pruebas_abb

# Binario de la app pedida en el enunciado
APP_SOURCES=src/abb.c src/tp1.c src/main.c
APP_OBJECTS=$(APP_SOURCES:.c=.o)
APP_BIN=abb_app

all: $(BIN) $(APP_BIN)

$(BIN): CFLAGS+= -DPRUEBAS_ALUMNO
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(APP_BIN): CFLAGS+= -DABB_APP
$(APP_BIN): $(APP_OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(BIN)
	./$(BIN)

run-app: $(APP_BIN)
	./$(APP_BIN) ../TP1-ENUNCIADO/pokedex.csv


valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN)

# Ejemplo: correr solo una función de test bajo valgrind (requiere soporte en el runner)

clean:
	rm -f $(BIN) $(APP_BIN) src/*.o
