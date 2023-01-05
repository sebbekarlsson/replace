#include <ctype.h>
#include <linux/limits.h>
#include <replace/args.h>
#include <replace/iter.h>
#include <replace/macros.h>
#include <replace/replace.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <text/text.h>
#include <unistd.h>

static int get_extension(const char *path, char *out) {
  if (!path)
    return 0;
  char *dot = strrchr(path, '.');
  if (!dot)
    return 0;

  int64_t len = strlen(path);
  int64_t dot_pos = dot - path;
  int64_t dot_len = len - dot_pos;

  memcpy(&out[0], dot, dot_len);

  return 1;
}

static bool can_replace_file(const char *path) {
  if (strstr(path, ".git/") != 0)
    return false;

  char ext[32];
  memset(&ext[0], 0, 32 * sizeof(char));

  if (get_extension(path, ext)) {
    if (strcmp(ext, ".out") == 0 || strcmp(ext, ".o") == 0 ||
        strcmp(ext, ".a") == 0 || strcmp(ext, ".bin") == 0 ||
        strcmp(ext, ".so") == 0 || strcmp(ext, ".dll") == 0)
      return false;
  }

  FILE *fp = fopen(path, "rb");
  if (!fp)
    return false;

  fseek(fp, 0L, SEEK_END);
  int64_t sz = ftell(fp);
  rewind(fp);

  if (sz <= 0) {
    fclose(fp);
    return false;
  }

  int checklen = MIN(sz, 16);

  char buff[checklen];
  fread(&buff[0], sizeof(char), checklen, fp);

  for (int i = 0; i < checklen; i++) {
    int c = (int)buff[i];

    bool is_bad = (c <= 0 || c >= 127);

    if (is_bad) {
      fclose(fp);
      return false;
    }
  }

  return true;
}

bool replace_file_exists(const char *path) { return access(path, F_OK) == 0; }

char *replace_get_file_contents(const char *filepath) {
  if (!replace_file_exists(filepath)) {
    fprintf(stderr, "Error: No such file %s\n", filepath);
    return 0;
  }
  FILE *fp = fopen(filepath, "r");
  char *buffer = NULL;
  size_t len;
  ssize_t bytes_read = getdelim(&buffer, &len, '\0', fp);
  if (bytes_read == -1) {
    printf("Failed to read file `%s`\n", filepath);
    return 0;
  }

  fclose(fp);

  return buffer;
}

static bool prompt_user(const char *message, bool *all) {
  printf("%s (Y/N/A)?: ", message);
  char ans = 0;
  scanf("%c", &ans);
  fflush(stdin);

  *all = (ans == 'a' || ans == 'A');
  return ans == 'y' || ans == 'Y';
}

static void replace_callback(const char *path, ReplaceState *state) {
  ReplaceArgs args = state->args;

  if (!can_replace_file(path)) {
    printf("Skipping `%s`\n", path);
    return;
  }

  char *contents = replace_get_file_contents(path);

  if (!contents) {
    printf("Skipping `%s` (failed to open.)\n", path);
    return;
  }

  char message[PATH_MAX];
  sprintf(message, "Replace contents of `%s`? (pattern=`%s`)", path,
          args.pattern);
  if (state->all || prompt_user(message, &state->all)) {

    if (!strstr(contents, args.pattern)) {
      printf("Pattern not found in `%s`.\n", path);
      return;
    }
    char *next_contents = text_replace(contents, args.pattern, args.repl);

    if (!next_contents) {
      printf("Failed to replace contents of `%s`.\n", path);
      return;
    }

    FILE *fp = fopen(path, "w");
    if (!fp) {
      fprintf(stderr, "Failed to open `%s`\n", path);
      return;
    }

    fwrite(&next_contents[0], sizeof(char), strlen(next_contents), fp);

    fclose(fp);

    printf("Successfully replaced contents of `%s`\n", path);
  }

  free(contents);
}

int replace(ReplaceState *state) {
  ReplaceArgs args = state->args;
  if (!args.path) {
    fprintf(stderr, "No path specified.\n");
    return 0;
  }

  if (!args.pattern) {
    fprintf(stderr, "No replace pattern specified.\n");
    return 0;
  }

  if (!args.repl) {
    fprintf(stderr, "No replacement specified.\n");
    return 0;
  }

  replace_listdir(args.path, replace_callback, state);

  return 1;
}
