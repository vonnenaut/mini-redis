#include "kv_hashmap.h"
#include "kv_string.h"
#include <stdint.h>
#include <stdlib.h>

uint64_t kv_hash_fnv1a(const uint8_t *data, size_t len) {
  const uint64_t fnv_offset_basis = 0xcbf29ce484222325ULL;

  if (len == 0 || data == NULL)
    // TODO: return initial offset basis
    return fnv_offset_basis;

  const uint64_t fnv_prime_const = 0x100000001b3ULL;

  uint64_t hash = fnv_offset_basis;

  for (size_t i = 0; i < len - 1; i++) {
    hash = hash ^ data[i];
    hash = hash * fnv_prime_const;
  }
}

kv_hashmap_t *kv_hashmap_new(size_t capacity) {
  kv_hashmap_t *new_entry = malloc(capacity);

  if (new_entry != NULL) {
    return new_entry;
  } else
    return NULL;
}

void kv_hashmap_free(kv_hashmap_t *hashmap) {
  // free(hashmap);
}

int kv_hashmap_set(kv_hashmap_t *map) {}

kv_string_t *kv_hashmap_get() {}

int kv_hashmap_delete(kv_hashmap_t *map) {}

size_t return_active_count(const kv_hashmap_t *map) {}