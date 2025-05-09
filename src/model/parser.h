#ifndef _3D_PARSER_H
#define _3D_PARSER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 256

const char *get_filename_ext(const char *filename);
bool is_obj_file(const char *filename);

void count_vertices_faces(FILE *fp, size_t *n_v, size_t *n_f);
char *get_filecontent(const char *path);

#endif  // _3D_PARSER_H