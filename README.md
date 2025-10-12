<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA ABB


## Alumno: (Jerónimo Pérez Córdoba) - (111939) - (jeperezc@fi.uba.ar - jeroperez42@gmail.com)

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g src/*.c main.c -o abb_app```

- Para ejecutar:

```compilar y ejecutar pruebas 
make run 
```

- Para ejecutar con valgrind:
```
make valgrind
```

```MAIN: estando en ABB-ENUNCIADO
./abb_app <archivo_csv> <comando>
```

---

##  Funcionamiento

El TDA ABB (Árbol Binario de Búsqueda) implementado en este TP es completamente genérico: puede almacenar cualquier tipo de puntero, y el criterio de orden lo define el usuario mediante una función de comparación pasada al crear el ABB.

---

## ABB_CREAR

-Objetivo: Crear un nuevo árbol binario de búsqueda vacío.

-Qué hace: Reserva memoria para la estructura principal y   guarda el puntero a la función de comparación que define el criterio de orden.

-Complejidad: O(1), porque solo reserva memoria y guarda punteros.
   
---
## ABB_INSERTAR 

-Objetivo: Insertar un elemento en el ABB.

-Qué hace: Busca la posición correcta según el comparador y agrega el elemento. Si el elemento ya existe, igualmente lo inserta (pueden existir duplicados).

-Complejidad: O(log n) promedio, O(n) peor caso, porque depende de la altura del árbol.

---
## ABB_BUSCAR 

-Objetivo: Buscar un elemento en el ABB.

-Qué hace: Recorre el árbol usando el comparador hasta encontrar el elemento o llegar a una hoja. Devuelve un puntero al dato o NULL si no está.

-Complejidad: O(log n) promedio, O(n) peor caso, porque recorre un solo camino desde la raíz.

---
## ABB_EXISTE 

-Objetivo: Verificar si un elemento está en el ABB.

-Qué hace: Igual que abb_buscar, pero devuelve true/false.

-Complejidad: O(log n) promedio, O(n) peor caso, porque internamente busca igual que abb_buscar.

---
## ABB_ELIMINAR

-Objetivo: Eliminar un elemento del ABB.

-Qué hace: Busca el nodo a eliminar y lo quita del árbol. Si tiene dos hijos, lo reemplaza por el predecesor inorden.

-Complejidad: O(log n) promedio, O(n) peor caso, porque depende de la altura del árbol.

---
## ABB_RAIZ

-Objetivo: Obtener el dato almacenado en la raíz del ABB.

-Qué hace: Devuelve un puntero al dato de la raíz.

-Complejidad: O(1), porque es acceso directo a un puntero.

---
## ABB_ESTA_VACIO

-Objetivo: Verificar si el ABB está vacío.

-Qué hace: Devuelve true si la cantidad de elementos es 0.

-Complejidad: O(1), porque compara un contador.

---
## ABB_CON_CADA_ELEMENTO

-Objetivo: Recorrer el ABB aplicando una función a cada elemento.

-Qué hace: Recorre el árbol en el orden pedido (inorden, preorden, postorden) y ejecuta una función visitante para cada dato.

-Complejidad: O(n), porque visita todos los nodos una vez.

---
## ABB_VECTORIZAR

-Objetivo: Copiar los elementos del ABB a un vector.

-Qué hace: Recorre el árbol en el orden pedido y copia los punteros a un vector.

-Complejidad: O(n), porque recorre todos los nodos para copiarlos.

---
## ABB_DESTRUIR
-Objetivo: Liberar toda la memoria usada por el ABB.

-Qué hace: Libera todos los nodos del árbol, pero no los datos almacenados.

-Complejidad: O(n), porque libera cada nodo una vez.

---
## ABB_DESTRUIR_TODO

-Objetivo: Liberar toda la memoria del ABB y de los datos almacenados.

-Qué hace: Igual que abb_destruir, pero además llama a una función destructora para cada dato.

-Complejidad: O(n), porque recorre y destruye todos los nodos y datos.


<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:




<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas