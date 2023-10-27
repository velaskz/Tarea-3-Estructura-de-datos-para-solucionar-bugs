#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"


void* heap_top(Heap* pq){
  if(pq==NULL||pq->size==0){
    return NULL;    
  }
  else{
    return pq->heapArray[0].data;
  }
}

int parent(int i){
  return (i-1)/2;
}

void swap(heapElem* x, heapElem* y){
  heapElem aux=*x;
  *x=*y;
  *y=aux;
}

void heap_push(Heap* pq, void* data, int priority){
  heapElem newElem;
  newElem.data=data;
  newElem.priority=priority;
  if(pq->size==pq->capac){
    pq->capac=((pq->capac)*2)+1;
    pq->heapArray=realloc(pq->heapArray, pq->capac*sizeof(heapElem));
  }
  int i=pq->size;
  pq->heapArray[i]=newElem;
  pq->size++;

  while(i!=0&&pq->heapArray[parent(i)].priority < pq->heapArray[i].priority){
    swap(&(pq->heapArray[i]), &(pq->heapArray[parent(i)]));
    i=(i-1)/2;
  }
}

void heap_pop(Heap* pq){
  pq->heapArray[0] = pq->heapArray[pq->size - 1];
  pq->size--;

  int i=0;
  int izquierda=1;
  int derecha=2;
  int largo=i;

  while(1){

    if((izquierda < pq->size)&&(pq->heapArray[izquierda].priority > pq->heapArray[largo].priority)){
      largo= izquierda;
    }
    if((derecha < pq->size)&&(pq->heapArray[derecha].priority > pq->heapArray[largo].priority)){
      largo =derecha;
    }
    if(largo==i){
      break;
    }
    swap(&(pq->heapArray[i]),&(pq->heapArray[largo]));
    i=largo;
    break;
  }
}

Heap* createHeap(){
  Heap* NewHeap= (Heap*)malloc(sizeof(Heap));
  NewHeap->size=0;
  NewHeap->capac=10;
  NewHeap->heapArray=(heapElem*)malloc(NewHeap->capac*sizeof(heapElem));
  return NewHeap;
}
