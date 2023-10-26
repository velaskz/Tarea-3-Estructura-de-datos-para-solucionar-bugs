#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#include "funcionesAnswers.h"
#include "list.h"
#include "hashmap.h"
#include "heap.h"
#include "treemap.h"
#define MAXLINEAS 1001

//----------------------------------------------------------------
//                            Estructuras                        |
//----------------------------------------------------------------

typedef struct Libro{
  HashMap* mapaDePalabras;
  HashMap* mapaConteoDePalabras;
  char ID[101];
  char* titulo;
  char** lineas;
  int cantidadLineas;
  int cantidadPalabras;
  int cantidadCaracteres;
} Libro;

//----------------------------------------------------------------
//                        Funciones del Menú                     |
//----------------------------------------------------------------

//                                        (1)
// 

void inicializarLibro(Libro *contenido) {
    contenido->titulo = NULL;
    contenido->lineas = NULL;
    contenido->cantidadLineas = 0;
    contenido->cantidadPalabras = 0;
    contenido->cantidadCaracteres = 0;
    contenido->mapaDePalabras = createMap(1001); 
    contenido->mapaConteoDePalabras = createMap(1001);
}

char *obtenerTitulo(FILE *file, const char *tituloArchivo) {
    char line[MAXLINEAS];
    char *title = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, tituloArchivo)) {
            title = strdup(line + strlen(tituloArchivo));
            break;  
        }
    }

    return title;
}

void addLine(Libro *contenido, char *line) {

    // Resto del código para agregar la línea como lo hacías
    contenido->lineas = (char **)realloc(contenido->lineas, (contenido->cantidadLineas + 1) * sizeof(char *));
    if (contenido->lineas == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    contenido->lineas[contenido->cantidadLineas] = strdup(line);
    if (contenido->lineas[contenido->cantidadLineas] == NULL) {
        fprintf(stderr, "Error: No se pudo duplicar la línea.\n");
        exit(EXIT_FAILURE);
    }

    contenido->cantidadLineas++;
    contenido->cantidadCaracteres += strlen(line);

    // Dividir la línea en palabras y contarlas
    int numPalabras = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL) {
        numPalabras++;
        token = strtok(NULL, " \t\n");
    }

    // Aumentar la cantidad de palabras en el libro
    contenido->cantidadPalabras += numPalabras;
}

void readBook(FILE *file, const char *startMark, const char *endMark, Libro *contenido) {
    char line[MAXLINEAS];
    int inContent = 0;

    while (fgets(line, sizeof(line), file)) {
        if (!inContent) {
            if (strstr(line, startMark)) {
                inContent = 1;
            }
        } else {
            if (strstr(line, endMark)) {
                break;
            } else {
                addLine(contenido, line);
            }
        }
    }
}

