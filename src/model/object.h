#ifndef _3D_OBJECT_H
#define _3D_OBJECT_H

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define MAX_N_V 10000000UL
#define NORM_RATIO 1.0f

typedef struct {
  char *name;
  size_t n_vertices;
  size_t n_faces;
  float *vertices;
  int *faces;
} Object;

Object *create_obj(const char *filename);
Object *allocate_obj(const char *name, size_t n_v, size_t n_f);
void destroy_obj(Object *obj);

bool try_parse(FILE *fp, Object *obj);
bool parse_vertex(Object *obj, const char *line, float *max, size_t *ind);
bool parse_faces(Object *obj, const char *line, size_t *ind);

void normalize(Object *obj, float *mx);

#endif  // _3D_OBJECT_H
