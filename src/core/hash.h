#pragma once

typedef struct Node {
  char * key;
  char * value;
  struct Node* next;
} Node;

typedef struct HashMap {
  int nElems, capacity;
  Node** elems;
  char** keys;
} HashMap;

void init_hash_map(HashMap* map);
int insert(HashMap* map, char const * key, char const * value);
int delete_key(HashMap* map, char const * key);
int search(HashMap* map, char const *key, char const ** value);
int exists(HashMap* map, char const *key);

#define HASH_KEY_NOT_FOUND -1

#define HASH_KEY_INSERTED 1
#define HASH_KEY_DELETED 2
#define HASH_KEY_FOUND 3
