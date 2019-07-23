#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#define SAC_NULL_HEADER_REQUIRED
#include "sacio.h"
#include "strip.h"
#include "defs.h"

#define ERROR_NOT_A_SAC_FILE                1317
#define ERROR_OVERWRITE_FLAG_IS_OFF         1303
#define ERROR_WRITING_FILE                  115
#define ERROR_READING_FILE                  114
#define ERROR_OPENING_FILE                  101
#define SAC_OK                              0

#define SAC_HEADER_SIZEOF_NUMBER          (sizeof(float))
#define SAC_DATA_SIZE                     SAC_HEADER_SIZEOF_NUMBER
#define SAC_HEADER_MAJOR_VERSION          6
#define SAC_VERSION_LOCATION              76

/**
 * @param SAC_HEADER_FLOATS
 *    Number of Floating point values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_FLOATS                 70    /* 4 bytes  (real or float)    */
/**
 * @param SAC_HEADER_INTEGERS
 *    Number of Integer values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_INTEGERS               15

/**
 * @param SAC_HEADER_ENUMS
 *    Number of Enumerated values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_ENUMS                  20
/**
 * @param SAC_HEADER_LOGICALS
 *    Number of Logical values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_LOGICALS               5
/**
 * @param SAC_HEADER_STRINGS
 *    Number of strings in the SAC Header.  The number here is in reality
 *      one less as the number below as of header version 6.  The second
 *      value, the event name is twice as long as any other characer string
 *
 * @see SAC_HEADER_STRING_LENGTH
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_STRINGS                24    /* 9 bytes  (character or char)
                                                 *   actually 23 + 1 */
/**
 * @param SAC_HEADER_STRINGS_SIZE_FILE
 *    Size of file character header data block in four byte increments
 *    @size (8) * \p SAC_HEADER_STRINGS
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_STRINGS_SIZE_FILE    ( 2 * SAC_HEADER_STRINGS )

/**
 * @param SAC_HEADER_NUMBERS
 *    Number of numeric values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 * @date 2009 Feb 15
 *
 */
#define SAC_HEADER_NUMBERS                ( SAC_HEADER_FLOATS +   \
              SAC_HEADER_INTEGERS + \
              SAC_HEADER_ENUMS +    \
              SAC_HEADER_LOGICALS )
/**
 * @param SAC_HEADER_STRING_LENGTH_FILE
 *    Size of a character string stored on disk for a SAC header
 *    Strings are stored without the C string termination character
 *
 */
#define SAC_HEADER_STRING_LENGTH_FILE     8
/**
 * @param SAC_HEADER_STRING_SIZE_BYTES_FILE
 *     Size of string header in file measured in bytes
 *     SAC_HEADER_STRINGS * SAC_HEADER_STRINGS_LENGTH_FILE
 */
#define SAC_HEADER_STRINGS_SIZE_BYTES_FILE ( SAC_HEADER_STRINGS * SAC_HEADER_STRING_LENGTH_FILE )

/**
 * @param SAC_HEADER_NUMBERS_SIZE_BYTES_FILE
 *     Size of numbers header in file measured in bytes
 *     SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER
 */
#define SAC_HEADER_NUMBERS_SIZE_BYTES_FILE ( SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER )


void
update_distaz(sac *s) {
    UNUSED(s);
}

int
sac_check_npts(int npts) {
    if (npts <= 0) {
        return ERROR_WRITING_FILE;
    }
    return SAC_OK;
}
size_t
sac_size(sac *s) {
    #define SAC_HEADER_SIZE 632
    return SAC_HEADER_SIZE + (4 * s->h->npts);
}