void liberarContenido(Libro *contenido) {
    for (int i = 0; i < contenido->cantidadLineas; i++) {
        free(contenido->lineas[i]);
    }
    free(contenido->lineas);
    contenido->lineas = NULL;
    contenido->cantidadLineas = 0;
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void procesarLinea(const char *linea, HashMap *mapa, HashMap *mapaConteoDePalabras) {
    char copiaLinea[strlen(linea) + 1];
    int posPalabra = 1;
    strcpy(copiaLinea, linea);

    char *token = strtok(copiaLinea, " \t\n");
    while (token != NULL) {
        // Convertir la palabra a minúsculas
        toLowerCase(token);

        // Agregar la palabra al mapa con la posición como valor
        insertMap(mapa, token, &posPalabra);

        // Incrementar la frecuencia de la palabra en el mapa de conteo
        Pair *conteoPair = searchMap(mapaConteoDePalabras, token);
        if (conteoPair) {
            int *frecuencia = (int *)conteoPair->value;
            (*frecuencia)++; // Incrementar la frecuencia
        } else {
            // Si la palabra no existe en el mapa de conteo, la añadimos con frecuencia 1
            int frecuencia = 1;
            insertMap(mapaConteoDePalabras, token, &frecuencia);
        }

        // Obtener el siguiente token
        token = strtok(NULL, " \t\n");
        posPalabra++;
    }
}

void procesarPalabra(const char *linea, HashMap *mapaLibros, HashMap *mapaConteoDePalabras) {
    char palabra[100]; // Suponiendo una longitud máxima de palabra de 100 caracteres
    int posPalabra = 1;

    int i = 0; // Índice para recorrer la línea de entrada
    while (linea[i] != '\0') {
        // Inicializa el búfer de la palabra
        int palabraIndex = 0;

        // Encuentra el inicio de una palabra
        while (linea[i] != '\0' && !isalpha((unsigned char)linea[i])) {
            i++;
        }

        // Extrae la palabra
        while (linea[i] != '\0' && isalpha((unsigned char)linea[i])) {
            palabra[palabraIndex] = tolower((unsigned char)linea[i]);
            palabraIndex++;
            i++;
        }
        palabra[palabraIndex] = '\0';

        if (palabraIndex > 0) {
            // Agrega la palabra a los mapas
            insertMap(mapaLibros, palabra, &posPalabra);

            Pair *conteoPair = searchMap(mapaConteoDePalabras, palabra);
            if (conteoPair) {
                int *frecuencia = (int *)conteoPair->value;
                (*frecuencia)++; // Incrementa la frecuencia
            } else {
                int frecuencia = 1;
                insertMap(mapaConteoDePalabras, palabra, &frecuencia);
            }
            posPalabra++;
        }
    }
}

void cargarDocumentos(HashMap* mapaLibros, HashMap* mapaIdes) {
    char nombreArchivo[101];
    const char *tituloTexto = "Title:";
    const char *startMark = "*** START";
    const char *endMark = "*** END ";
    char linea[1024]; // Tamaño máximo de una línea, ajústalo según tus necesidades

    printf("Ingrese el nombre del archivo de texto (o 'fin' para terminar la carga):\n");
    while (1) {
        // Leer el nombre del archivo
        scanf("%s", nombreArchivo);
        if (strcmp(nombreArchivo, "fin") == 0) {
            break;  // Salir del bucle si el usuario ingresa 'fin'
        }

        FILE* archivo = fopen(nombreArchivo, "r");
        if (archivo == NULL) {
            printf("No se pudo abrir el archivo: '%s', inténtelo de nuevo...\n", nombreArchivo);
            continue;  // Continuar con el próximo archivo si no se puede abrir
        }
        Libro *contenido =  (Libro*) malloc(sizeof(Libro));
        inicializarLibro(contenido);


        char *titulo = obtenerTitulo(archivo, tituloTexto);

        // Eliminar espacios en blanco alrededor del título
        if (titulo) {
            while (isspace(*titulo)) {
                titulo++;  
            }
            int len = strlen(titulo);
            while (len > 0 && isspace(titulo[len - 1])) {
                titulo[len - 1] = '\0';  
                len--;
            }
        }

        contenido->titulo = titulo;

        // Extraer el nombre del archivo sin la extensión .txt
        char nombreLibro[101];
        strcpy(nombreLibro, nombreArchivo);
        char *ext = strrchr(nombreLibro, '.'); // Buscar la última aparición del punto
        if (ext != NULL) {
            *ext = '\0'; // Establecer el punto como final de la cadena
        }

        strncpy(contenido->ID, nombreLibro, sizeof(contenido->ID) - 1);
        contenido->ID[sizeof(contenido->ID) - 1] = '\0';

        // Lee y carga el contenido del libro
        readBook(archivo, startMark, endMark, contenido);

        /*for (int i = 0; i < contenido->cantidadPalabras; i++) {
            printf("%s\n", contenido->lineas[i]);
        }*/

        for(int i = 0; i < contenido->cantidadLineas; i++) {
            procesarLinea(contenido->lineas[i], contenido->mapaDePalabras, contenido->mapaConteoDePalabras);
        }

        for(int i = 0; i < contenido->cantidadLineas; i++) {
            procesarPalabra(contenido->lineas[i], contenido->mapaDePalabras, contenido->mapaConteoDePalabras);
        }

        // Almacena el contenido del libro como valor en el mapaLibros con el nombre del archivo como clave
        insertMap(mapaLibros, contenido->titulo, contenido);
        insertMap(mapaIdes, contenido->ID, contenido);

        printf("\nEl libro '%s' ha sido cargado con éxito!\n\n", contenido->titulo);

        printf("Ingresa el nombre de otro archivo de texto (o 'fin' para terminar la carga):\n");

        //liberarContenido(contenido);
        fclose(archivo);
    }

    printf("\n\nLa carga de libros ha sido finalizada!\n");
}

//                                        (2)
// Ordena a veces
int compareStrings(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

void mostrarLibro(void *data) {
    Libro *libro = (Libro *)data;
    printf("ID: %s\n", libro->ID);
    printf("Título: %s\n", libro->titulo);
    printf("Cantidad de Palabras: %d\n", libro->cantidadPalabras);
    printf("Cantidad de Caracteres: %d\n", libro->cantidadCaracteres);
    printf("Cantidad de lineas: %i\n\n",libro->cantidadLineas);
}

void mostrarDocumentosOrdenados(HashMap* mapaLibros) {
    TreeMap* arbol = createTreeMap(lower_than_int); // Usar tu función de comparación

    printf("\nLista de Documentos Ordenados por título:\n\n");

    Pair *pair = firstMap(mapaLibros);
    while (pair != NULL) {
        /*printf("%s\n", pair->key); //prueba
        insertTreeMap(arbol, pair->key, pair->value); // Insertar en el TreeMap*/
        mostrarLibro(pair->value);
        pair = nextMap(mapaLibros);

    }
  /*
    Pair *current = firstTreeMap(arbol);

    //int cont = 1;
    while (current != NULL) {
        mostrarLibro(current->value);
        current = nextTreeMap(arbol);
        printf("%i",cont);//pasa solo una vez
        cont++;
    }

    free(arbol);*/
}

//                                        (3)
// muestra solo un libro :/

int sizeList(List *lista) {
    int numElementos = 0;
    void *nodo = firstList(lista);
    while (nodo != NULL) {
        numElementos++;
        nodo = nextList(lista);
    }
    return numElementos;
}

void buscarLibroXTitulo(HashMap *mapaLibros) {
    char palabra[101];

    // Crear una lista para almacenar los títulos de los libros coincidentes
    List *librosCoincidentes = createList();
    List *palabrasClave = createList();

    while (1) {
        // Leer una palabra clave
        printf("Ingrese palabra clave para buscar libros (ingrese 'fin' para finalizar):\n");
        scanf("%s", palabra);

        if (strcmp(palabra, "fin") == 0) {
            break;  // Salir del bucle si el usuario ingresa 'fin'
        }

        // Convertir la palabra clave a minúsculas
        toLowerCase(palabra);

        // Crear una lista para almacenar las palabras clave
        pushFront(palabrasClave, palabra);

        // Recorrer el mapa de libros
        Pair *pair = firstMap(mapaLibros);
        while (pair != NULL) {
            Libro *libro = (Libro *)pair->value;
            HashMap *mapaDePalabras = libro->mapaDePalabras;
            int coincideTodas = 1;

            // Verificar si el libro contiene todas las palabras clave
            void *palabraClave = firstList(palabrasClave);
            while (palabraClave != NULL) {
                char *clave = (char *)palabraClave;
                if (searchMap(mapaDePalabras, clave) == NULL) {
                    coincideTodas = 0;
                    break;
                }
                palabraClave = nextList(palabrasClave);
            }

            // Si el libro coincide con todas las palabras clave, agregar su título a la lista
            if (coincideTodas) {
                pushFront(librosCoincidentes, libro->titulo);
            }

            pair = nextMap(mapaLibros);

            // Liberar la memoria de la lista de palabras clave del libro actual
            cleanList(palabrasClave);
        }
    }

    // Verificar si se encontraron libros coincidentes
    if (librosCoincidentes != NULL) {
        printf("Libros que contienen todas las palabras clave:\n");

        // Mostrar los títulos de los libros coincidentes
        int contador = 1;
        void *tituloLibro = firstList(librosCoincidentes);
        while (tituloLibro != NULL) {
            char *titulo = (char *)tituloLibro;
            printf("%d. %s\n", contador, titulo);
            contador++;
            tituloLibro = nextList(librosCoincidentes);
        }
    } else {
        printf("No se encontraron libros que contengan todas las palabras clave.\n");
    }

    // Liberar la memoria de las listas utilizadas
    cleanList(librosCoincidentes);
}

/*
void buscarLibroXTitulo(HashMap *mapaLibros) {
    char palabras[1000];
    char delimitador= [];
    printf("Ingrese palabras clave para buscar libros");
    scanf("%999[^\n]s", palabras);

    char *token = strtok(palabras, delimitador);
    Libro *libro = (Libro *)pair->value;
    HashMap *mapaDePalabras = libro->mapaDePalabras;
    while(token != NULL){


    }

}*/

//                                        (4)
// Funciona pero maomenos mal

/*int greater_than_int(const void *a, const void *b) {
    int num1 = *((int *)a);
    int num2 = *((int *)b);

    if (num1 < num2) return 1;
    if (num1 > num2) return -1;
    return 0;
}

void palabrasConMayorFrecuencia(HashMap* mapaLibros) {
    char auxId[101];

    printf("Ingrese el ID del libro:\n");
    scanf("%s", auxId);

    Pair *contentAux = searchMap(mapaLibros, auxId);
    if (contentAux == NULL) {
        printf("El libro con ID '%s' no se encontró.\n", auxId);
        return;
    }

    Libro *auxContenido = (Libro *)contentAux->value;
    HashMap *mapaConteoDePalabras = auxContenido->mapaConteoDePalabras;
    int totalPalabras = auxContenido->cantidadPalabras;

    if (mapaConteoDePalabras == NULL || totalPalabras == 0) {
        printf("El libro no contiene palabras para analizar.\n");
        return;
    }

    List *listaFrecuencias = createList();




}*/


/* // Crear una lista para almacenar pares de palabras y frecuencias
    List *listaFrecuencias = createList();

    // Recorrer el mapa de palabras para calcular las frecuencias
    Pair *pair = firstMap(mapaDePalabras);
    while (pair != NULL) {
        char *palabra = pair->key;
        int *frecuencia = (int *)pair->value;
        double frecuenciaRelativa = (double)(*frecuencia) / totalPalabras;

        // Crear una cadena formateada con la palabra y su frecuencia
        char cadenaFrecuencia[256];
        snprintf(cadenaFrecuencia, sizeof(cadenaFrecuencia), "%s (%d veces, %.2f%%)", palabra, *frecuencia, frecuenciaRelativa * 100);

        // Agregar la cadena formateada a la lista
        pushBack(listaFrecuencias, cadenaFrecuencia);

        pair = nextMap(mapaDePalabras);
    }

    // Ordenar la lista de frecuencias de mayor a menor
    //ordenarLista(listaFrecuencias);

    printf("Las 10 palabras con mayor frecuencia en el libro '%s' son:\n", auxContenido->titulo);
    int contador = 0;

    // Mostrar las 10 palabras con mayor frecuencia
    while (contador < 10) {
        char *frecuencia = (char *)popBack(listaFrecuencias);
        if (frecuencia == NULL) {
            break; 
        }
        printf("%d. %s\n", contador + 1, frecuencia);
        contador++;
    }

    // Liberar la lista de frecuencias
    cleanList(listaFrecuencias);*/



// Función para mostrar las palabras con mayor frecuencia en un libro
void palabrasConMayorFrecuencia(HashMap *mapaIdes) {
    char auxtitulo[101];

    printf("Ingrese el ID del libro:\n");
    scanf("%s", auxtitulo);

    Pair *contentAux = searchMap(mapaIdes, auxtitulo);
    if (contentAux == NULL) {
        printf("El libro con ID '%s' no se encontró.\n", auxtitulo);
        return;
    }

    Libro *auxContenido = (Libro *)contentAux->value;
    HashMap *mapaConteoDePalabras = auxContenido->mapaConteoDePalabras;
    int totalPalabras = auxContenido->cantidadPalabras;

    if (mapaConteoDePalabras == NULL || totalPalabras == 0) {
        printf("El libro no contiene palabras para analizar.\n");
        return;
    }

    // Crear un Heap (montículo) para mantener las palabras con sus frecuencias relativas
    Heap *monticulo = createHeap();

    // Iterar a través del mapa de conteo de palabras y agregar las palabras al montículo
    Pair *pair = firstMap(mapaConteoDePalabras);
    while (pair != NULL) {
        char *palabra = (char *)pair->key;
        int frecuencia = *((int *)pair->value);

        // Calcular la frecuencia relativa
        double frecuenciaRelativa = (double)frecuencia / totalPalabras;

        // Agregar la palabra con su frecuencia al montículo
        heap_push(monticulo, palabra, frecuenciaRelativa * 10000.0); // Multiplicar por 10000 para convertir la frecuencia en un valor entero

        pair = nextMap(mapaConteoDePalabras);
    }

    // Mostrar las 10 palabras con mayor frecuencia
    printf("Las 10 palabras con mayor frecuencia en el libro '%s' son:\n");
    for (int i = 0; i < 10; i++) {
        if (monticulo->size == 0) {
            break;  // Si no hay más elementos en el montículo, salir del bucle
        }
        char *palabra = (char *)heap_top(monticulo);
        double frecuenciaRelativa = monticulo->heapArray[0].priority / 10000.0; // Convertir la prioridad de vuelta a valor decimal

        // Imprimir la palabra y su frecuencia
        printf("%d. Palabra: %s, Frecuencia: %.4lf\n", i + 1, palabra, frecuenciaRelativa);

        heap_pop(monticulo);
    }

    // Liberar el montículo
    free(monticulo);
}


//                                        (5)
// Apagalo otto!
void mostrarPalabrasRelevantes(HashMap* mapaLibros) {
    char titulo[101];

    printf("Ingrese el título del libro: ");
    scanf(" %100[^\n]", titulo);

    // Buscar el libro por título
    Pair *contentAux = firstMap(mapaLibros);
    while (contentAux != NULL) {
        Libro *auxContenido = (Libro *)contentAux->value;
        if (strcmp(auxContenido->titulo, titulo) == 0) {
            // Encontrado el libro
            HashMap *mapaDePalabras = auxContenido->mapaDePalabras;
            int totalPalabrasLibro = auxContenido->cantidadPalabras;

            // Crear un montículo para almacenar pares de palabras y sus valores TF-IDF
            Heap *monticulo = createHeap();

            // Recorrer el mapa de palabras del libro
            Pair *pair = firstMap(mapaDePalabras);
            while (pair != NULL) {
                char *palabra = pair->key;
                int vecesEnLibro = *((int *)pair->value);

                // Calcular IDF (Inverse Document Frequency)
                int documentosConPalabra = 0;

                // Recorrer la colección de libros nuevamente para contar los documentos que contienen la palabra
                Pair *content = firstMap(mapaLibros);
                int numDocumentosProcesados = 0; // Número total de documentos procesados
                while (content != NULL) {
                    HashMap *mapaDePalabrasDocumento = ((Libro *)content->value)->mapaDePalabras;
                    if (searchMap(mapaDePalabrasDocumento, palabra) != NULL) {
                        documentosConPalabra++;
                    }
                    numDocumentosProcesados++; // Incrementar el número de documentos procesados
                    content = nextMap(mapaLibros);
                }
                int idf = (int)log((double)(numDocumentosProcesados) / (double)(documentosConPalabra + 1)) + 1;

                // Calcular TF-IDF
                double tfidf = ((double)vecesEnLibro / (double)totalPalabrasLibro) * idf;

                // Insertar el valor TF-IDF en el montículo con la palabra como clave
                heap_push(monticulo, palabra, tfidf);

                pair = nextMap(mapaDePalabras);
            }

            printf("Las 10 palabras más relevantes en el libro '%s' son:\n", auxContenido->titulo);
            int cont = 0;

            // Mostrar las 10 palabras más relevantes desde el montículo
          while ((cont < 10) && (monticulo->size > 0)) {
              char *palabra = (char *)heap_top(monticulo);
              double tfidf = monticulo->heapArray[0].priority;
              printf("%d. %s (TF-IDF: %.4f)\n", cont + 1, palabra, tfidf);
              heap_pop(monticulo);
                cont++;
          }

            // Liberar el montículo
            free(monticulo);

            return; 
        }

        contentAux = nextMap(mapaLibros);
    }

    printf("El libro con título '%s' no se encontró.\n", titulo);
}

//                                        (6)
// tiene bucle al momento de mostrar por pantalla
void buscarXPalabra(HashMap *mapaLibros) {
    char palabra[101];
    printf("Ingrese la palabra clave a buscar: ");
    scanf(" %100[^\n]", palabra);

    // Crear un TreeMap para almacenar los libros ordenados por relevancia
    TreeMap *librosOrdenados = createTreeMap(lower_than_int); // Ordenar de mayor a menor relevancia

    Pair *contentAux = firstMap(mapaLibros);

    // Recorrer el mapa de libros y calcular la relevancia de la palabra en cada libro
    while (contentAux != NULL) {
        Libro *auxContenido = (Libro *)contentAux->value;
        HashMap *mapaDePalabras = auxContenido->mapaDePalabras;
        int totalPalabrasLibro = auxContenido->cantidadPalabras;

        // Calcular la relevancia de la palabra en el libro
        Pair *palabraRelevancia = searchMap(mapaDePalabras, palabra);
        double relevancia = 0.0;
        if (palabraRelevancia != NULL) {
            int vecesEnLibro = *((int *)palabraRelevancia->value);
            relevancia = (double)vecesEnLibro / totalPalabrasLibro;
        }

        // Agregar el libro y su relevancia al TreeMap
        insertTreeMap(librosOrdenados, &relevancia, auxContenido);

        contentAux = nextMap(mapaLibros);
    }

    // Mostrar los libros ordenados por relevancia
    Pair *pair = firstTreeMap(librosOrdenados);
    printf("Libros con la palabra clave '%s', ordenados por relevancia:\n", palabra);
    while (pair != NULL) {
        Libro *libro = (Libro *)pair->value;
        printf("ID: %s\n", libro->ID);
        printf("Título: %s\n", libro->titulo);
        printf("Relevancia: %.4lf\n", *((double *)pair->key));
        printf("\n");

        // Avanza al siguiente par en el TreeMap
        pair = nextTreeMap(librosOrdenados);
    }

    // Liberar la memoria del TreeMap
    free(librosOrdenados);
}



//                                        (7)
// Lista (tiene un detalle en con las lineas pero cumple la función)
void palabraEnContextoDelLibro(HashMap *mapaLibros) {
    char titulo[101];
    char palabra[101];
    int cont=1;

    printf("Ingrese el título del libro: ");
    scanf(" %100[^\n]", titulo);
    printf("Ingrese la palabra a buscar: ");
    scanf(" %100[^\n]", palabra);

    // Buscar el libro por título
    Pair *contentAux = firstMap(mapaLibros);
    while (contentAux != NULL) {
        Libro *auxContenido = (Libro *)contentAux->value;
        if (strcmp(auxContenido->titulo, titulo) == 0) {
            // Encontrado el libro
            char **lineas = auxContenido->lineas;
            int cantidadLineas = auxContenido->cantidadLineas;
            int encontrada = 0;

            // Buscar la palabra en el contexto del documento
            printf("\n-------------------------------------------------------\n");
            for (int i = 0; i < cantidadLineas; i++) {
                char *linea = lineas[i];
                char *lowerLinea = strdup(linea); // Hacer una copia de la línea en minúsculas para la búsqueda
                toLowerCase(lowerLinea);
                if (strstr(lowerLinea, palabra) != NULL) {
                    encontrada = 1;

                    printf("Aparición N°'%i' de la palabra '%s' en el contexto:\n", cont,palabra);
                    printf("-------------------------------------------------------\n\n");
                    cont++;
                    // Mostrar la línea anterior, actual y siguiente
                    int inicio = (i > 1) ? i - 1 : 0;
                    int fin = (i < cantidadLineas - 2) ? i + 1 : cantidadLineas - 1;
                    for (int j = inicio; j <= fin; j++) {
                        printf("%s\n", lineas[j]);
                    }
                    printf("-------------------------------------------------------\n");
                }
                free(lowerLinea); // Liberar la memoria de la línea en minúsculas
            }

            if (!encontrada) {
                printf("La palabra clave '%s' no se encontró en el libro '%s'.\n", palabra, titulo);
                printf("-------------------------------------------------------\n");
            }

            return;
        }

        contentAux = nextMap(mapaLibros);
    }

    printf("El libro con título '%s' no se encontró.\n", titulo);
}

//
void liberarMemoria(HashMap *mapaOrdenado,HashMap *mapaIdes){
    free(mapaOrdenado);
    free(mapaIdes);
}

//----------------------------------------------------------------
//                   Funciones de implementación                 |
//----------------------------------------------------------------
//
void mostrarOpciones(){           
  printf("\n╔═════════════════════════════════════════════════════════════╗\n");
  printf("║                            MENÚ                             ║\n");
  printf("╚═════════════════════════════════════════════════════════════╝\n");
  printf("╔═════════════════════════════════════════════════════════════╗\n");
  printf("║ 1. Cargar documentos                                        ║\n");
  printf("║ 2. Mostrar documentos ordenados                             ║\n");
  printf("║ 3. Buscar un libro por título                               ║\n");
  printf("║ 4. Palabras con mayor frecuencia                            ║\n");
  printf("║ 5. Palabras más relevantes                                  ║\n");
  printf("║ 6. Buscar por palabra                                       ║\n");
  printf("║ 7. Mostrar palabra en su contexto dentro del libro          ║\n");
  printf("║ 0. Salir                                                    ║\n");
  printf("║                                                             ║\n");
  printf("╚═════════════════════════════════════════════════════════════╝\n\n");
  printf("Seleccione una opción: ");
}

//
void mostrarMenu(HashMap* mapaLibro,HashMap*mapaIdes){  

  unsigned short opciones;
  while(1){
      char opcion[2]; 
      mostrarOpciones();
      scanf("%1s", opcion);
      getchar();

      if (isdigit(opcion[0])){
          opciones = atoi(opcion);

          switch(opciones){
              case 1:
                cargarDocumentos(mapaLibro, mapaIdes);
                break;

              case 2:
                mostrarDocumentosOrdenados(mapaLibro);
                break;

              case 3:
                buscarLibroXTitulo(mapaLibro);
                break;

              case 4:
                palabrasConMayorFrecuencia(mapaIdes);
                break;

              case 5:
                mostrarPalabrasRelevantes(mapaLibro);
                break;

              case 6:
                buscarXPalabra(mapaLibro);
                break;

              case 7:
                palabraEnContextoDelLibro(mapaLibro);
                break;

              case 0:
                printf("Saliendo de la aplicación\n");
                return;

              default:
                printf("Opción no valida ingrese nuevamente\n");
          }
      }
      else{
          printf("\nopción no válida. Ingrese un número.\n");
      }
  }
}