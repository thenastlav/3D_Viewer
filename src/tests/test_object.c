#include "test_viewer.h"
#include "../model/object.h"
#include <stdio.h>
#include <stdlib.h>

FILE* create_temp_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w+"); 
    if (file) {
        fputs(content, file);
        rewind(file); 
    }
    return file;
}

START_TEST(test_parse_vertex) {
    Object obj;
    obj.n_vertices = 1;
    obj.vertices = calloc(3, sizeof(float));

    float max_xyz[] = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    size_t index = 0;

    const char* line = "v 1.0 2.0 3.0";
    ck_assert(parse_vertex(&obj, line, max_xyz, &index));
    ck_assert_float_eq(obj.vertices[0], 1.0f);
    ck_assert_float_eq(obj.vertices[1], 2.0f);
    ck_assert_float_eq(obj.vertices[2], 3.0f);
    ck_assert_float_eq(max_xyz[0], 1.0f);
    ck_assert_float_eq(max_xyz[1], 2.0f);
    ck_assert_float_eq(max_xyz[2], 3.0f);

    const char* bad_line = "v 1.0 2.0";
    ck_assert(!parse_vertex(&obj, bad_line, max_xyz, &index));

    free(obj.vertices);
}
END_TEST

START_TEST(test_parse_faces) {
    Object obj;
    obj.n_vertices = 3; 
    obj.n_faces = 1;
    obj.faces = calloc(3, sizeof(int)); 

    size_t index = 0;

    const char* line = "f 1 2 3";
    ck_assert(parse_faces(&obj, line, &index));
    ck_assert_int_eq(obj.faces[0], 0);
    ck_assert_int_eq(obj.faces[1], 1);
    ck_assert_int_eq(obj.faces[2], 2);

    const char* bad_line = "f 1 2";
    ck_assert(!parse_faces(&obj, bad_line, &index));

    const char* bad_line2 = "f 1 2 4"; 
    ck_assert(!parse_faces(&obj, bad_line2, &index));

    free(obj.faces);
}
END_TEST

START_TEST(test_try_parse) {
    Object obj;
    obj.n_vertices = 2;
    obj.n_faces = 1;
    obj.vertices = calloc(6, sizeof(float)); 
    obj.faces = calloc(3, sizeof(int));     

    const char* filename = "test_try_parse.obj";
    const char* content = "v 1.0 2.0 3.0\nv 4.0 5.0 6.0\nf 1 2 1\n";
    FILE* file = create_temp_file(filename, content);
    ck_assert_ptr_nonnull(file);
    fclose(file);

    file = fopen(filename, "r");
    ck_assert_ptr_nonnull(file);
    ck_assert(try_parse(file, &obj));

    ck_assert_int_eq(obj.faces[0], 0);
    ck_assert_int_eq(obj.faces[1], 1);
    ck_assert_int_eq(obj.faces[2], 0);

    fclose(file);
    free(obj.vertices);
    free(obj.faces);
    remove(filename); 


    const char* bad_filename = "bad_try_parse.obj";
    const char* bad_content = "v 1.0 2.0\nf 1 2 1\n";
    file = create_temp_file(bad_filename, bad_content);
    ck_assert_ptr_nonnull(file);
    fclose(file);

    file = fopen(bad_filename, "r");
    ck_assert_ptr_nonnull(file);
    ck_assert(!try_parse(file, &obj)); 
    fclose(file);
    remove(bad_filename); 
}
END_TEST

START_TEST(test_create_obj_file_error) {
    const char* filename = "nonexistent.obj";
    Object* obj = create_obj(filename);
    ck_assert_ptr_null(obj); 
}
END_TEST


START_TEST(test_create_obj_format) {
    const char* filename = "test_format.txt";
    const char* content = "This is not an OBJ file.";
    FILE* file = create_temp_file(filename, content);
    ck_assert_ptr_nonnull(file);
    fclose(file);

    Object* obj = create_obj(filename);
    ck_assert_ptr_null(obj); 
    remove(filename); 
}
END_TEST

START_TEST(test_allocate_obj) {
    const char* name = "test_object";
    size_t n_v = 10; 
    size_t n_f = 5; 

    Object* obj = allocate_obj(name, n_v, n_f);
    ck_assert_ptr_nonnull(obj); 
    ck_assert_str_eq(obj->name, name); 
    ck_assert_uint_eq(obj->n_vertices, n_v);
    ck_assert_uint_eq(obj->n_faces, n_f);
    ck_assert_ptr_nonnull(obj->vertices); 
    ck_assert_ptr_nonnull(obj->faces);

    destroy_obj(obj); 
}
END_TEST

START_TEST(test_create_obj_parse_error) {
    const char* filename = "test_parse_error.obj";
    const char* content = "v 1.0 2.0 3.0\nv 4.0 5.0\nf 1 2 1\n";
    FILE* file = create_temp_file(filename, content);
    ck_assert_ptr_nonnull(file);
    fclose(file);

    Object* obj = create_obj(filename);
    ck_assert_ptr_null(obj); 
    remove(filename);
}
END_TEST


START_TEST(test_create_obj_success) {
    const char* filename = "test_success.obj";
    const char* content = "v 1.0 2.0 3.0\nv 4.0 5.0 4.0\nf 1 2 1\n";
    const float max_vertex_coord = 5.0f;

    FILE* file = create_temp_file(filename, content);
    ck_assert_ptr_nonnull(file);
    fclose(file);

    Object* obj = create_obj(filename);
    ck_assert_ptr_nonnull(obj); 

    ck_assert_uint_eq(obj->n_vertices, 2); 
    ck_assert_uint_eq(obj->n_faces, 1); 

    ck_assert_float_eq(obj->vertices[0], 1.0f / max_vertex_coord);
    ck_assert_float_eq(obj->vertices[1], 2.0f / max_vertex_coord);
    ck_assert_float_eq(obj->vertices[2], 3.0f / max_vertex_coord);
    ck_assert_float_eq(obj->vertices[3], 4.0f / max_vertex_coord);
    ck_assert_float_eq(obj->vertices[4], 5.0f / max_vertex_coord);
    ck_assert_float_eq(obj->vertices[5], 4.0f / max_vertex_coord);

    ck_assert_int_eq(obj->faces[0], 0);
    ck_assert_int_eq(obj->faces[1], 1);
    ck_assert_int_eq(obj->faces[2], 0);

    destroy_obj(obj); 
    remove(filename); 
}
END_TEST

Suite *object_suite() {
    Suite *s;
    s = suite_create("\033[33m3d_object_tests\033[0m");

    TCase *tc_core;
    tc_core = tcase_create("object_core");
    tcase_add_test(tc_core, test_try_parse);
    tcase_add_test(tc_core, test_parse_vertex);
    tcase_add_test(tc_core, test_parse_faces);
    tcase_add_test(tc_core, test_allocate_obj); 
    tcase_add_test(tc_core, test_create_obj_file_error);
    tcase_add_test(tc_core, test_create_obj_format);
    tcase_add_test(tc_core, test_create_obj_parse_error);
    tcase_add_test(tc_core, test_create_obj_success);
    suite_add_tcase(s, tc_core);

    return s;
}