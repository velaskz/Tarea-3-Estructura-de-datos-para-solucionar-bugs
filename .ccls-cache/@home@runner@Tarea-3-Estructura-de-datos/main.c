#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionesAnswers.h"
#include "list.h"
#include "hashmap.h"
#include "heap.h"

int main(){
    HashMap* mapaLibro = createMap(1001);
    HashMap* mapaIdes= createMap(1001);
    mostrarMenu(mapaLibro,mapaIdes);
    liberarMemoria(mapaLibro, mapaIdes);
  return 0;
}