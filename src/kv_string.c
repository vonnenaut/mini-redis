//
// Begin Headers /////
#include "kv_string.h"
#include <stdlib.h>
#include <string.h>

//
// Begin functions /////

/**
 * @brief Creates from C string.
 *     Responsibilities:
 *     1. Allocate memory for the kv_string_t struct itself.
 *     2. Allocate memory for the buffer: len + 1 bytes (to ensure space for
 * '\0' at end).
 *     3. Error Handling: If either malloc fails, make sure you don't leak
 * anything and return NULL.
 *     4. If init != NULL, copy len bytes into data (using memcpy) and set
 * data[len] = '\0'.
 *     5. If init == NULL, set data[0] = '\0'.
 *     6. Initialize str->len = len and str->cap = len.
 *     7. Return the pointer to the struct.
 *
 * @param *init
 */
kv_string_t *kv_string_new(const char *init) {
  size_t len = init ? strlen(init) : 0; // ternary operator 
  return kv_string_new_len(init, len);
}

/**
 * @brief Creates with specified length
 *
 * @param *init
 * @param len
 */
kv_string_t *kv_string_new_len(const char *init, size_t len) {
  // allocate struct
  kv_string_t *str = malloc(sizeof(kv_string_t));
 
  // struct out of memory (OOM) check 
  if(!str) {
    return NULL;
  }

  // allocate buffer
  str->data = malloc(len + 1);

  // buffer OOM check
  if(!str->data) {
    free(str);
    return NULL;
  }

  // copy and terminate string with '\0'
  if(init != NULL) {
    memcpy(str->data, init, len);
    str->data[len] = '\0';
  } else {
    str->data[0] = '\0';
  }

  // set metadata
  str->len = len;
  str->cap = len;

  return str;
}

/**
 * @brief Frees memory
 *
 * @param *str
 */
void kv_string_free(kv_string_t *str) {
  if(!str) {
    return;
  }

  free(str->data);
  free(str);
}

/**
 * @brief appends to
 *
 * @param *str
 * @param *suffix
 */
int kv_string_append(kv_string_t *str, const char *suffix) {
  if(!str || !suffix)
    return -1;

  size_t suffix_len = strlen(suffix);
  
  return kv_string_append_len(str, suffix, suffix_len); 
}

/**
 * @brief Appends raw bytes to
 *
 * @param *str
 * @param *data
 * @param len
 */
int kv_string_append_len(kv_string_t *str, const void *data, size_t len) {
  if(!str || !data) {
    return -1;
  } 

  if(len == 0) {
    return 0;
  }

  size_t required_capacity = str->len + len;
  size_t new_capacity = 0;
  char *temp_buffer = NULL;

  if(required_capacity > str->cap) {
    new_capacity = str->cap * 2;

    if(new_capacity < required_capacity)
      new_capacity = required_capacity;

    temp_buffer = realloc(str->data, new_capacity + 1);

    if(!temp_buffer)
      return -1;

    str->data = temp_buffer;
    str->cap = new_capacity;
  }

  memcpy(&str->data[str->len], data, len);

  str->len = required_capacity;
  str->data[str->len] = '\0';

  return 0; 
}

//
// END of File
