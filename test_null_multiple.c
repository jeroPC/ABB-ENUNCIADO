#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>

int comparador_null_siempre_igual(const void *a, const void *b)
{
	// Este comparador hace que NULL siempre sea igual a NULL
	// pero también hace que NULL sea "menor" que cualquier otra cosa
	if (a == NULL && b == NULL)
		return 0;  // NULL == NULL
	if (a == NULL)
		return -1; // NULL < cualquier cosa
	if (b == NULL)
		return 1;  // cualquier cosa > NULL
	
	// Para punteros no-NULL, siempre son diferentes
	// (para que se puedan insertar múltiples)
	if (a < b)
		return -1;
	if (a > b)
		return 1;
	return 0;
}

void destruir_int(void *dato)
{
	free(dato);
}

int main()
{
	printf("Test de múltiples NULLs\n");
	abb_t *abb = abb_crear(comparador_null_siempre_igual);
	
	printf("Insertar primer NULL: ");
	bool r1 = abb_insertar(abb, NULL);
	printf("%s\n", r1 ? "OK" : "FALLO");
	printf("Cantidad: %zu\n", abb_cantidad(abb));
	
	printf("Insertar segundo NULL: ");
	bool r2 = abb_insertar(abb, NULL);
	printf("%s (esperado: FALLO porque NULL == NULL)\n", r2 ? "OK" : "FALLO");
	printf("Cantidad: %zu\n\n", abb_cantidad(abb));
	
	printf("Test de múltiples inserciones con comparador que permite duplicados\n");
	abb_t *abb2 = abb_crear(comparador_null_siempre_igual);
	
	int contador = 0;
	for (int i = 0; i < 500; i++) {
		int *val = malloc(sizeof(int));
		*val = i;
		if (abb_insertar(abb2, val)) {
			contador++;
		} else {
			free(val); // No se insertó, liberar
		}
	}
	
	printf("Insertados: %d de 500\n", contador);
	printf("Cantidad en abb: %zu\n", abb_cantidad(abb2));
	
	// Liberar memoria
	abb_destruir_todo(abb2, destruir_int);
	abb_destruir(abb);
	
	printf("\nTest completado\n");
	return 0;
}
