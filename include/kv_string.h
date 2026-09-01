#include <stddef.h>

#ifndef KV_STRING_H
#define KV_STRING_H

//
// structs /////
typedef struct {
  size_t len; // Current length of the string (excluding null terminator)
  size_t cap; // Total allocated capacity (exluding null terminator)
  char *data; // Pointer to heap-allocated char buffer
} kv_string_t;

//
// function prototypes /////

// create from C string
kv_string_t *kv_string_new(const char *init);

// create with specified length
kv_string_t *kv_string_new_len(const char *init, size_t len);

// free memory
void kv_string_free(kv_string_t *str);

// append to
int kv_string_append(kv_string_t *str, const char *suffix);

// append raw bytes to
int kv_string_append_len(kv_string_t *str, const void *data, size_t len);

#endif
