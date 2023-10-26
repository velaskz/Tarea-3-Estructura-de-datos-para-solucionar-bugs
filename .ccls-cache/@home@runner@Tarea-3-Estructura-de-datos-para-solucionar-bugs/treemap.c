#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);

};

int lower_than_int(void* key1, void* key2) {
    const char* str1 = (const char*)key1;
    const char* str2 = (const char*)key2;

    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] < str2[i]) {
            return 1;  // key1 es menor que key2
        } else if (str1[i] > str2[i]) {
            return 0;  // key2 es menor que key1
        }
        i++;
    }

    // Si llegamos aquí, una cadena es prefijo de la otra
    // En ese caso, la cadena más corta es "menor"
    if (str1[i] == '\0') {
        return 1;  // key1 es menor que key2
    }

    return 0;  // Las cadenas son idénticas
}




int is_equal_tree(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap * mapa = (TreeMap *)malloc(sizeof(TreeMap));
    if (mapa == NULL){
        return NULL;  
    } 
    mapa->root = NULL;
    mapa->current = NULL;
    mapa->lower_than = lower_than;
    return mapa;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL){
        return;
    }

    if (searchTreeMap(tree, key) != NULL) {
        return;
    }

    TreeNode* nuevoNodo = createTreeNode(key, value);
    if (nuevoNodo == NULL){
        return; 
    }

    if (tree->root == NULL){
        tree->root = nuevoNodo;
    } else{
        TreeNode* current = tree->root;
        TreeNode* parent = NULL;

        while (current != NULL){
            parent = current;

            int comparacion = tree->lower_than(current->pair->key, key);

            if (comparacion == 1){
                current = current->right;
            } else{
                current = current->left;
            }
        }

        if (tree->lower_than(parent->pair->key, key) == 1) {
            parent->right = nuevoNodo;
        } else {
            parent->left = nuevoNodo;
        }
        nuevoNodo->parent = parent;
    }

    tree->current = nuevoNodo;   
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) {
        return NULL; 
    }

    TreeNode* current = x;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL){
        return;
    }

    TreeNode* parent = node->parent;
    if(node->left == NULL && node->right == NULL){
        if (parent != NULL){
            if (parent->left == node){
                parent->left = NULL;
            }
            else{
                parent->right = NULL;
            }
        }
        else{
            tree->root = NULL;
        }
        free(node);
    }

    else if (node->left != NULL && node->right == NULL) {
        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = node->left;
            } else {
                parent->right = node->left;
            }
        } else {
            tree->root = node->left;
        }
        node->left->parent = parent;
        free(node);
    }

    else if (node->left == NULL && node->right == NULL){
        if(parent != NULL){
            if(parent->right == node){
                parent->right = parent->right;
            } else{
                parent->left = node->right;
            }
        }else{
            tree->root = node->right;
        }
        node->right->parent = parent;
        free(node);
    }

    else{
        TreeNode * menorSubDerecho = minimum(node->right);
        node->pair = menorSubDerecho->pair;
        removeNode(tree,menorSubDerecho);
        return;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* parent = NULL; 

    while(current != NULL){
        if(is_equal_tree(tree, current->pair->key, key)){
            tree->current = current;
            return current->pair;
        }
        else{
            parent = current;

            int comparacion = tree->lower_than(current->pair->key, key);

            if (comparacion == 1){
                current = current->right;
            }
            else{
                current = current->left;
            }
        }  

    }
    tree->current = parent;
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    Pair* buscado = searchTreeMap(tree, key);
    if (buscado != NULL) {
        return buscado; 
    }

    if (!tree->lower_than(tree->current->pair->key, key)) {
        return tree->current->pair;
    }

    Pair* siguiente = nextTreeMap(tree);
    if (siguiente != NULL) {
        return siguiente; 
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL){
      return NULL;  
    }
    TreeNode* current = tree->root;

    while(current->left != NULL){
        current = current->left;
    }
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL){
        return NULL;
    }

    if (tree->current->right != NULL){
        TreeNode* minimo = minimum(tree->current->right);
        tree->current = minimo;
        return minimo->pair;
    }

    TreeNode * current = tree->current;

    while (current != NULL){
    if (current->parent != NULL && tree->lower_than(current->pair->key,current->parent->pair->key)){
        tree->current = current->parent;
        return current->parent->pair;
        }
        current = current->parent;    
    }
    return NULL;  
}