int
sac_check_lovrok(int lovrok) {
    if (!lovrok) {
        return ERROR_OVERWRITE_FLAG_IS_OFF;
    }
    return SAC_OK;
}
int
sac_comps(sac * s) {
    int n = 0;
    switch (s->h->iftype) {
        case ITIME:
        case IXY:
        case IUNKN:
            n = (s->h->leven) ? 1 : 2;
            break;
        case IXYZ:
            n = 1;
            break;
        case IRLIM:
        case IAMPH:
            n = 2;
            break;
        default:
            fprintf(stderr, "unknown sac data type: %d\n", s->h->iftype);
            n = 2;
            break;
    }
    return n;
}
static float
array_min(float *y, int n) {
    float v = y[0];
    for(int i = 0; i < n; i++) {
        v = fmin(v, y[i]);
    }
    return v;
}
static float
array_max(float *y, int n) {
    float v = y[0];
    for(int i = 0; i < n; i++) {
        v = fmax(v, y[i]);
    }
    return v;
}
static float
array_mean(float *y, int n) {
    double v = 0.0;
    for(int i = 0; i < n; i++) {
        v += y[i];
    }
    return v / n;
}


static void
check_value(float vmin, float vmax) {
    if(! isfinite(vmin) || ! isfinite(vmax)) {
        printf("Non-finite floating point value encountered\n");
        printf("  Min value: %g ", vmin);
        printf("  Max value: %g\n", vmax);
    }
}
void
sac_extrema(sac * s) {
    s->h->depmin = array_min(s->y, s->h->npts);
    s->h->depmax = array_max(s->y, s->h->npts);
    s->h->depmen = array_mean(s->y, s->h->npts);
    check_value(s->h->depmin, s->h->depmax);
}

char *
khdr(sac * s, int k) {
    char *p;
    switch (k) {
        case 1:
            p = s->h->kstnm;
            break;
        case 2:
        case 3:
            p = s->h->kevnm;
            break;
        case 4:
            p = s->h->khole;
            break;
        case 5:
            p = s->h->ko;
            break;
        case 6:
            p = s->h->ka;
            break;
        case 7:
            p = s->h->kt0;
            break;
        case 8:
            p = s->h->kt1;
            break;
        case 9:
            p = s->h->kt2;
            break;
        case 10:
            p = s->h->kt3;
            break;
        case 11:
            p = s->h->kt4;
            break;
        case 12:
            p = s->h->kt5;
            break;
        case 13:
            p = s->h->kt6;
            break;
        case 14:
            p = s->h->kt7;
            break;
        case 15:
            p = s->h->kt8;
            break;
        case 16:
            p = s->h->kt9;
            break;
        case 17:
            p = s->h->kf;
            break;
        case 18:
            p = s->h->kuser0;
            break;
        case 19:
            p = s->h->kuser1;
            break;
        case 20:
            p = s->h->kuser2;
            break;
        case 21:
            p = s->h->kcmpnm;
            break;
        case 22:
            p = s->h->knetwk;
            break;
        case 23:
            p = s->h->kdatrd;
            break;
        case 24:
            p = s->h->kinst;
            break;
        default:
            p = NULL;
            break;
    }
    return p;
}

double
check_precision(float dt, float val) {
    float df = val - nextafterf(val,0.0);
    if(df > dt) {         // Return difference if larger than sampling rate
        return df;
    }
    return 0.0;           // Otherwise return 0.0
}
void
sac_check_time_precision(struct SACheader *h) {
    int i, n;
    double df;
    char *names[] = {"b","e","a","o","t0","t1","t2","t3","t4","t5","t6","t7","t8","t9","f"};
    float values[] = {h->b, h->e, h->a, h->o,
                      h->t0,h->t1,h->t2,h->t3,h->t4, h->t5,h->t6,h->t7,h->t8,h->t9,
                      h->f};
    n = sizeof(values)/sizeof(float);
    for(i = 0; i < n; i++) {
        if(values[i] == SAC_FLOAT_UNDEFINED) {
            continue;
        }
        if((df = check_precision(h->delta, values[i])) != 0) {
            printf("minimum precision > sampling rate: %s = %f\n"
                "       sampling rate (delta):      %f\n"
                "       32-bit minimum precision:   %f",
                names[i], values[i], h->delta, df);
            //outmsg();
            //clrmsg();
        }
    }
}
static void
byteswap_bsd(void *v, size_t n) {
    char t = 0;
    char *p = v;
    for(size_t i = 0; i < n/2; i++) {
        size_t j = n-i-1;
        t = p[i];
        p[i] = p[j];
        p[j] = t;
    }
}

