#include <dirent.h>
#include <replace/iter.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <replace/iter.h>


static bool is_dir(const char *path) {
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
    return 0;
  return S_ISDIR(statbuf.st_mode);
}

int replace_listdir(const char *path, ReplaceCallback callback, ReplaceState* state) {
  struct dirent *entry;
  DIR *dp;

  ReplaceArgs args = state->args;

  dp = opendir(path);
  if (dp == NULL) {
    perror("opendir: Path does not exist or could not be read.");
    return -1;
  }

  while ((entry = readdir(dp))) {
    const char *name = entry->d_name;
    if (!name) continue;
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;

    if (args.path_pattern != 0 && strstr(name, args.path_pattern) == 0) continue;

    char fullpath[PATH_MAX];
    if (path[strlen(path) - 1] != '/' && name[0] != '/') {
      sprintf(fullpath, "%s/%s", path, name);
    } else {
      sprintf(fullpath, "%s%s", path, name);
    }

    if (entry->d_type == DT_DIR) {
      replace_listdir(fullpath, callback, state);
    } else {
      callback(fullpath, state);
    }
  }

  closedir(dp);
  return 0;
}
