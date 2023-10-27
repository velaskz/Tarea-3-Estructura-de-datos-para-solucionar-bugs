#ifndef TREEMAP_h
#define TREEMAP_h

#include"hashmap.h"

typedef struct TreeMap TreeMap;

int compare(const void * key1, const void * key2);

TreeMap * createTreeMap(int (*lower_than_int) (void* key1, void* key2));

void insertTreeMap(TreeMap * tree, void* key, void * value);

void eraseTreeMap(TreeMap * tree, void* key);

Pair * searchTreeMap(TreeMap * tree, void* key);

Pair * upperBound(TreeMap * tree, void* key);

Pair * firstTreeMap(TreeMap * tree);

Pair * nextTreeMap(TreeMap * tree);

int lower_than_int(void* key1, void* key2);


#endif /* TREEMAP_h */
