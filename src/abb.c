#include "abb.h"
#include "abb_estructura_privada.h"

abb_t *abb_crear(int (*comparador)(const void *, const void *))
{
    abb_t abb;
	return NULL;
}

bool abb_insertar(abb_t *abb, void *dato)
{
	return false;
}

bool abb_existe(abb_t *abb, void *dato)
{
	return false;
}

void *abb_buscar(abb_t *abb, void *dato)
{
	return NULL;
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	return NULL;
}

void *abb_raiz(abb_t *abb)
{
	return NULL;
}

size_t abb_cantidad(abb_t *abb)
{
	return 0;
}

bool abb_esta_vacio(abb_t *abb)
{
	return false;
}

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra)
{
	return 0;
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	return 0;
}

void abb_destruir(abb_t *abb)
{
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
}
