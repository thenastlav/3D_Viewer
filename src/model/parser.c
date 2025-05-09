#include "parser.h"

const char *get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename) return "";
  return dot + 1;
}

bool is_obj_file(const char *filename) {
  const char *ext = get_filename_ext(filename);
  return strcmp(ext, "obj") == 0;
}

void count_vertices_faces(FILE *fp, size_t *n_v, size_t *n_f) {
  char line[BUFF];
  while (fgets(line, BUFF, fp)) {
    if (strncmp(line, "v ", 2) == 0) {
      (*n_v)++;
    } else if (strncmp(line, "f ", 2) == 0) {
      (*n_f)++;
    }
  }

  rewind(fp);
}

char *get_filecontent(const char *path) {
  FILE *fp = fopen(path, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Error openning file %s: ", path);
    perror("");
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  rewind(fp);

  char *buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    perror("Memory allocation error:");
    fclose(fp);
    return NULL;
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, fp);
  fclose(fp);

  if (bytes_read != (size_t)file_size) {
    fprintf(stderr, "Error reading file: %s\n", path);
    free(buffer);
    return NULL;
  }

  buffer[file_size] = '\0';
  return buffer;
}