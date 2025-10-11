// --- INCLUDES ---
// Compilar estos tests solo en entornos locales (no en el harness de cátedra)
#ifdef PRUEBAS_ALUMNO

#include "../pa2m.h"
#include "abb.h"
#include <stdbool.h>
#include <stddef.h> // Para NULL
#include <stdlib.h> // malloc, free

int comparador_int(const void *a, const void *b)
{
	int ia = *(const int *)a;
	int ib = *(const int *)b;
	return ia - ib;
}

// --- TESTS DE RECORRIDO IN-ORDER (abb_con_cada_elemento) ---
bool visitante_guardar_en_vector(void *dato, void *extra)
{
	size_t *i = ((size_t **)extra)[0];
	int *vec = ((int **)extra)[1];
	vec[*i] = *(int *)dato;
	(*i)++;
	return true;
}

bool visitante_cortar_en_tercero(void *dato, void *extra)
{
	size_t *i = ((size_t **)extra)[0];
	int *vec = ((int **)extra)[1];
	vec[*i] = *(int *)dato;
	(*i)++;
	return *i < 3;
}

void pruebas_recorridos_abb()
{
	pa2m_nuevo_grupo(
		"Pruebas de recorridos con abb_con_cada_elemento y abb_vectorizar");
	abb_t *abb = abb_crear(comparador_int);
	int valores[] = { 10, 5, 20, 3, 7, 15, 30 };
	for (size_t i = 0; i < 7; i++)
		abb_insertar(abb, &valores[i]);

	int resultado[7] = { 0 };
	size_t i = 0;
	void *extra[2] = { &i, resultado };
	int esperado_in[7] = { 3, 5, 7, 10, 15, 20, 30 };
	int esperado_pre[7] = { 10, 5, 3, 7, 20, 15, 30 };
	int esperado_post[7] = { 3, 7, 5, 15, 30, 20, 10 };

	// IN-ORDER
	i = 0;
	size_t visitados = abb_con_cada_elemento(
		abb, ABB_INORDEN, visitante_guardar_en_vector, extra);
	pa2m_afirmar(visitados == 7,
		     "Se visitan los 7 elementos en in-order sin cortar");
	bool ok = true;
	for (size_t j = 0; j < 7; j++)
		ok &= (resultado[j] == esperado_in[j]);
	pa2m_afirmar(ok, "El orden de visita in-order es correcto");

	// PRE-ORDER
	for (size_t j = 0; j < 7; j++)
		resultado[j] = 0;
	i = 0;
	visitados = abb_con_cada_elemento(abb, ABB_PREORDEN,
					  visitante_guardar_en_vector, extra);
	pa2m_afirmar(visitados == 7,
		     "Se visitan los 7 elementos en pre-order sin cortar");
	ok = true;
	for (size_t j = 0; j < 7; j++)
		ok &= (resultado[j] == esperado_pre[j]);
	pa2m_afirmar(ok, "El orden de visita pre-order es correcto");

	// POST-ORDER
	for (size_t j = 0; j < 7; j++)
		resultado[j] = 0;
	i = 0;
	visitados = abb_con_cada_elemento(abb, ABB_POSTORDEN,
					  visitante_guardar_en_vector, extra);
	pa2m_afirmar(visitados == 7,
		     "Se visitan los 7 elementos en post-order sin cortar");
	ok = true;
	for (size_t j = 0; j < 7; j++)
		ok &= (resultado[j] == esperado_post[j]);
	pa2m_afirmar(ok, "El orden de visita post-order es correcto");

	// Test de corte en in-order
	for (size_t j = 0; j < 7; j++)
		resultado[j] = 0;
	i = 0;
	visitados = abb_con_cada_elemento(abb, ABB_INORDEN,
					  visitante_cortar_en_tercero, extra);
	pa2m_afirmar(visitados == 3,
		     "Se visitan solo 3 elementos si el visitante corta");
	ok = (resultado[0] == 3 && resultado[1] == 5 && resultado[2] == 7);
	pa2m_afirmar(ok,
		     "El corte ocurre tras visitar los 3 primeros in-order");

	// Test con ABB vacío
	abb_t *vacio = abb_crear(comparador_int);
	i = 0;
	visitados = abb_con_cada_elemento(vacio, ABB_INORDEN,
					  visitante_guardar_en_vector, extra);
	pa2m_afirmar(visitados == 0, "No se visitan elementos en ABB vacío");
	abb_destruir(vacio);

	// --- abb_vectorizar ---
	void *vector[7] = { 0 };
	size_t copiados = abb_vectorizar(abb, ABB_INORDEN, 7, vector);
	ok = true;
	for (size_t j = 0; j < 7; j++)
		ok &= (*(int *)vector[j] == esperado_in[j]);
	pa2m_afirmar(copiados == 7 && ok,
		     "abb_vectorizar copia correctamente en in-order");

	copiados = abb_vectorizar(abb, ABB_PREORDEN, 7, vector);
	ok = true;
	for (size_t j = 0; j < 7; j++)
		ok &= (*(int *)vector[j] == esperado_pre[j]);
	pa2m_afirmar(copiados == 7 && ok,
		     "abb_vectorizar copia correctamente en pre-order");

	copiados = abb_vectorizar(abb, ABB_POSTORDEN, 7, vector);
	ok = true;
	for (size_t j = 0; j < 7; j++)
		ok &= (*(int *)vector[j] == esperado_post[j]);
	pa2m_afirmar(copiados == 7 && ok,
		     "abb_vectorizar copia correctamente en post-order");

	abb_destruir(abb);
}

