#ifndef KV_HASHMAP_H
#define KV_HASHMAP_H

#include "kv_string.h"
#include <stdint.h>
#include <stdlib.h>

//
// structs
typedef struct kv_entry {
  kv_string_t *key;
  kv_string_t *value;
  uint64_t hash;
  struct kv_entry *next; // references next node
} kv_entry_t;

typedef struct kv_hashmap {
  kv_entry_t **buckets;
  size_t capacity;
  size_t size;
} kv_hashmap_t;

//
// function prototypes
/**
 * @brief Computes 64-bit FNV-1a hash over raw bytes.
 *     Responsibilities:
 *     1. Initialize 64-bit hash with FNV offset basis
 (0xcbf29ce484222325ULL).
 *     2. For each byte in buffer:
 *        a. XOR byte into lower hash bits.
 *        b. Multiply hash by 64-bit FNV prime (0x100000001b3ULL).
 *     3. Return computed 64-bit hash.
 *     4. Ensure binary safety: treat data as unsigned byte stream.
 *
 * @param data Read-only pointer to raw byte sequence.
 * @param len  Byte count to hash.
 * @return uint64_t Computed 64-bit hash.
*/
uint64_t kv_hash_fnv1a(const uint8_t *data, size_t len);

/**
 * @brief Allocates and initializes new hash map.
 *     Responsibilities:
 *     1. Allocate memory for kv_hashmap_t struct itself.
 *     2. If struct allocation fails, return NULL.
 *     3. Allocate and zero-initialize bucket array using
 calloc(capacity, sizeof(kv_entry_t *)).
 *     4. If bucket allocation fails, free map struct and return NULL.
 *     5. Initialize map->capacity = capacity and map->size = 0.
 *     6. Return pointer to initialized map.
 *
 * @param capacity Initial number of bucket slots to allocate.
 * @return kv_hashmap_t* Pointer to newly allocated map, or NULL on OOM.
*/
kv_hashmap_t *kv_hashmap_new(size_t capacity);

/**
 * @brief Deallocates all memory associated with hash map.
 *     Responsibilities:
 *     1. If map is NULL, return immediately (safe no-op).
 *     2. Iterate through each bucket index:
 *        a. Traverse linked list nodes.
 *        b. Free key and value strings via kv_string_free.
 *        c. Free node struct.
 *     3. Free buckets pointer array.
 *     4. Free map struct.
 *
 * @param map Pointer to hash map to free.
 */
void kv_hashmap_free(kv_hashmap_t *hashmap);

/**
 * @brief Inserts or updates key-value entry in hash map.
 *     Responsibilities:
 *     1. Compute 64-bit hash of key->data via kv_hash_fnv1a.
 *     2. Determine bucket index: hash % capacity.
 *     3. Search bucket chain:
 *        a. If node->hash matches AND key bytes match:
 *           - Free old value via kv_string_free.
 *           - Assign new value to node->value.
 *           - Return 0 (update success).
 *     4. If key is new:
 *        a. Allocate new kv_entry_t node.
 *        b. Assign key, value, and cached hash.
 *        c. Prepend node to bucket chain.
 *        d. Increment map->size.
 *     5. If load factor (size / capacity) exceeds 0.75:
 *        a. Trigger dynamic rehashing (double capacity, re-index nodes).
 *     6. Return 0 on success, -1 on allocation failure.
 *
 * @param map   Pointer to hash map.
 * @param key   Pointer to key string.
 * @param value Pointer to value string.
 * @return int 0 on success, -1 on memory allocation error.
 */
int kv_hashmap_set(kv_hashmap_t *map, kv_string_t *key, kv_string_t *value);

/**
 * @brief Retrieves value associated with specified key.
 *     Responsibilities:
 *     1. If map or key is NULL, return NULL.
 *     2. Compute 64-bit hash of key->data via kv_hash_fnv1a.
 *     3. Determine bucket index: hash % capacity.
 *     4. Search bucket chain:
 *        a. Check node->hash == computed hash (fast-path rejection).
 *        b. If hash matches, compare binary strings (length and data
 bytes).
 *        c. If match found, return node->value.
 *     5. Return NULL if key not found.
 *
 * @param map Constant pointer to hash map.
 *
 * @param key Constant pointer to key to query.
 * @return kv_string_t* Pointer to matching value string, or NULL if absent.
*/
kv_string_t *kv_hashmap_get(const kv_hashmap_t *map, kv_string_t *key);

/**
 * @brief Removes key-value entry from hash map.
 *     Responsibilities:
 *     1. If map or key is NULL, return -1.
 *     2. Compute 64-bit hash and find target bucket index.
 *     3. Traverse bucket chain tracking previous node pointer:
 *        a. When matching node found:
 *           - Unlink node from chain.
 *           - Free key and value via kv_string_free.
 *           - Free node struct.
 *           - Decrement map->size.
 *           - Return 0 (deletion success).
 *     4. Return -1 if key not found.
 *
 * @param map Pointer to hash map.
 * @param key Constant pointer to key to remove.
 * @return int 0 if entry found and removed, -1 if not found.
 */
int kv_hashmap_delete(kv_hashmap_t *map, const kv_string_t *key);

/**
 * @brief Returns active entry count in map.
 *     Responsibilities:
 *     1. Return map ? map->size : 0 in O(1) time.
 *
 * @param map Constant pointer to hash map.
 * @return size_t Current active element count.
 */
size_t kv_hashmap_size(const kv_hashmap_t *map);
#endif