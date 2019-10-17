
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#define assert_eq(a,b) assert((a) == (b))
#define assert_ne(a,b) assert((a) != (b))

#include "sacio.h"

void test_000000() {
    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert_ne(s, NULL);
    assert_eq(nerr, 0);
}
void test_000001() {
    int nerr = 0;
    sac *s = sac_read("non-existant-file", &nerr);
    assert_eq(s, NULL);
    assert_eq(nerr, 108);
}
void test_000002() {
    int nerr = 0;
    sac *s = sac_read("sacio.c", &nerr);
    assert_eq(s, NULL);
    assert_eq(nerr, 1317);
}
void test_000003() {
    int nerr = 0;
    sac *s = sac_read_header("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
    assert_eq(s->x, NULL);
    assert_eq(s->y, NULL);
    assert_ne(s->h, NULL);
}
void test_000004() {
    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
    sac_write(s, "t/test_io_small.sac.tmp", &nerr);
    assert_eq(nerr, 0);
}
void test_000005() {
    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
    sac_write_header(s, "t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
}
void test_000006() {
    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
    sac_write_header(s, "non-existant-file", &nerr );
    assert_eq(nerr, 108);
}
void test_000007() {
    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
    sac_write_header(s, "sacio.c", &nerr);
    assert_eq(nerr, 1317);
}
void test_000008() {
    assert_eq(sac_is_timeval(SAC_B), 1);
    assert_eq(sac_is_timeval(SAC_DEPMAX), 0);
}
void test_000009() {
    int nerr = 0;
    double t = 0.0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    sac_set_pick(s, 3, 13.50);
    sac_get_float(s, SAC_T3, &t);
    assert_eq(t, 13.50);
}
void test_000010() {
    int nerr = 0;
    double t = 0.0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    sac_set_pick(s, 3, 13.50);
    sac_get_pick(s, 3, &t);
    assert_eq(t, 13.50);
}
void test_000011() {
    int nerr = 0;
    double t = 0.0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    sac_set_float(s, SAC_DELTA, 1.0/3.0);
    
    // Get delta, 32bit value, from the v6 header
    sac_get_float(s, SAC_DELTA, &t);
    assert_ne(t, 1.0/3.0);
    
    // Switch to the v7 header
    sac_set_v7(s);
    
    // Get delta, 64bit value, from the v7 footer
    sac_get_float(s, SAC_DELTA, &t);
    assert_eq(t, 1.0/3.0);
}
void test_000012() {
    int nerr = 0;
    double t = 0.0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    sac_set_float(s, SAC_DELTA, 1.0/3.0);
    
    // Switch to the v7 header
    sac_set_v7(s);
    
    // Get delta, 64bit value, from the v7 footer
    sac_get_float(s, SAC_DELTA, &t);
    assert_eq(t, 1.0/3.0);
    
    // Switch to the v6 header
    sac_set_v6(s);
    
    // Get delta, 32bit value, from the v6 header
    sac_get_float(s, SAC_DELTA, &t);
    assert_ne(t, 1.0/3.0);
    
}
void test_000013() {
    int v = 0;
    sac *s = sac_new();
    assert_ne(s, NULL);
    assert_eq(s->x, NULL);
    assert_eq(s->y, NULL);
    
    sac_get_int(s, SAC_EVEN, &v);
    assert_eq(v, TRUE);
    
    sac_get_int(s, SAC_POLARITY, &v);
    assert_eq(v, FALSE);
    
    sac_get_int(s, SAC_OVERWRITE, &v);
    assert_eq(v, TRUE);
    
    sac_get_int(s, SAC_CALC_DIST_AZ, &v);
    assert_eq(v, TRUE);
    
    sac_get_int(s, SAC_FILE_TYPE, &v);
    assert_eq(v, ITIME);
}
void test_000014() {
    sac *s = sac_new();
    sac_set_int(s, SAC_NPTS, 100);
    sac_alloc(s);
    assert_ne(s->y, NULL);
    sac_free(s);
    
}
void test_000015() {
    sac *s = sac_new();
    sac_set_int(s, SAC_NPTS, 100);
    sac_alloc(s);
    assert_ne(s->y, NULL);
    assert_eq(s->x, NULL);
    
    // Set the file type to Amplitude/Phase (2 components)
    sac_set_int(s, SAC_FILE_TYPE, IAMPH);
    sac_alloc(s);
    assert_ne(s->y, NULL);
    assert_ne(s->x, NULL);
}
void test_000016() {
    int nerr = 0;
    sac *s = sac_read("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 0);
    
    // Write the sac file into alpha-numeric format
    sac_write_alpha(s, "t/test_io_small_alpha.sac.tmp", &nerr);
    assert_eq(nerr, 0);
}
void test_000017() {
    int nerr = 0;
    sac *s = sac_read_alpha("t/test_spec_alpha.sac", &nerr);
    assert_eq(nerr, 0);
    assert_ne(s, NULL);
}
void test_000018() {
    int nerr = 0;
    sac *s = sac_read_alpha("non-existant-file", &nerr);
    assert_eq(nerr, 101);
    assert_eq(s, NULL);
}
void test_000019() {
    int nerr = 0;
    sac *s = sac_read_alpha("t/test_io_small.sac", &nerr);
    assert_eq(nerr, 1319);
    assert_eq(s, NULL);
}
int main() {
    test_000000();
    test_000001();
    test_000002();
    test_000003();
    test_000004();
    test_000005();
    test_000006();
    test_000007();
    test_000008();
    test_000009();
    test_000010();
    test_000011();
    test_000012();
    test_000013();
    test_000014();
    test_000015();
    test_000016();
    test_000017();
    test_000018();
    test_000019();
    return 0;
}