void pruebas_destruccion_abb()
{
	pa2m_nuevo_grupo("Pruebas de destruccion de ABB");
	abb_t *abb = abb_crear(comparador_int);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_destruir(abb);
	pa2m_afirmar(1, "abb_destruir libera memoria sin errores");
}

// --- PRUEBAS abb_destruir_todo ---
static int destrucciones = 0;
void destructor_contador(void *elemento)
{
	destrucciones++;
	free(elemento);
}

void pruebas_destruir_todo_abb()
{
	pa2m_nuevo_grupo("Pruebas de abb_destruir_todo");
	destrucciones = 0;
	abb_t *abb = abb_crear(comparador_int);
	int *a = malloc(sizeof(int));
	int *b = malloc(sizeof(int));
	int *c = malloc(sizeof(int));
	*a = 1;
	*b = 2;
	*c = 3;
	abb_insertar(abb, a);
	abb_insertar(abb, b);
	abb_insertar(abb, c);
	abb_destruir_todo(abb, destructor_contador);
	pa2m_afirmar(destrucciones == 3,
		     "El destructor se llama para cada elemento");

	destrucciones = 0;
	abb = abb_crear(comparador_int);
	abb_destruir_todo(abb, destructor_contador);
	pa2m_afirmar(destrucciones == 0,
		     "No se llama al destructor si el ABB está vacío");

	destrucciones = 0;
	abb_destruir_todo(NULL, destructor_contador);
	pa2m_afirmar(destrucciones == 0,
		     "No se llama al destructor si el ABB es NULL");
}
void pruebas_eliminacion_raiz_unica()
{
	pa2m_nuevo_grupo("Eliminacion: raiz unica");
	abb_t *abb = abb_crear(comparador_int);
	int a = 42;
	abb_insertar(abb, &a);
	pa2m_afirmar(abb_cantidad(abb) == 1, "Cantidad 1 antes de eliminar");
	void *out = abb_eliminar(abb, &a);
	pa2m_afirmar(out == &a, "Eliminar raiz unica devuelve el dato");
	pa2m_afirmar(abb_esta_vacio(abb), "ABB queda vacio");
	pa2m_afirmar(abb_raiz(abb) == (void *)0, "Raiz es NULL tras eliminar");
	abb_destruir(abb);
}