/**
 * Swap the Sac Header
 *
 * @param hdr
 *    Sac Header to Swap, packed
 *
 * @bug Unexpected and compiler packing of the structure
 *    will cause problems swapping the header, each value
 *    should be swapped individually, it is the safest way
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
void
sac_header_swap(void *hdr) {
    int i;
    float *ptr;
    for (i = 0, ptr = hdr; i < SAC_HEADER_NUMBERS; i++, ptr++) {
        byteswap_bsd(ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}
void
sac_data_swap(float *y, int n) {
    int i;
    float *ptr;
    for (i = 0, ptr = y; i < n; i++, ptr++) {
        byteswap_bsd((void *) ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}

void
sac_copy_strings_add_terminator(sac *s, char *src) {
    void *p = src;
    for(int i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_EVENT2) {
            continue;
        }
        char *dst = khdr(s, i-SAC_STA+1);
        size_t n = (i == SAC_EVENT) ? 16 : 8;
        memcpy(dst, p, n);
        dst[n] = 0;
        p += n;
    }
}

void
sac_copy_strings_strip_terminator(sac *s, char *dst) {
    void *p = dst;
    for(int i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_EVENT2) {
            continue;
        }
        size_t n = (i == SAC_EVENT) ? 16 : 8;
        memcpy(p, khdr(s, i-SAC_STA+1), n);
        p += n;
    }
}

/**
 * Write a Sac Header
 *
 * @param nun
 *    Logical file unite to write SAC Header To
 * @param nerr
 *    Error return Flag
 *    - SAC_OK
 *    - Non-Zero on Error
 *
 * @date November 7, 2010
 */
