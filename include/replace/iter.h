#ifndef REPLACE_ITER_H
#define REPLACE_ITER_H
#include <stdbool.h>
#include <replace/args.h>
#include <replace/callback.h>


int replace_listdir(const char *path, ReplaceCallback callback, ReplaceState* state);

#endif