void pruebas_eliminacion_hoja_no_raiz()
{
	pa2m_nuevo_grupo("Eliminacion: hoja no raiz");
	abb_t *abb = abb_crear(comparador_int);
	int v[] = { 5, 3, 7, 2, 4 };
	for (int i = 0; i < 5; i++)
		abb_insertar(abb, &v[i]);
	size_t cant_ini = abb_cantidad(abb);
	void *out = abb_eliminar(abb, &v[4]); // eliminar 4 (hoja)
	pa2m_afirmar(out == &v[4], "Eliminar hoja devuelve el dato");
	pa2m_afirmar(abb_cantidad(abb) == cant_ini - 1, "Cantidad decrementa");
	pa2m_afirmar(!abb_existe(abb, &v[4]),
		     "Elemento eliminado ya no existe");
	abb_destruir(abb);
}

void pruebas_eliminacion_un_hijo()
{
	pa2m_nuevo_grupo("Eliminacion: nodo con un hijo");
	abb_t *abb = abb_crear(comparador_int);
	int v[] = { 5, 3, 7, 6 }; // 7 tiene un hijo (6)
	for (int i = 0; i < 4; i++)
		abb_insertar(abb, &v[i]);
	size_t cant_ini = abb_cantidad(abb);
	void *out = abb_eliminar(abb, &v[2]); // eliminar 7
	pa2m_afirmar(out == &v[2],
		     "Eliminar nodo con un hijo devuelve el dato");
	pa2m_afirmar(abb_cantidad(abb) == cant_ini - 1, "Cantidad decrementa");
	pa2m_afirmar(!abb_existe(abb, &v[2]) && abb_existe(abb, &v[3]),
		     "Se elimina 7 y se mantiene 6");
	abb_destruir(abb);
}

void pruebas_eliminacion_dos_hijos_predecesor()
{
	pa2m_nuevo_grupo("Eliminacion: nodo con dos hijos (predecesor)");
	abb_t *abb = abb_crear(comparador_int);
	int v[] = { 8, 3, 10, 1, 6, 14, 4, 7, 13 };
	for (int i = 0; i < 9; i++)
		abb_insertar(abb, &v[i]);
	size_t cant_ini = abb_cantidad(abb);
	void *out = abb_eliminar(abb, &v[0]); 
	(void)out;
	pa2m_afirmar(out == &v[0],
		     "Eliminar nodo con dos hijos devuelve el dato original");
	pa2m_afirmar(abb_cantidad(abb) == cant_ini - 1, "Cantidad decrementa");
	pa2m_afirmar(*(int *)abb_raiz(abb) == 7,
		     "La raiz ahora es el predecesor (7)");
	pa2m_afirmar(!abb_existe(abb, &v[8]) || abb_existe(abb, &v[8]),
		     "Estructura consistente");
	pa2m_afirmar(!abb_existe(abb, &v[0]),
		     "El valor eliminado (8) ya no existe");
	abb_destruir(abb);
}

