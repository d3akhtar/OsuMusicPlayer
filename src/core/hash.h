#pragma once

#include <stddef.h>

typedef struct Node {
  char * key;
  void * value;
  size_t len;
  struct Node* next;
} Node;

typedef struct HashMap {
  int nElems, capacity;
  Node** elems;
  char** keys;
} HashMap;

void init_hash_map(HashMap* map);
int insert(HashMap* map, char const * key, void const * value, size_t len);
int delete_key(HashMap* map, char const * key);
int search(HashMap* map, char const *key, void ** value, size_t *len);
int exists(HashMap* map, char const *key);

#define HASH_KEY_NOT_FOUND -1

#define HASH_KEY_INSERTED 1
#define HASH_KEY_DELETED 2
#define HASH_KEY_FOUND 3
