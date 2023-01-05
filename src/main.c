#include "replace/args.h"
#include <stdio.h>
#include <stdlib.h>
#include <replace/replace.h>

int main(int argc, char *argv[]) {

  if (argc < 4) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "replace <path> <pattern> <replacement>\n");
    return 1;
  }

  ReplaceArgs args = {0};
  args.path = argv[1];
  args.pattern = argv[2];
  args.repl = argv[3];

  ReplaceState state = {0};
  state.args = args;


  if (!replace(&state)) {
    fprintf(stderr, "Failed to replace.\n");
    return 1;
  }

  return 0;
  
}