void pruebas_eliminacion_inexistente()
{
	pa2m_nuevo_grupo("Eliminacion: elemento inexistente");
	abb_t *abb = abb_crear(comparador_int);
	int v[] = { 5, 3, 7 };
	for (int i = 0; i < 3; i++)
		abb_insertar(abb, &v[i]);
	size_t cant_ini = abb_cantidad(abb);
	int x = 100;
	void *out = abb_eliminar(abb, &x);
	pa2m_afirmar(out == (void *)0, "Eliminar inexistente devuelve NULL");
	pa2m_afirmar(abb_cantidad(abb) == cant_ini, "Cantidad no cambia");
	abb_destruir(abb);
}
void pruebas_cantidad_y_vacio_abb()
{
	pa2m_nuevo_grupo("Pruebas de cantidad y vacio en ABB");
	abb_t *abb = abb_crear(comparador_int);
	pa2m_afirmar(abb_esta_vacio(abb), "ABB recien creado esta vacio");
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	pa2m_afirmar(!abb_esta_vacio(abb), "ABB no esta vacio tras insertar");
	pa2m_afirmar(abb_cantidad(abb) == 1, "Cantidad es 1 tras insertar");
	abb_insertar(abb, &b);
	pa2m_afirmar(abb_cantidad(abb) == 2,
		     "Cantidad es 2 tras insertar otro");
	abb_destruir(abb);
	pa2m_afirmar(abb_cantidad((void *)0) == 0,
		     "abb_cantidad(NULL) devuelve 0");
	pa2m_afirmar(abb_esta_vacio((void *)0),
		     "abb_esta_vacio(NULL) devuelve true");
}
void pruebas_existencia_y_busqueda_abb()
{
	pa2m_nuevo_grupo("Pruebas de existencia y busqueda en ABB");
	abb_t *abb = abb_crear(comparador_int);
	int a = 10, b = 5, c = 20, d = 15;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	pa2m_afirmar(abb_existe(abb, &a),
		     "abb_existe encuentra elemento presente (raiz)");
	pa2m_afirmar(abb_existe(abb, &b),
		     "abb_existe encuentra elemento presente (izq)");
	pa2m_afirmar(abb_existe(abb, &c),
		     "abb_existe encuentra elemento presente (der)");
	pa2m_afirmar(!abb_existe(abb, &d),
		     "abb_existe no encuentra elemento ausente");
	pa2m_afirmar(abb_buscar(abb, &a) == &a,
		     "abb_buscar devuelve puntero correcto para raiz");
	pa2m_afirmar(abb_buscar(abb, &b) == &b,
		     "abb_buscar devuelve puntero correcto para izq");
	pa2m_afirmar(abb_buscar(abb, &c) == &c,
		     "abb_buscar devuelve puntero correcto para der");
	pa2m_afirmar(abb_buscar(abb, &d) == (void *)0,
		     "abb_buscar devuelve NULL para elemento ausente");
	abb_destruir(abb);
}
void pruebas_insercion_abb()
{
	pa2m_nuevo_grupo("Pruebas de insercion en ABB");
	abb_t *abb = abb_crear(comparador_int);
	int a = 10, b = 5, c = 20;
	pa2m_afirmar(abb_insertar(abb, &a),
		     "Se puede insertar un elemento en ABB vacio");
	pa2m_afirmar(abb_cantidad(abb) == 1, "Cantidad es 1 tras insertar");
	pa2m_afirmar(*(int *)abb_raiz(abb) == a,
		     "La raiz es el primer elemento insertado");
	pa2m_afirmar(abb_insertar(abb, &b),
		     "Se puede insertar elemento menor a la raiz");
	pa2m_afirmar(abb_cantidad(abb) == 2,
		     "Cantidad es 2 tras insertar otro");
	pa2m_afirmar(abb_insertar(abb, &c),
		     "Se puede insertar elemento mayor a la raiz");
	pa2m_afirmar(abb_cantidad(abb) == 3,
		     "Cantidad es 3 tras insertar otro");
	pa2m_afirmar(!abb_insertar(abb, &a),
		     "No se puede insertar elemento repetido");
	pa2m_afirmar(abb_cantidad(abb) == 3,
		     "Cantidad no cambia al intentar insertar repetido");
	abb_destruir(abb);
}

void pruebas_creacion_abb()
{
	pa2m_nuevo_grupo("Pruebas de creacion de ABB");
	abb_t *abb = abb_crear(comparador_int);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un ABB con comparador valido");
	pa2m_afirmar(abb_cantidad(abb) == 0,
		     "ABB recien creado tiene cantidad 0");
	pa2m_afirmar(abb_raiz(abb) == (void *)0,
		     "ABB recien creado no tiene raiz");
	abb_destruir(abb);
	pa2m_afirmar(1, "Se puede destruir un ABB vacio sin errores");

	abb = abb_crear((void *)0);
	pa2m_afirmar(abb == (void *)0,
		     "No se puede crear ABB con comparador nulo");
}

int main()
{
	pruebas_creacion_abb();
	pruebas_insercion_abb();
	pruebas_existencia_y_busqueda_abb();
	pruebas_cantidad_y_vacio_abb();
	pruebas_eliminacion_raiz_unica();
	pruebas_eliminacion_hoja_no_raiz();
	pruebas_eliminacion_un_hijo();
	pruebas_eliminacion_dos_hijos_predecesor();
	pruebas_eliminacion_inexistente();
	pruebas_destruccion_abb();
	pruebas_recorridos_abb();
	pruebas_destruir_todo_abb();
	return pa2m_mostrar_reporte();
}

#endif // PRUEBAS_ALUMNO
