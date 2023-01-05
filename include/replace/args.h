#ifndef REPLACE_ARGS_H
#define REPLACE_ARGS_H
#include <stdbool.h>
typedef struct {
  const char* path_pattern;
  const char* pattern;
  const char* repl;
  const char* path;
} ReplaceArgs;

typedef struct {
  ReplaceArgs args;
  bool all;
} ReplaceState;
#endif
