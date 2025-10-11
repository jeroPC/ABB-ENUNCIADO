#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "tp1.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp_pokemon_por_id(const void *a, const void *b)
{
	const struct pokemon *pa = (const struct pokemon *)a;
	const struct pokemon *pb = (const struct pokemon *)b;
	return (pa->id > pb->id) - (pa->id < pb->id);
}

static int cmp_pokemon_por_nombre(const void *a, const void *b)
{
	const struct pokemon *pa = (const struct pokemon *)a;
	const struct pokemon *pb = (const struct pokemon *)b;
	return strcmp(pa->nombre, pb->nombre);
}

static int es_espacio(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
		c == '\f');
}

static char *recortar_espacios(char *cadena)
{
	if (!cadena)
		return cadena;

	char *inicio = cadena;
	while (*inicio && es_espacio(*inicio))
		inicio++;
	if (*inicio == '\0')
		return inicio;

	char *fin = inicio + strlen(inicio) - 1;
	while (fin > inicio && es_espacio(*fin)) {
		*fin = '\0';
		fin--;
	}
	return inicio;
}

static void imprimir_pokemon(const struct pokemon *p)
{
	if (!p) {
		printf("No encontrado\n");
		return;
	}
	printf("id=%d, nombre=%s, tipo=%d, atk=%d, def=%d, vel=%d\n", p->id,
	       p->nombre ? p->nombre : "", (int)p->tipo, p->ataque, p->defensa,
	       p->velocidad);
}

enum tipo_comando {
	COM_SALIR,
	COM_CANTIDAD,
	COM_ID,
	COM_NOMBRE,
	COM_DESCONOCIDO
};

static enum tipo_comando parsear_comando(char *linea, char **argumentos_out)
{
	char *comando = recortar_espacios(linea);
	char *cursor = comando;
	while (*cursor && !es_espacio(*cursor))
		cursor++;
	if (*cursor) {
		*cursor = '\0';
		cursor++;
	}
	if (argumentos_out)
		*argumentos_out = recortar_espacios(cursor);

	if (strcmp(comando, "salir") == 0)
		return COM_SALIR;
	if (strcmp(comando, "cantidad") == 0)
		return COM_CANTIDAD;
	if (strcmp(comando, "id") == 0)
		return COM_ID;
	if (strcmp(comando, "nombre") == 0)
		return COM_NOMBRE;
	return COM_DESCONOCIDO;
}

static int parsear_entero(const char *texto, int *valor_out)
{
	if (!texto || !*texto || !valor_out)
		return 0;
	int signo = 1;
	size_t i = 0;
	if (texto[0] == '+' || texto[0] == '-') {
		signo = (texto[0] == '-') ? -1 : 1;
		i = 1;
	}
	if (texto[i] == '\0')
		return 0;

	long acumulado = 0;
	for (; texto[i] != '\0'; i++) {
		char c = texto[i];
		if (c < '0' || c > '9')
			return 0;
		acumulado = acumulado * 10 + (c - '0');
		if (acumulado > (long)INT_MAX + (signo < 0))
			return 0;
	}
	long resultado = signo * acumulado;
	if (resultado < INT_MIN || resultado > INT_MAX)
		return 0;
	*valor_out = (int)resultado;
	return 1;
}

static int ejecutar_comando(enum tipo_comando tipo, char *argumentos, tp1_t *tp,
			    abb_t *arbol_por_id, abb_t *arbol_por_nombre)
{
	switch (tipo) {
	case COM_SALIR:
		return 1;
	case COM_CANTIDAD:
		printf("%zu\n", tp1_cantidad(tp));
		break;
	case COM_ID: {
		int id = 0;
		if (!parsear_entero(argumentos, &id)) {
			printf("Entrada invalida\n");
		} else {
			struct pokemon clave = { 0 };
			clave.id = id;
			struct pokemon *encontrado =
				abb_buscar(arbol_por_id, &clave);
			imprimir_pokemon(encontrado);
		}
		break;
	}
	case COM_NOMBRE: {
		if (!argumentos || *argumentos == '\0') {
			printf("Entrada invalida\n");
		} else {
			struct pokemon clave = { 0 };
			clave.nombre = argumentos;
			struct pokemon *encontrado =
				abb_buscar(arbol_por_nombre, &clave);
			imprimir_pokemon(encontrado);
		}
		break;
	}
	default:
		printf("Comando no reconocido. Escriba 'id <n>', 'nombre <s>' o 'salir'.\n");
		break;
	}
	return 0;
}

