#include "object.h"

Object *create_obj(const char *filename) {
  if (!is_obj_file(filename)) {
    fputs("Incorrect file format.", stderr);
    return NULL;
  }

  FILE *file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Error openning file %s: ", filename);
    perror("");
    return NULL;
  }

  size_t n_v = 0, n_f = 0;
  count_vertices_faces(file, &n_v, &n_f);
  if (n_v > MAX_N_V) {
    fputs("The object stores too many vertices", stderr);
    fclose(file);
    return NULL;
  }

  Object *obj = allocate_obj(filename, n_v, n_f);
  if (!obj) {
    fclose(file);
    return NULL;
  }

  bool success = try_parse(file, obj);
  fclose(file);
  if (!success) {
    fputs("Failed to parse object from file", stderr);
    destroy_obj(obj);
    return NULL;
  }

  return obj;
}

Object *allocate_obj(const char *name, size_t n_v, size_t n_f) {
  Object *obj = malloc(sizeof(Object));
  if (!obj || !name) {
    perror("Object Allocation Error:");
    free(obj);
    return NULL;
  }

  obj->n_vertices = n_v;
  obj->n_faces = n_f;

  obj->name = calloc(strlen(name) + 1, sizeof(char));
  if (!obj->name) {
    perror("Name Allocation Error:");
    free(obj);
    return NULL;
  }

  obj->vertices = calloc(n_v * 3, sizeof(float));
  if (!obj->vertices) {
    perror("Vertices Allocation Error:");
    free(obj->name);
    free(obj);
    return NULL;
  }

  obj->faces = calloc(n_f * 3, sizeof(int));
  if (!obj->faces) {
    perror("Edges Allocation Error:");
    free(obj->vertices);
    free(obj->name);
    free(obj);
    return NULL;
  }

  strcpy(obj->name, name);
  return obj;
}

bool try_parse(FILE *fp, Object *obj) {
  char line[BUFF];
  float max_xyz[] = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
  size_t v_ind = 0, f_ind = 0;

  bool success = true;
  while (fgets(line, BUFF, fp)) {
    if (strncmp(line, "v ", 2) == 0) {
      success = parse_vertex(obj, line, max_xyz, &v_ind);
    } else if (strncmp(line, "f ", 2) == 0) {
      success = parse_faces(obj, line, &f_ind);
    }

    if (!success) {
      return false;
    }
  }

  normalize(obj, max_xyz);
  return true;
}

bool parse_vertex(Object *obj, const char *line, float *max, size_t *ind) {
  float x = 0.0, y = 0.0, z = 0.0;
  if (sscanf(line, "v %f %f %f", &x, &y, &z) != 3) {
    return false;
  }

  if (*ind >= obj->n_vertices * 3) {
    return false;
  }

  obj->vertices[(*ind)++] = x;
  obj->vertices[(*ind)++] = y;
  obj->vertices[(*ind)++] = z;

  max[0] = fmaxf(fabsf(x), max[0]);
  max[1] = fmaxf(fabsf(y), max[1]);
  max[2] = fmaxf(fabsf(z), max[2]);

  return true;
}

bool parse_faces(Object *obj, const char *line, size_t *ind) {
  const char *ptr = line + 2;
  int vertices_found = 0;
  int v[3] = {0};

  for (int i = 0; i < 3; i++) {
    while (*ptr == ' ' || *ptr == '\t') {
      ptr++;
    }

    if (*ptr == '\0' || *ptr == '\n') {
      break;
    }

    char *endptr;
    long val = strtol(ptr, &endptr, 10);

    if (endptr == ptr) {
      return false;
    }

    if (val <= 0 || val > (long)obj->n_vertices) {
      return false;
    }

    v[i] = (int)val - 1;
    vertices_found++;

    while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n') {
      ptr++;
    }
  }

  if (vertices_found != 3) {
    return false;
  }

  for (int i = 0; i < 3; i++) {
    obj->faces[(*ind)++] = v[i];
  }

  return true;
}

void normalize(Object *obj, float *mx) {
  const float norm = NORM_RATIO / fmaxf(fmaxf(mx[0], mx[1]), mx[2]);

  for (size_t i = 0; i < obj->n_vertices * 3; i++) {
    obj->vertices[i] *= norm;
  }
}

void destroy_obj(Object *obj) {
  if (obj) {
    free(obj->vertices);
    free(obj->faces);
    free(obj->name);
    free(obj);
  }
}
