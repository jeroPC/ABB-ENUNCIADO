#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>

int comparador_int(const void *a, const void *b)
{
	if (a == NULL && b == NULL)
		return 0;
	if (a == NULL)
		return -1;
	if (b == NULL)
		return 1;
	int num_a = *(int *)a;
	int num_b = *(int *)b;
	return num_a - num_b;
}

int main()
{
	printf("Test 1: Insertar NULL\n");
	abb_t *abb = abb_crear(comparador_int);
	
	bool resultado = abb_insertar(abb, NULL);
	printf("Insertar NULL: %s\n", resultado ? "OK" : "FALLO");
	printf("Cantidad: %zu\n", abb_cantidad(abb));
	
	resultado = abb_insertar(abb, NULL);
	printf("Insertar NULL duplicado: %s (esperado: FALLO)\n", resultado ? "OK" : "FALLO");
	printf("Cantidad: %zu\n\n", abb_cantidad(abb));
	
	printf("Test 2: Insertar elemento tras NULL\n");
	int valor = 10;
	resultado = abb_insertar(abb, &valor);
	printf("Insertar 10: %s\n", resultado ? "OK" : "FALLO");
	printf("Cantidad: %zu\n\n", abb_cantidad(abb));
	
	printf("Test 3: Buscar NULL\n");
	void *encontrado = abb_buscar(abb, NULL);
	printf("Buscar NULL: %s\n", encontrado == NULL ? "Encontrado (es NULL)" : "No encontrado");
	
	printf("\nTest 4: Búsqueda mixta\n");
	int val2 = 5;
	abb_insertar(abb, &val2);
	
	void *eliminado = abb_eliminar(abb, &valor);
	printf("Eliminado 10: %p\n", eliminado);
	
	encontrado = abb_buscar(abb, &val2);
	printf("Buscar 5: %s\n", encontrado ? "Encontrado" : "No encontrado");
	
	abb_destruir(abb);
	printf("\nTest completado\n");
	return 0;
}
