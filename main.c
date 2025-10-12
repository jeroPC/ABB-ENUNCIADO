#define _POSIX_C_SOURCE 200809L
#include "src/abb.h"
#include "src/tp1.h"
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

struct contexto_construccion {
	abb_t *por_id;
	abb_t *por_nombre;
};

static bool insertar_en_arboles(struct pokemon *p, void *extra)
{
	struct contexto_construccion *ctx =
		(struct contexto_construccion *)extra;
	abb_insertar(ctx->por_id, p);
	abb_insertar(ctx->por_nombre, p);
	return true;
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Uso: %s <archivo_csv> <comando>\n", argv[0]);
		printf("Comandos:\n");
		printf("  id <numero>\n");
		printf("  nombre <cadena>\n");
		printf("  cantidad\n");
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

	struct contexto_construccion contexto = { arbol_por_id,
						  arbol_por_nombre };
	tp1_con_cada_pokemon(tp, insertar_en_arboles, &contexto);

	if (strcmp(argv[2], "cantidad") == 0) {
		printf("%zu\n", tp1_cantidad(tp));
	} else if (strcmp(argv[2], "id") == 0 && argc >= 4) {
		int id = atoi(argv[3]);
		struct pokemon clave = { 0 };
		clave.id = id;
		struct pokemon *encontrado = abb_buscar(arbol_por_id, &clave);
		imprimir_pokemon(encontrado);
	} else if (strcmp(argv[2], "nombre") == 0 && argc >= 4) {
		struct pokemon clave = { 0 };
		clave.nombre = argv[3];
		struct pokemon *encontrado =
			abb_buscar(arbol_por_nombre, &clave);
		imprimir_pokemon(encontrado);
	} else {
		printf("Comando no reconocido o faltan argumentos.\n");
	}

	abb_destruir(arbol_por_id);
	abb_destruir(arbol_por_nombre);
	tp1_destruir(tp);
	return 0;
}
