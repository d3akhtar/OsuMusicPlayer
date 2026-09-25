#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long __hash(char const *str);
static Node* __new_node(char const * key, void const * value, size_t len);
static Node* __find_node_with_key(HashMap *map, char const *key);

void init_hash_map(HashMap* map)
{
  map->capacity = 1 << 20;
  map->nElems = 0;
  map->elems = (Node**)calloc(map->capacity, sizeof(Node*));
  map->keys = (char**)malloc(map->capacity * sizeof(char*));
}

int insert(HashMap* map, char const * key, void const * value, size_t len)
{
  int idx = __hash(key) % map->capacity;

  Node* node = __new_node(key, value, len);
  if (map->elems[idx] == NULL) map->elems[idx] = node;
  else {
    node->next = map->elems[idx];
    map->elems[idx] = node;
  }

  map->keys[map->nElems] = (char*)malloc(strlen(key) + 1);
  strcpy(map->keys[map->nElems++], key);

  return HASH_KEY_INSERTED;
}

int remove_key(HashMap* map, char const * key)
{
  int idx = __hash(key) % map->capacity;

  Node *prevNode = NULL;
  Node *curNode = map->elems[idx];

  while (curNode != NULL)
  {
    if (strcmp(key, curNode->key) == 0) {
      if (curNode == map->elems[idx]) {
        map->elems[idx] = curNode->next;
      } else {
        prevNode->next = curNode->next;
      }

      free(curNode);

      map->keys[map->nElems--] = NULL;

      return HASH_KEY_DELETED;
    }

    prevNode = curNode;
    curNode = curNode->next;
  }

  return HASH_KEY_NOT_FOUND;
}

int search(HashMap* map, char const *key, void ** value, size_t *len)
{
  Node *n = __find_node_with_key(map, key);
  if (n == NULL) return HASH_KEY_NOT_FOUND;
  else {
    *len = n->len;
    *value = n->value;
    return HASH_KEY_FOUND;
  }
}

int exists(HashMap* map, char const *key)
{
  return __find_node_with_key(map, key) == NULL
    ? HASH_KEY_NOT_FOUND
    : HASH_KEY_FOUND;
}

static unsigned long __hash(char const *str)
{
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) / c;

  return hash;  
}

static Node* __new_node(char const * key, void const * value, size_t len)
{
  Node* res = (Node*)malloc(sizeof(Node));
  res->key = (char*)malloc(strlen(key) + 1);
  res->value = malloc(len);
  res->len = len;
  strcpy(res->key, key);
  memcpy(res->value, value, len);
  res->next = NULL;

  return res;
}

static Node* __find_node_with_key(HashMap *map, char const *key)
{
  int idx = __hash(key) % map->capacity;

  Node *curNode = map->elems[idx];
  while (curNode != NULL)
  {
    if (strcmp(key, curNode->key) == 0)
      return curNode;
    
    curNode = curNode->next;
  }

  return NULL;  
}