void
sac_header_write(sac *s, int nun, int swap, int *nerr) {
    int n;
    char temp2[256]; // 8*24 = 192; Should be big enough

    if (swap) {
        sac_header_swap((void *) s->h);
    }
    /* Write the numerical values */
    n = write(nun, (void *) s->h, SAC_HEADER_NUMBERS_SIZE_BYTES_FILE);

    if (n != SAC_HEADER_NUMBERS_SIZE_BYTES_FILE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
    if (swap) {
        sac_header_swap((void *) s->h);
    }

    sac_copy_strings_strip_terminator(s, temp2);
    n = write(nun, temp2, SAC_HEADER_STRINGS_SIZE_BYTES_FILE);
    if (n != SAC_HEADER_STRINGS_SIZE_BYTES_FILE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
}
void
sac_data_write1(int nun, float *data, int npts, int swap, int *nerr) {
    int n;
    if (swap) {
        sac_data_swap(data, npts);
    }
    n = write(nun, data, npts * SAC_DATA_SIZE);
    if (n != npts * SAC_DATA_SIZE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
}
void
sac_data_write2(int nun, float *y, float *x, int npts, int swap, int *nerr) {
    sac_data_write1(nun, y, npts, swap, nerr);
    if (*nerr) {
        return;
    }
    sac_data_write1(nun, x, npts, swap, nerr);
}
void
sac_data_write(int nun, float *y, float *x, int comps, int npts, int swap,
               int *nerr) {
    if (comps == 1) {
        sac_data_write1(nun, y, npts, swap, nerr);
    } else if (comps == 2) {
        sac_data_write2(nun, y, x, npts, swap, nerr);
    } else {
        *nerr = ERROR_WRITING_FILE;
    }
    return;
}

void
sac_write_internal(sac *s, char *filename, int write_data, int swap, int *nerr) {
    int nin = 0;
    if(write_data && ((*nerr = sac_check_npts(s->h->npts)) != SAC_OK)) {
        return;
    }
    if((*nerr = sac_check_lovrok(s->h->lovrok)) != SAC_OK) {
        return;
    }
    if(write_data) {
        sac_extrema(s);
    }
    update_distaz(s);

    sac_check_time_precision(s->h);

    if(write_data) {
        nin = creat(filename, 0666);
    } else {
        nin = open(filename, O_RDWR, 0666);
        swap = s->m->swap;
    }
    if(nin < 0) {
        *nerr = ERROR_OPENING_FILE;
        return;
    }
    sac_header_write(s, nin, swap, nerr);
    if(*nerr != SAC_OK) {
        return;
    }
    if(write_data) {
        sac_data_write(nin, s->y, s->x, sac_comps(s), s->h->npts, swap, nerr);
        if(*nerr != SAC_OK) {
            return;
        }
    }
    close(nin);
}

void
sac_write(sac *s, char *filename, int *nerr) {
    sac_write_internal(s, filename, SAC_WRITE_HEADER_AND_DATA, s->m->swap, nerr);
}
void
sac_alloc(sac * s) {
    if (!s) {
        return;
    }
    FREE(s->y);
    FREE(s->x);
    s->y = (float *) malloc(sizeof(float) * s->h->npts);
    memset(s->y, 0, s->h->npts * sizeof(float));
    if (sac_comps(s) == 2) {
        s->x = (float *) malloc(sizeof(float) * s->h->npts);
        memset(s->x, 0, s->h->npts * sizeof(float));
    }
}

int
sac_data_read_new(sac *s, FILE *fp) {
    float *p;
    int i;
    size_t n;
    n = s->h->npts;
    for(i = 0; i < sac_comps(s); i++) {
        p = (i == 0) ? s->y : s->x ;
        if(fread(p, sizeof(float), n, fp) != n) {
            return ERROR_READING_FILE;
        }
        if(s->m->swap) {
            sac_data_swap(p, s->h->npts);
        }
    }
    return 0;
}

/**
 * Check the Sac Header Version
 *
 * @param hdr
 *   Sac Header to Check
 * @param nerr
 *   Error return Flag
 *   - SAC_OK
 *   - SAC_ERROR_NOT_A_SAC_FILE
 *
 * @return
 *    - TRUE if file needs to be swapped
 *    - FALSE if file does not need to be swapped
 *    - -1 if there was an error checking the header
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
int
sac_check_header_version(float *hdr, int *nerr) {
    int lswap;
    int *ver;

    lswap = FALSE;
    *nerr = SAC_OK;
    /* determine if the data needs to be swapped. */
    ver = (int *) (hdr + SAC_VERSION_LOCATION);
    if (*ver < 1 || *ver > SAC_HEADER_MAJOR_VERSION) {
        byteswap_bsd((void *) ver, SAC_HEADER_SIZEOF_NUMBER);

        if (*ver < 1 || *ver > SAC_HEADER_MAJOR_VERSION) {
            *nerr = ERROR_NOT_A_SAC_FILE;
            printf("not in sac format, nor byteswapped sac format.");
            return -1;
        } else {
            /* swap back, so it can be */
            byteswap_bsd((void *) ver, SAC_HEADER_SIZEOF_NUMBER);
            lswap = TRUE;
        }
    }
    return lswap;
}

int
sac_header_read_new(sac *s, FILE *fp) {
    int nerr;
    size_t n;
    char str[SAC_HEADER_STRINGS * 8];
    //fprintf(stderr, "sac hdr: %p\n", s->h);
    n = SAC_HEADER_NUMBERS;
    if(fread((char *) s->h, sizeof(float), n, fp) != n) {
        return ERROR_READING_FILE;
    }
    s->m->swap = sac_check_header_version((float *) s->h, &nerr);
    if(nerr) {
        return ERROR_READING_FILE;
    }
    if(s->m->swap) {
        sac_header_swap((float *) s->h);
    }

    if(fread(str, 8, SAC_HEADER_STRINGS, fp) != SAC_HEADER_STRINGS) {
        return ERROR_READING_FILE;
    }
    sac_copy_strings_add_terminator(s, str);

    sac_check_time_precision(s->h);

    return 0;
}


sac *
sac_read_internal(char *filename, int read_data, int *nerr) {
    FILE *fp;
    sac *s;

    *nerr = 0;
    s = NULL;

    if(!filename) {
        return NULL;
    }

    if(!(fp = fopen(filename, "rb"))) {
        *nerr = 101;
        return NULL;
    }
    s = sac_new();

    s->m->filename = strdup(filename);
    *nerr = sac_header_read_new(s, fp);
    if(*nerr) {
        goto ERROR;
    }
    if(read_data) {
        //fprintf(stderr, "alloc: %d\n", s->h->npts);
        sac_alloc(s);
        s->m->nstart = 1;
        s->m->nstop  = s->h->npts;
        s->m->ntotal = s->h->npts;
        s->m->nfillb = 0;
        s->m->nfille = 0;
        if((*nerr = sac_data_read_new(s, fp))) {
            goto ERROR;
        }
    }
    sac_be(s);
    update_distaz(s);
    if(read_data) {
        sac_extrema(s);
    }

    fclose(fp);
    return s;

 ERROR:
    if(s) {
        sac_free(s);
        s = NULL;
    }
    fclose(fp);
    return NULL;
}

sac *
sac_read_header(char *filename, int *nerr) {
    return sac_read_internal(filename, 0, nerr);
}

sac *
sac_read(char *filename, int *nerr) {
    return sac_read_internal(filename, 1, nerr);
}

void
sac_hdr_init(sac_hdr *sh) {
    if (sh) {
        memcpy(sh, &NullSacHeader, sizeof(NullSacHeader));
        sh->nvhdr = SAC_HEADER_MAJOR_VERSION;

        sh->leven = TRUE;
        sh->lpspol = FALSE;
        sh->lovrok = TRUE;
        sh->lcalda = TRUE;

        sh->iftype = ITIME;
    }
}
void
sac_free(sac * s) {
    if (s) {
        FREE(s->h);
        FREE(s->x);
        FREE(s->y);
        if (s->m) {
            FREE(s->m->filename);
        }
        FREE(s->m);
        FREE(s->sddhdr);
        FREE(s);
    }
}

sacmeta *
sac_meta_new() {
    sacmeta *m;
    m = (sacmeta *) malloc(sizeof(sacmeta));
    if (m) {
        m->swap = FALSE;
        m->filename = NULL;
        m->data_read = TRUE;
        m->nstop = 0;
        m->nstart = 0;
        m->nfillb = 0;
        m->nfille = 0;
        m->ntotal = 0;
    }
    return m;
}

float
calc_e_even(sac *s) {
    switch (s->h->iftype) {
    case ITIME:
    case IXY:
    case IUNKN:
        return s->h->b + s->h->delta * (float)(s->h->npts - 1);
        break;
    case IRLIM:
    case IAMPH: {
        int nfreq = 0;
        if(s->h->npts % 2 == 0) {
            nfreq = s->h->npts / 2;
        } else {
            nfreq = (s->h->npts-1) / 2;
        }
        return s->h->b + (float) nfreq * s->h->delta;
    }
        break;
    case IXYZ:
        break;
    }
    return SAC_FLOAT_UNDEFINED;
}


int
sac_set_string(sac *s, int hdr, char *v) {
    char *k = NULL;
    if(hdr < SAC_STA || hdr > SAC_INST) {
        return 0;
    }
    if(!(k = khdr(s, hdr-SAC_STA+1))) {
        return 0;
    }
    strlcpy(k, v, (hdr == SAC_EVENT) ? 17 : 9);
    return 1;
}
int
sac_get_string(sac *s, int hdr, char *v, size_t n) {
    char *k = NULL;
    if(hdr < SAC_STA || hdr > SAC_INST) {
        return 0;
    }
    if(!(k = khdr(s, hdr-SAC_STA+1))) {
        return 0;
    }
    strlcpy(v, k, n);
    return 1;
}

int
sac_set_float(sac *s, int hdr, float v) {
    if(hdr < SAC_DELTA || hdr > SAC_UN70) {
        return 0;
    }
    float *fp = (float *) (&(s->h->delta));
    fp[hdr - SAC_DELTA] = v;
    return 1;
}
int
sac_get_float(sac *s, int hdr, float *v) {
    if(hdr < SAC_DELTA || hdr > SAC_UN70) {
        return 0;
    }
    float *fp = (float *) (&(s->h->delta));
    *v = fp[hdr - SAC_DELTA];
    return 1;
}
int
sac_set_int(sac *s, int hdr, int v) {
    if(hdr < SAC_YEAR || hdr > SAC_UN105) {
        return 0;
    }
    int *ip = (int *) (&(s->h->nzyear));
    ip[hdr - SAC_YEAR] = v;
    return 1;
}
int
sac_get_int(sac *s, int hdr, int *v) {
    if(hdr < SAC_YEAR || hdr > SAC_UN105) {
        return 0;
    }
    int *ip = (int *) (&(s->h->nzyear));
    *v = ip[hdr - SAC_YEAR];
    return 1;
}

#define is_float(x) ( x >= SAC_DELTA && h <= SAC_UN70 )
#define is_int(x)   ( x >= SAC_YEAR && h <= SAC_UN105 )
#define is_str(x)   ( x >= SAC_STA && h <= SAC_INST )

int
sac_hdr_defined(sac *s, ...) {
    int h = 0;
    va_list ap;
    if(!s) {
        return 0;
    }
    va_start(ap, s);
    while((h = va_arg(ap, int)) > 0) {
        if( is_float(h) ) {
            float v = 0.0;
            sac_get_float(s, h, &v);
            if(v == SAC_FLOAT_UNDEFINED) {
                return 0;
            }
        }
        else if( is_int(h) ) {
            int v = 0;
            sac_get_int(s, h, &v);
            if(v == SAC_INT_UNDEFINED) {
                return 0;
            }
        }
        else if( is_str(h) ) {
            char v[20] = {0};
            sac_get_string(s, h, v, sizeof(v));
            if(h == SAC_EVENT) {
                if(strcmp(v, SAC_CHAR_UNDEFINED_2) == 0) {
                    return 0;
                }
            } else {
                if(strcmp(v, SAC_CHAR_UNDEFINED) == 0) {
                    return 0;
                }
            }
        }
    }
    va_end(ap);
    return 1;
}

/**
 * Get the reference time from a sac file
 *
 * @memberof sac
 *
 * @param s   sac file
 * @param t   timespec64 output value
 *
 * @return 1 on success, 0 in failure
 *
 */
int
sac_get_time_ref(sac *s, timespec64 *t) {
    if( sac_hdr_defined(s,
                        SAC_YEAR, SAC_DAY, SAC_HOUR, SAC_MIN, SAC_SEC, SAC_MSEC, -1)) {
         *t = timespec64_from_yjhmsf(s->h->nzyear, s->h->nzjday,
                                    s->h->nzhour, s->h->nzmin, s->h->nzsec,
                                    s->h->nzmsec * 1000000);
        return 1;
    }
    return 0;
}

/**
 * Get time value from sac file
 *
 * @memberof sac
 *
 * @param s    sac file
 * @param hdr  header index value
 * @param t    timespec64 output value
 *
 * @reutrn 1 in success, 0 on failure
 *
 */
int
sac_get_time(sac *s, int hdr, timespec64 *t) {
    float v = 0.0;
    double fract = 0.0, ipart = 0.0;
    if(!sac_get_time_ref(s, t)) {
        return 0;
    }
    if(!sac_get_float(s, hdr, &v)) {
        return 0;
    }
    fract = modf(v, &ipart);
    t->tv_sec +=  (int64_t) ipart;
    t->tv_nsec += (int) (fract * 1e9);
    return 1;
}



/**
 * Set the reference time for a sac file, assumed to be the origin time
 *
 * @memberof sac
 *
 * @param s   sac file
 * @param t   origin time
 *
 * @return 1 on success, 0 on failure
 *
 * @note Reference time is set to the time provided, the time reference
 *   is set to ORIGIN, and all other times are references relative
 *
 * Effective sac commands are:
 *
 *    SAC> ch o gmt 1994 160 00 33 16 230
 *    SAC> ch iztype IO
 *    SAC> ch allt (-1.0 * &1,o)
 *
 */
int
sac_set_time(sac *s, timespec64 t) {
    struct TM tm = {0};
    timespec64 t0;
    float dt = 0.0;
    if(sac_get_time_ref(s, &t0)) {
        int64_t dtf = t.tv_sec - t0.tv_sec;
        int64_t dtp = t.tv_nsec - t0.tv_nsec;
        dt = (float) dtf + (float) dtp / 1e9;
    }
    gmtime64_r(&t.tv_sec, &tm);
    s->h->iztype = IO;
    s->h->nzyear = tm.tm_year + 1900;
    s->h->nzjday = tm.tm_yday + 1;;
    s->h->nzhour = tm.tm_hour;
    s->h->nzmin  = tm.tm_min;
    s->h->nzsec  = tm.tm_sec;
    s->h->nzmsec = t.tv_nsec / 1000000;
    s->h->o = dt;
    for(int j = SAC_B; j <= SAC_F; j++) {
        float v = 0.0;
        sac_get_float(s, j, &v);
        if(v != SAC_FLOAT_UNDEFINED) {
            sac_set_float(s, j, v - dt);
        }
    }
    return 1;
}

void
sac_be(sac *s) {
    if(s->h->leven) {
        s->h->e = calc_e_even(s);
    } else {
        if(s->x) {
            s->h->b = array_min(s->x, s->h->npts);
            s->h->e = array_max(s->x, s->h->npts);
        }
    }
}
size_t
sac_strlcat(char *dst, char *src, size_t n) {
#define NT 20
    char tmp[NT] = {0};
    if(strcmp(src, SAC_CHAR_UNDEFINED) != 0) {
        strlcpy(tmp, src, NT);
        rstrip(tmp);
        return strlcat(dst, tmp, n);
    }
    return strlen(dst);
}

size_t
sac_timelcat(char *dst, sac *s, int hdr, size_t n) {
    char tmp[64] = {0};
    timespec64 t = {0,0};
    if(sac_get_time(s, hdr, &t)) {
        strftime64t(tmp, sizeof(tmp), "%FT%T.%3f", &t);
        return strlcat(dst, tmp, n);
    }
    return strlen(dst);
}
size_t
sac_floatlcat(char *dst, sac *s, int hdr, size_t n) {
    float v = 0.0;
    char tmp[64] = {0};
    sac_get_float(s, hdr, &v);
    if(v != SAC_FLOAT_UNDEFINED) {
        snprintf(tmp, sizeof(dst), "%g", v);
    }
    return strlcat(dst, tmp, n);
}

sac_hdr *
sac_hdr_new() {
    sac_hdr *sh;
    sh = (sac_hdr *) malloc(sizeof(sac_hdr));
    if (sh) {
        sac_hdr_init(sh);
    }
    return sh;
}

size_t
sac_fmt(char *dst, size_t n, const char *fmt, sac *s) {
    size_t i = 0, j = 0;
    unsigned char c = 0;
    if(!dst || !fmt || n == 0) {
        return -1;
    }
    memset(dst, 0, n);
    while((c = *fmt) != 0) {
        if(i < 0) {
            return -1;
        }
        // Reglar Character
        if((c = *fmt++) != '%') {
            dst[i++] = c;
            continue;
        }
        // Format Character
        switch(c = *fmt++) {
        case 'E': i = sac_strlcat(dst, s->h->kevnm, n);  break;
        case 'I': i = sac_strlcat(dst, s->h->kinst, n);  break;
        case 'N': i = sac_strlcat(dst, s->h->knetwk, n); break;
        case 'S': i = sac_strlcat(dst, s->h->kstnm, n);  break;
        case 'C': i = sac_strlcat(dst, s->h->kcmpnm, n); break;
        case 'H': i = sac_strlcat(dst, s->h->khole, n);  break;
        case 'L': {
            if((j = sac_strlcat(dst, s->h->khole, n)) == i) {
                // No chracters copied, either empty "" or -12345
                j = sac_strlcat(dst, "--", n);
            }
            i = j;
            break;
        }
        case 't': {
            if(*fmt == 0) {
                printf("Unexpected end of format, expected time specifier\n");
                return -1;
            }
            switch(c = *fmt++) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                i = sac_floatlcat(dst, s, SAC_T0 + (c - '0'), n);
                break;
            case 'b': i = sac_floatlcat(dst, s, SAC_B, n); break;
            case 'e': i = sac_floatlcat(dst, s, SAC_E, n); break;
            case 'o': i = sac_floatlcat(dst, s, SAC_O, n); break;
            case 'a': i = sac_floatlcat(dst, s, SAC_A, n); break;
            default:
                printf("Unknown conversion character: %c\n", c);
                return -1;
            }
        }
            break;
        case '{': {
            char key[16] = {0};
            char *p = NULL;
            if(!(p = strchr(fmt, '}'))) {
                printf("Expected '}', not found\n");
                return -1;
            }
            memcpy(key, fmt, p-fmt);
            key[p-fmt] = 0;
            fmt = p;
            fmt++;
        }
            break;
        case 'Z':
            i = sac_fmt(dst+i, n-i, "%N.%S.%H.%C", s);
            break;
        case 'R':
            i = sac_fmt(dst+i, n-i, "%N %S %L %C %TB %TE", s);
            break;
        case 'T':
            if(*fmt == 0) {
                printf("Unexpected end of format, exptected time specifier\n");
                return -1;
            }
            switch(c = *fmt++) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                i = sac_timelcat(dst, s, SAC_T0 + (c - '0'), n);
                break;
            case 'B': i = sac_timelcat(dst, s, SAC_B, n); break;
            case 'E': i = sac_timelcat(dst, s, SAC_E, n); break;
            case 'O': i = sac_timelcat(dst, s, SAC_O, n); break;
            case 'A': i = sac_timelcat(dst, s, SAC_A, n); break;
            default:
                printf("Unknown conversion character: %c\n", c);
                return -1;
            }
            break;
        default:
            printf("Unknown conversion character: %c\n", c);
            return -1;
        }

    }
    return i;
}


