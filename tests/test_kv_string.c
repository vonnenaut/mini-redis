#include "kv_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
 * TEST 1: test_creation_and_free
      - Create string from literal: "Hello, Redis!"
      - Assert str is not NULL
      - Assert str->len == 13
      - Assert str->cap >= 13
      - Assert strcmp(str->data, "Hello, Redis!") == 0
      - Free string
*/
void test_new_and_free() {
  kv_string_t *str = kv_string_new("hello");

  assert(str);
  assert(str->len == 5);
  assert(str->cap >= 5);
  assert(strcmp(str->data, "hello") == 0);
  kv_string_free(str);
}

/*
    TEST 2: test_null_and_empty
      - Create string with NULL: kv_string_new(NULL)
      - Assert str->len == 0 and str->data[0] == '\0'
      - Free string
      - Create string with "": kv_string_new("")
      - Assert str->len == 0 and str->data[0] == '\0'
      - Free string
*/
void test_null_and_empty() {
  kv_string_t *nullstr = kv_string_new(NULL);
  assert(nullstr->len == 0 && nullstr->data[0] == '\0');
  kv_string_free(nullstr);

  kv_string_t *emptystr = kv_string_new("");
  assert(emptystr->len == 0 && emptystr->data[0] == '\0');
  kv_string_free(emptystr);
}

/*
    TEST 3: test_append
      - Create string: "Hello"
      - Append: " World"
      - Assert str->len == 11
      - Assert strcmp(str->data, "Hello World") == 0
      - Free string
*/
void test_append() {
  kv_string_t *str = kv_string_new("Hello");
  kv_string_append(str, " World");
  assert(str->len == 11);
  assert(strcmp(str->data, "Hello World") == 0);
  kv_string_free(str);
}

/*
    TEST 4: test_growth_and_capacity
      - Create string: "a"
      - In a loop of 1,000 iterations:
          append "b" to the string
      - Assert str->len == 1001
      - Assert str->cap >= 1001
      - Free string
*/
void test_growth_and_capacity() {
  kv_string_t *str = kv_string_new("a");

  for (int i = 0; i < 1000; i += 1) {
    kv_string_append(str, "b");
  }

  assert(str->len == 1001);
  assert(str->cap >= 1001);
  kv_string_free(str);
}

/*
    TEST 5: test_binary_safety
      - Create a buffer containing embedded null bytes: "foo\0bar" (length 7)
      - Create string with kv_string_new_len("foo\0bar", 7)
      - Assert str->len == 7
      - Append raw bytes: "\0baz" (length 4)
      - Assert str->len == 11
      - Assert Enabledmemcmp(str->data, "foo\0bar\0baz", 11) == 0
      - Free string
*/
void test_binary_safety() {
  kv_string_t *str = kv_string_new_len("foo\0bar", 7);
  // printf("str->len: %li", str->len);
  assert(str->len == 7);
  kv_string_append_len(str, "\0baz", 4);
  assert(str->len == 11);
  assert(memcmp(str->data, "foo\0bar\0baz", 11) == 0);
  kv_string_free(str);
}

int main() {
  test_new_and_free();
  test_null_and_empty();
  test_append();
  test_growth_and_capacity();
  test_binary_safety();

  printf("\n\nAll kv_string unit tests passed successfully!\n\n\n");
}