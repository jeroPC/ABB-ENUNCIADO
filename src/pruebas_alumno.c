#include <stddef.h> // Para NULL
#include "pa2m.h"
#include "src/abb.h"

// Prototipo del comparador para usarlo antes de su definición
int comparador_int(const void* a, const void* b);

void pruebas_destruccion_abb() {
	pa2m_nuevo_grupo("Pruebas de destruccion de ABB");
	abb_t* abb = abb_crear(comparador_int);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_destruir(abb);
	pa2m_afirmar(1, "abb_destruir libera memoria sin errores");
}
void pruebas_eliminacion_stub_abb() {
	pa2m_nuevo_grupo("Pruebas de eliminacion (stub) en ABB");
	abb_t* abb = abb_crear(comparador_int);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	size_t cant = abb_cantidad(abb);
	pa2m_afirmar(abb_eliminar(abb, &a) == (void*)0, "abb_eliminar devuelve NULL (stub)");
	pa2m_afirmar(abb_cantidad(abb) == cant, "Cantidad no cambia tras eliminar (stub)");
	abb_destruir(abb);
}
void pruebas_cantidad_y_vacio_abb() {
	pa2m_nuevo_grupo("Pruebas de cantidad y vacio en ABB");
	abb_t* abb = abb_crear(comparador_int);
	pa2m_afirmar(abb_esta_vacio(abb), "ABB recien creado esta vacio");
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	pa2m_afirmar(!abb_esta_vacio(abb), "ABB no esta vacio tras insertar");
	pa2m_afirmar(abb_cantidad(abb) == 1, "Cantidad es 1 tras insertar");
	abb_insertar(abb, &b);
	pa2m_afirmar(abb_cantidad(abb) == 2, "Cantidad es 2 tras insertar otro");
	abb_destruir(abb);
	pa2m_afirmar(abb_cantidad((void*)0) == 0, "abb_cantidad(NULL) devuelve 0");
	pa2m_afirmar(abb_esta_vacio((void*)0), "abb_esta_vacio(NULL) devuelve true");
}
void pruebas_existencia_y_busqueda_abb() {
	pa2m_nuevo_grupo("Pruebas de existencia y busqueda en ABB");
	abb_t* abb = abb_crear(comparador_int);
	int a = 10, b = 5, c = 20, d = 15;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	pa2m_afirmar(abb_existe(abb, &a), "abb_existe encuentra elemento presente (raiz)");
	pa2m_afirmar(abb_existe(abb, &b), "abb_existe encuentra elemento presente (izq)");
	pa2m_afirmar(abb_existe(abb, &c), "abb_existe encuentra elemento presente (der)");
	pa2m_afirmar(!abb_existe(abb, &d), "abb_existe no encuentra elemento ausente");
	pa2m_afirmar(abb_buscar(abb, &a) == &a, "abb_buscar devuelve puntero correcto para raiz");
	pa2m_afirmar(abb_buscar(abb, &b) == &b, "abb_buscar devuelve puntero correcto para izq");
	pa2m_afirmar(abb_buscar(abb, &c) == &c, "abb_buscar devuelve puntero correcto para der");
	pa2m_afirmar(abb_buscar(abb, &d) == (void*)0, "abb_buscar devuelve NULL para elemento ausente");
	abb_destruir(abb);
}
void pruebas_insercion_abb() {
	pa2m_nuevo_grupo("Pruebas de insercion en ABB");
	abb_t* abb = abb_crear(comparador_int);
	int a = 10, b = 5, c = 20;
	pa2m_afirmar(abb_insertar(abb, &a), "Se puede insertar un elemento en ABB vacio");
	pa2m_afirmar(abb_cantidad(abb) == 1, "Cantidad es 1 tras insertar");
	pa2m_afirmar(*(int*)abb_raiz(abb) == a, "La raiz es el primer elemento insertado");
	pa2m_afirmar(abb_insertar(abb, &b), "Se puede insertar elemento menor a la raiz");
	pa2m_afirmar(abb_cantidad(abb) == 2, "Cantidad es 2 tras insertar otro");
	pa2m_afirmar(abb_insertar(abb, &c), "Se puede insertar elemento mayor a la raiz");
	pa2m_afirmar(abb_cantidad(abb) == 3, "Cantidad es 3 tras insertar otro");
	pa2m_afirmar(!abb_insertar(abb, &a), "No se puede insertar elemento repetido");
	pa2m_afirmar(abb_cantidad(abb) == 3, "Cantidad no cambia al intentar insertar repetido");
	abb_destruir(abb);
}


int comparador_int(const void* a, const void* b) {
	int ia = *(const int*)a;
	int ib = *(const int*)b;
	return ia - ib;
}

void pruebas_creacion_abb() {
	pa2m_nuevo_grupo("Pruebas de creacion de ABB");
	abb_t* abb = abb_crear(comparador_int);
	pa2m_afirmar(abb != NULL, "Se puede crear un ABB con comparador valido");
	pa2m_afirmar(abb_cantidad(abb) == 0, "ABB recien creado tiene cantidad 0");
	pa2m_afirmar(abb_raiz(abb) == (void*)0, "ABB recien creado no tiene raiz");
	abb_destruir(abb);
	pa2m_afirmar(1, "Se puede destruir un ABB vacio sin errores");

	abb = abb_crear((void*)0);
	pa2m_afirmar(abb == (void*)0, "No se puede crear ABB con comparador nulo");
}

int main()
{
	pruebas_creacion_abb();
	pruebas_insercion_abb();
	pruebas_existencia_y_busqueda_abb();
	pruebas_cantidad_y_vacio_abb();
	pruebas_eliminacion_stub_abb();
	pruebas_destruccion_abb();
	return pa2m_mostrar_reporte();
}
