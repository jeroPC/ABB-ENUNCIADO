#include "abb.h"
#include "abb_estructura_privada.h"

abb_t *abb_crear(int (*comparador)(const void *, const void *))
{
	if(!comparador) return NULL;
	abb_t *abb = calloc(1, sizeof(abb_t));
	if(!abb) return NULL;
	abb->comparador =comparador;
	return abb;
}

static nodo_t* crear_nodo(void* dato){
	nodo_t * nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->dato = dato;
	nodo->der = NULL;
	nodo->izq = NULL;

	return nodo;
}


static bool arbol_vacio(abb_t *abb, void *dato){
	if(!abb || !dato) return false;
	if(!abb->raiz){
		abb->raiz = crear_nodo(dato);
		if(!abb->raiz) return false;
		abb->cantidad++;
		return true;
	}
	return false;
}

static bool arbol_insercion_con_elementos(abb_t *abb, void *dato){
	if (!abb || !dato) return false;
	nodo_t *actual = abb->raiz;

	while (actual){
		int cmp = abb->comparador(dato,actual->dato);
		if(cmp == 0 ) return false;
		if (cmp < 0){
			if(!actual->izq){
				actual->izq = crear_nodo(dato);
				if (!actual->izq) return false;
                abb->cantidad++;
                return true;
			}
			actual = actual->izq;
		}else{
			if(!actual->der){
				actual->der = crear_nodo(dato);
				if (!actual->der) return false;
                abb->cantidad++;
                return true;
			}
			actual = actual->der;
		}
		
	}
	return false;
}


bool abb_insertar(abb_t *abb, void *dato)
{
	if (!abb ) return false;
	if(arbol_vacio(abb,dato)) return true;
	if(arbol_insercion_con_elementos(abb,dato))return true;

	return false;
}


bool abb_existe(abb_t *abb, void *dato)
{
	if (!abb ) return false;
	nodo_t *actual = abb->raiz;
	while (actual) {
		int cmp = abb->comparador(dato,actual->dato);
		if(cmp == 0) return true;
		else if(cmp < 0){
			actual = actual->izq;
		}else{
			actual = actual->der;
		}
	}

	return false;
}

void *abb_buscar(abb_t *abb, void *dato)
{
	if (!abb ) return NULL;
	nodo_t *actual = abb->raiz;
	while (actual) {
		int cmp = abb->comparador(dato,actual->dato);
		if(cmp == 0) return actual->dato;
		else if(cmp < 0){
			actual = actual->izq;
		}else{
			actual = actual->der;
		}
	}
	return NULL;
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	return NULL;
}

void *abb_raiz(abb_t *abb)
{
	if(!abb || !abb->raiz ) return NULL;
	return abb->raiz->dato;
}

size_t abb_cantidad(abb_t *abb)
{
	if(!abb) return 0;
	return abb->cantidad;
}

bool abb_esta_vacio(abb_t *abb)
{
	return (!abb || abb->cantidad ==0 );
}


static size_t abb_recorrido_in_order(nodo_t *nodo, bool (*f)(void *, void *), void *extra, bool *cortar ){
	if(!nodo || *cortar) return 0;
	size_t izq = abb_recorrido_in_order(nodo->izq ,f, extra, cortar);
	if(*cortar) return 0;
	if(!f(nodo->dato,extra)){
		*cortar = true;
		return izq + 1;
	}
	size_t der = abb_recorrido_in_order(nodo->der, f, extra, cortar);
	return izq +1 + der ;
}


size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra)
{
	bool cortar = false;
	return abb_recorrido_in_order(abb->raiz, f, extra, &cortar);
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	return 0;
}


static void abb_destruir_nodos(nodo_t* nodo){
	if (!nodo) return;
  	abb_destruir_nodos(nodo->izq);
    abb_destruir_nodos(nodo->der);
    free(nodo);
}

void abb_destruir(abb_t *abb)
{
	if(!abb) return;
	abb_destruir_nodos(abb->raiz);
	free(abb);
	
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
}
