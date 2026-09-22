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

unsigned long Hash(char const *str);

void InitializeHashMap(HashMap* map);
int Insert(HashMap* map, char const * key, char const * value);
int Delete(HashMap* map, char const * key);
int Search(HashMap* map, char const *key, char const ** value);
int Exists(HashMap* map, char const *key);

#define HASH_KEY_NOT_FOUND -1

#define HASH_KEY_INSERTED 1
#define HASH_KEY_DELETED 2
#define HASH_KEY_FOUND 3