static int leer_linea(char **buffer, size_t *capacidad)
{
	ssize_t leidos = getline(buffer, capacidad, stdin);
	if (leidos == -1)
		return 0;
	if (leidos > 0 && (*buffer)[leidos - 1] == '\n')
		(*buffer)[leidos - 1] = '\0';
	return 1;
}

static int procesar_linea(char *linea, tp1_t *tp, abb_t *arbol_por_id,
			  abb_t *arbol_por_nombre)
{
	if (!linea)
		return 0;
	if (*recortar_espacios(linea) == '\0')
		return 0;
	char *argumentos = NULL;
	enum tipo_comando tipo = parsear_comando(linea, &argumentos);
	return ejecutar_comando(tipo, argumentos, tp, arbol_por_id,
				arbol_por_nombre);
}

struct contexto_construccion {
	abb_t *por_id;
	abb_t *por_nombre;
};

static bool insertar_en_arboles(struct pokemon *p, void *extra)
{
	struct contexto_construccion *contexto =
		(struct contexto_construccion *)extra;
	(void)abb_insertar(contexto->por_id, p);
	(void)abb_insertar(contexto->por_nombre, p);
	return true;
}

static void mostrar_ayuda(const char *prog)
{
	printf("Uso: %s <archivo_csv>\n", prog);
	printf("Comandos interactivos (por stdin):\n");
	printf("  id <numero>        Busca por id\n");
	printf("  nombre <cadena>    Busca por nombre (resto de la linea)\n");
	printf("  cantidad           Muestra cantidad de pokemones\n");
	printf("  salir              Termina el programa\n");
}

#ifdef ABB_APP
int main(int argc, char **argv)
{
	if (argc < 2) {
		mostrar_ayuda(argv[0]);
		return 1;
	}

	const char *ruta = argv[1];
	tp1_t *tp = tp1_leer_archivo(ruta);
	if (!tp) {
		fprintf(stderr, "Error: no se pudo leer '%s'\n", ruta);
		return 1;
	}

	abb_t *arbol_por_id = abb_crear(cmp_pokemon_por_id);
	abb_t *arbol_por_nombre = abb_crear(cmp_pokemon_por_nombre);
	if (!arbol_por_id || !arbol_por_nombre) {
		fprintf(stderr, "Error: no se pudo crear los ABB\n");
		abb_destruir(arbol_por_id);
		abb_destruir(arbol_por_nombre);
		tp1_destruir(tp);
		return 1;
	}

	struct contexto_construccion contexto = {
		.por_id = arbol_por_id, .por_nombre = arbol_por_nombre
	};
	tp1_con_cada_pokemon(tp, insertar_en_arboles, &contexto);

	printf("Cargados %zu pokemones en ABB. Ingrese consultas. (escriba 'salir' para terminar)\n",
	       tp1_cantidad(tp));

	char *linea_ingresada = NULL;
	size_t capacidad_linea = 0;
	int finalizar = 0;
	while (!finalizar) {
		puts(">");
		if (!leer_linea(&linea_ingresada, &capacidad_linea))
			finalizar = 1;
		else
			finalizar = procesar_linea(linea_ingresada, tp,
						   arbol_por_id,
						   arbol_por_nombre);
	}

	free(linea_ingresada);
	abb_destruir(arbol_por_id);
	abb_destruir(arbol_por_nombre);
	tp1_destruir(tp);
	return 0;
}
#endif // ABB_APP