sac *
sac_new() {
    sac *s;
    s = (sac *) malloc(sizeof(sac));
    if (s) {
        s->h = sac_hdr_new();
        if (!s->h) {
            goto ERROR;
        }
        s->m = sac_meta_new();
        if (!s->m) {
            goto ERROR;
        }
        s->n = 1;
        s->y = NULL;
        s->x = NULL;
        s->sddhdr = NULL;
    }

    return s;
  ERROR:
    sac_free(s);
    return NULL;
}


#ifdef __TESTING__

int
main() {
    int nerr = 0;
    char dst[128] = {0};
    sac *s = sac_new();
    timespec64 t = {0,0};
    timespec64 t2 = {0,0};
    s->h->npts = 10;
    s->h->delta = 1.0;
    s->h->b = 0.0;
    sac_alloc(s);
    for(int i = 0; i < 10; i++) {
        s->y[i] = i;
    }
    timespec64_parse("1976/01/27T03:12:00", &t);
    printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    sac_set_time(s, t);
    sac_be(s);
    sac_get_time(s, SAC_B, &t2);
    printf("%lld %lld\n", t2.tv_sec, t2.tv_nsec);
    sac_write(s, "bsd.sac", &nerr);
    printf("err: %d\n", nerr);
    sac_fmt(dst, sizeof(dst), "%TO", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TB", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TE", s);
    printf("%s\n", dst);

    s = sac_read("XE.DOOR..BHZ.M.1994.160.003321.sac", &nerr);
    printf("file: %s\n", s->m->filename);
    sac_fmt(dst, sizeof(dst), "%TO", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TB", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TE", s);
    printf("%s\n", dst);


    return 0;
}

#endif
