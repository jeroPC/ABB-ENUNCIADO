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
	printf("Test de eliminación y búsqueda mixta\n");
	abb_t *abb = abb_crear(comparador_int);
	
	int vals[] = {50, 30, 70, 20, 40, 60, 80, 35, 45};
	int n = sizeof(vals) / sizeof(vals[0]);
	
	printf("Insertando %d elementos: ", n);
	for (int i = 0; i < n; i++) {
		if (abb_insertar(abb, &vals[i])) {
			printf("%d ", vals[i]);
		} else {
			printf("X ");
		}
	}
	printf("\n");
	printf("Cantidad en el abb: %zu\n\n", abb_cantidad(abb));
	
	printf("Eliminando 70 (tiene dos hijos)...\n");
	void *eliminado = abb_eliminar(abb, &vals[2]); // 70
	printf("Eliminado: %p (esperado: %p)\n", eliminado, (void*)&vals[2]);
	printf("Cantidad: %zu\n\n", abb_cantidad(abb));
	
	printf("Eliminando 30 (tiene dos hijos)...\n");
	eliminado = abb_eliminar(abb, &vals[1]); // 30
	printf("Eliminado: %p (esperado: %p)\n", eliminado, (void*)&vals[1]);
	printf("Cantidad: %zu\n\n", abb_cantidad(abb));
	
	printf("Buscando elementos que deberían seguir:\n");
	void *encontrado;
	
	encontrado = abb_buscar(abb, &vals[0]); // 50
	printf("Buscar 50: %s (%p)\n", encontrado ? "OK" : "FALLO", encontrado);
	
	encontrado = abb_buscar(abb, &vals[3]); // 20
	printf("Buscar 20: %s (%p)\n", encontrado ? "OK" : "FALLO", encontrado);
	
	encontrado = abb_buscar(abb, &vals[4]); // 40
	printf("Buscar 40: %s (%p)\n", encontrado ? "OK" : "FALLO", encontrado);
	
	encontrado = abb_buscar(abb, &vals[5]); // 60
	printf("Buscar 60: %s (%p)\n", encontrado ? "OK" : "FALLO", encontrado);
	
	encontrado = abb_buscar(abb, &vals[6]); // 80
	printf("Buscar 80: %s (%p)\n", encontrado ? "OK" : "FALLO", encontrado);
	
	printf("\nBuscando elementos que NO deberían estar:\n");
	encontrado = abb_buscar(abb, &vals[1]); // 30
	printf("Buscar 30: %s (esperado: FALLO)\n", encontrado ? "OK" : "FALLO");
	
	encontrado = abb_buscar(abb, &vals[2]); // 70
	printf("Buscar 70: %s (esperado: FALLO)\n", encontrado ? "OK" : "FALLO");
	
	abb_destruir(abb);
	printf("\nTest completado sin crashes\n");
	return 0;
}
