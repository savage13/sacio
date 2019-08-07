/**
 * @file
 * @brief sac I/O and manipulation
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <float.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/** @cond NO_DOCS */
#define SAC_NULL_HEADER_REQUIRED /**< @private Define a fully NULL sac header */
/** @endcond */
#include <sacio.h>

#include "time64.h"
#include "strip.h"
#include "defs.h"

#include "geodesic.h"

#define ERROR_NOT_A_SAC_FILE                1317 /**< @brief Not a sac file */
#define ERROR_OVERWRITE_FLAG_IS_OFF         1303 /**< @brief Overwrite flag, lovrok is set to 0 */
#define ERROR_WRITING_FILE                  115 /**< @brief Error writing sac file */
#define ERROR_READING_FILE                  114 /**< @brief Error reading sac file */
#define ERROR_FILE_DOES_NOT_EXIST           108
#define ERROR_OPENING_FILE                  101 /**< @brief Error opening sac file */
#define SAC_OK                              0 /**< @brief Success, everything is ok */

#define SAC_HEADER_SIZEOF_NUMBER          (sizeof(float))  /**< @brief Size of header values in bytes */
#define SAC_DATA_SIZE                     SAC_HEADER_SIZEOF_NUMBER /**< @brief Size of data values in bytes */
#define SAC_HEADER_MAJOR_VERSION          6 /**< @brief Current Major sac version number */
#define SAC_VERSION_LOCATION              76 /**< @brief Offset in 4-byte words of the header version */

/**
 * @defgroup sac sac
 * @brief sac file I/O and manipulation
 *
 */


/** \cond NO_DOCS */
sacmeta * sac_meta_new();
sac * sac_read_internal(char *filename, int read_data, int *nerr);
double calc_e_even(sac *s);
void sac_write_internal(sac *s, char *filename, int write_data, int swap, int *nerr);
static float array_max(float *y, int n);
static float array_min(float *y, int n);
static float array_mean(float *y, int n);
sac_hdr * sac_hdr_new();
int sac_get_time_ref(sac *s, timespec64 *t);
/** \endcond */


/**
 * @param SAC_HEADER_FLOATS
 *    Number of Floating point values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_FLOATS                 70    /* 4 bytes  (real or float)    */
/**
 * @param SAC_HEADER_INTEGERS
 *    Number of Integer values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_INTEGERS               15

/**
 * @param SAC_HEADER_ENUMS
 *    Number of Enumerated values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_ENUMS                  20
/**
 * @param SAC_HEADER_LOGICALS
 *    Number of Logical values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_LOGICALS               5
/**
 * @param SAC_HEADER_STRINGS
 *    Number of strings in the SAC Header.  The number here is in reality
 *      one less as the number below as of header version 6.  The second
 *      value, the event name is twice as long as any other characer string
 *
 */
#define SAC_HEADER_STRINGS                24    /* 9 bytes (characters) actually 23 + 1 */

/**
 * @param SAC_HEADER_NUMBERS
 *    Number of numeric values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_NUMBERS                ( SAC_HEADER_FLOATS + SAC_HEADER_INTEGERS + SAC_HEADER_ENUMS + SAC_HEADER_LOGICALS )
/** @cond NO_DOCS */
/**
 * @param SAC_HEADER_STRINGS_SIZE_FILE
 *    Size of file character header data block in four byte increments
 * @note 8 * \p SAC_HEADER_STRINGS
 */
#define SAC_HEADER_STRINGS_SIZE_FILE    ( 2 * SAC_HEADER_STRINGS )

/**
 * @param SAC_HEADER_STRING_LENGTH_FILE
 *    Size of a character string stored on disk for a SAC header
 *    Strings are stored without the C string termination character
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
/** \endcond */

/**
 * @brief      Read a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac file, data and header
 *
 * @param      filename   file to read data and header from
 * @param      nerr       status code, 0 on success, non-zero on header
 *
 * @return     sac file structure, NULL on failure
 */
sac *
sac_read(char *filename, int *nerr) {
    return sac_read_internal(filename, 1, nerr);
}


/**
 * @brief      Read a sac file header
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac file header
 *
 * @param      filename    file to read sac header from
 * @param      nerr        status code, 0 on success, non-zero on failure
 *
 * @return     sac file structure, NULL on failure
 */
sac *
sac_read_header(char *filename, int *nerr) {
    return sac_read_internal(filename, 0, nerr);
}



/**
 * @brief      write a sac file to disk
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    write a sac file to dist, header and data
 *
 * @param      s         sac file to write to disk
 * @param      filename  file to write to
 * @param      nerr      status code, 0 on success, non-zero on error
 *
 */
void
sac_write(sac *s, char *filename, int *nerr) {
    sac_write_internal(s, filename, SAC_WRITE_HEADER_AND_DATA, s->m->swap, nerr);
}



sac_f64 *
sac_f64_new() {
    sac_f64 *z;
    z = (sac_f64 *) malloc(sizeof(sac_f64));
    if(z) {
#define X(name,key)  z->key = SAC_FLOAT_UNDEFINED;
  SAC_F64
#undef X
    }
    return z;
}
int
sac_set_f32(sac *s, int n, float value) {
    switch(n) {
#define X(name,key)  case SAC_##name: s->h->key = value; break;
  SAC_F32
#undef X
    default:
        fprintf(stderr, "Error in sac_set_f32(): Unknown type: %d\n", n);
        return 0;
    }
    return 1;
}

int
sac_set_f64(sac *s, int n, double value) {
    switch(n) {
#define X(name,key)  case SAC_##name: s->z->key = value; break;
        SAC_F64
#undef X
    default:
        return sac_set_f32(s, n, (float) value);
        //fprintf(stderr, "Error in sac_set_f64(): Unknown type: %d\n", n);
        break;
    }
    return 1;
}

int
sac_get_f32(sac *s, int n, float *v) {
    switch(n) {
#define X(name,key) case SAC_##name: *v = s->h->key; break;
        SAC_F32
#undef X
    default:
        fprintf(stderr, "Error in sac_get_f32(): Unknown type: %d\n", n);
        return 0;
    }
    return 1;
}

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

int
sac_get_f64(sac *s, int n, double *v) {
    float f = SAC_FLOAT_UNDEFINED;
    switch(n) {
#define X(name,key)  case SAC_##name: *v = s->z->key; fprintf(stderr, "%d %s %s %e\n", n, EXPAND_AND_QUOTE(name), EXPAND_AND_QUOTE(key), s->z->key ); break;
        SAC_F64
#undef X
    default:
        if(!sac_get_f32(s, n, &f)) {
            return 0;
        }
        *v = f;
        //fprintf(stderr, "Error in sac_get_f64(): Unknown type: %d\n", n);
        break;
    }
    return 1;
}

void
sac_copy_f64_to_f32(sac *s) {
#define X(name,key) s->h->key = s->z->key;
    SAC_F64
#undef X
}
void
sac_copy_f32_to_f64(sac *s) {
#define X(name,key) s->z->key = s->h->key;
    SAC_F64
#undef X
}

int
sac_set_pick(sac *s, int n, double value) {
    switch(n) {
    case 0: return sac_set_float(s, SAC_T0, value); break;
    case 1: return sac_set_float(s, SAC_T1, value); break;
    case 2: return sac_set_float(s, SAC_T2, value); break;
    case 3: return sac_set_float(s, SAC_T3, value); break;
    case 4: return sac_set_float(s, SAC_T4, value); break;
    case 5: return sac_set_float(s, SAC_T5, value); break;
    case 6: return sac_set_float(s, SAC_T6, value); break;
    case 7: return sac_set_float(s, SAC_T7, value); break;
    case 8: return sac_set_float(s, SAC_T8, value); break;
    case 9: return sac_set_float(s, SAC_T9, value); break;
    default:
        fprintf(stderr, "Error in sac_set_pick(): Unknown pick: %d\n", n);
        return 0;
        break;
    }
    return 1;
}

int
sac_get_pick(sac *s, int n, double *t) {
    switch(n) {
    case 0: return sac_get_float(s, SAC_T0, t); break;
    case 1: return sac_get_float(s, SAC_T1, t); break;
    case 2: return sac_get_float(s, SAC_T2, t); break;
    case 3: return sac_get_float(s, SAC_T3, t); break;
    case 4: return sac_get_float(s, SAC_T4, t); break;
    case 5: return sac_get_float(s, SAC_T5, t); break;
    case 6: return sac_get_float(s, SAC_T6, t); break;
    case 7: return sac_get_float(s, SAC_T7, t); break;
    case 8: return sac_get_float(s, SAC_T8, t); break;
    case 9: return sac_get_float(s, SAC_T9, t); break;
    default:
        fprintf(stderr, "Error in sac_get_pick(): Unknown pick: %d\n", n);
        break;
    }
    return 0;
}

/**
 * @brief      create a new sac file structure
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    create a new sac file structure. Header and meta data are allocated.
 *             Data components are not allocated and should be done with sac_alloc()
 *             after setting \p the type of file and the number of data points
 *
 * @return     newly created sac file structure
 */
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
        s->z = sac_f64_new();
        if(!s->z) {
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


/**
 * @brief      free a sac file structure
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    free a sac file structure.  Free any data components, meta
 *             data, header and other associated components
 *
 * @param      s   sac file struture to free
 *
 */
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

/**
 * @brief      allocate the sac data portion of a sac strucutre
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    allocate the sac data poriton of a sac structure.  The memory
 *             size the data and components are determined by sac_comps() and
 *             the number of points \p npts. Any previous data is freed 
 *             before allocation
 *
 * @param      s   sac file to allocate the data for
 *
 */
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

/**
 * @brief      Set the beginning and end time for a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set the beginning and end time for a sac file
 *
 * @param      s   sac file
 *
 */
void
sac_be(sac *s) {
    if(s->h->leven) {
        sac_set_float(s, SAC_E, calc_e_even(s));
    } else {
        if(s->x) {
            sac_set_float(s, SAC_B, array_min(s->x, s->h->npts));
            sac_set_float(s, SAC_E, array_max(s->x, s->h->npts));
        }
    }
}

#ifdef USE_GEOGRAPHICLIB

/**
 * @brief      Update the dist, az, gcarc, baz header fields
 *
 * @details    Update the dist, az, gcarc, baz header fields of a sac file
 *             using the geographiclib v 1.49.  Header updates are performed
 *             if stlo, stla, evlo, evla are defined. Radius and flattening
 *             used are defined from WGS84:
 *             - radius:     6378137 m
 *             - flattening: 1.0 / 298.257223563
 *             See https://geographiclib.sourceforge.io/
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      s  sac file to update
 *
 */
void
update_distaz(sac *s) {
    double lat1 = 0.0, lon1 = 0.0, lat2 = 0.0, lon2 = 0.0;
    double degrees = 0.0, meters = 0.0, az1 = 0.0, az2 = 0.0;
    double a = 6378137, f = 1/298.257223563; /* WGS84 */
    struct geod_geodesic g;
    if(!s->h->lcalda) {
        return;
    }
    if(!sac_hdr_defined(s, SAC_STLA, SAC_STLO, SAC_EVLA, SAC_EVLO, NULL)) {
        return;
    }
    geod_init(&g, a, f);
    sac_get_float(s, SAC_EVLA, &lat1);
    sac_get_float(s, SAC_EVLO, &lon1);
    sac_get_float(s, SAC_STLA, &lat2);
    sac_get_float(s, SAC_STLO, &lon2);
    degrees = geod_geninverse(&g, lat1, lon1, lat2, lon2,
                              &meters, &az1, &az2,
                              0, 0, 0, 0);
    sac_set_float(s, SAC_GCARC, degrees);
    sac_set_float(s, SAC_DIST, meters/1e3);
    sac_set_float(s, SAC_AZ, az1);
    az2 = az2 - 180.0;
    if(az2 <= 0.0) {
        az2 += 360.0;
    }
    sac_set_float(s, SAC_BAZ, az2);
}
#else
void distaz(double the, double phe, float *ths, float *phs,
            int ns, float *dist, float *az, float *baz, float *xdeg,
            int *nerr);
void
update_distaz(sac * s) {
    float d,a,b,g;
    int ndaerr = 0;
    //DEBUG("\n");
    double sx, sy, ex, ey;
    float fsx, fsy;
    sac_get_float(s, SAC_STLO, &sx);
    sac_get_float(s, SAC_STLA, &sy);
    sac_get_float(s, SAC_EVLO, &ex);
    sac_get_float(s, SAC_EVLA, &ey);
    if (s->h->lcalda && sx != SAC_FLOAT_UNDEFINED &&
        sy != SAC_FLOAT_UNDEFINED && ex != SAC_FLOAT_UNDEFINED
        && ey != SAC_FLOAT_UNDEFINED) {
        //DEBUG("compute\n");
        /* These temporary values are necessary as distaz() will not
         * calculate an output if the input is < 0
         */
        d = a = b = g = 0;
        fsx = sx;
        fsy = sy;
        distaz(ey, ex,
               (float *) &fsy, (float *) &fsx,
               1,
               (float *) &d, (float *) &a, (float *) &b, (float *) &g,
               &ndaerr);
        sac_set_float(s, SAC_DIST, d);
        sac_set_float(s, SAC_GCARC, g);
        sac_set_float(s, SAC_AZ, a);
        sac_set_float(s, SAC_BAZ, b);
        if (sx == ex && sy == ey) {
            sac_set_float(s, SAC_AZ, 0.0);
            sac_set_float(s, SAC_BAZ, 0.0);
        }
        if (ndaerr) {
            sac_set_float(s, SAC_DIST, SAC_FLOAT_UNDEFINED);
            sac_set_float(s, SAC_GCARC, SAC_FLOAT_UNDEFINED);
            sac_set_float(s, SAC_AZ, SAC_FLOAT_UNDEFINED);
            sac_set_float(s, SAC_BAZ, SAC_FLOAT_UNDEFINED);
        }
    }
    return;
}
#endif

void
sac_header_copy(sac * to, sac * from) {
    memmove(to->h, from->h, sizeof(sac_hdr));
    memmove(to->z, from->z, sizeof(sac_f64));
}
void
sac_meta_copy(sac *to, sac *from) {
    to->m->swap      = from->m->swap;
    to->m->filename  = strdup(from->m->filename);
    to->m->data_read = from->m->data_read;
    to->m->nstop     = from->m->nstop;
    to->m->nstart    = from->m->nstart;
    to->m->nfillb    = from->m->nfillb;
    to->m->nfille    = from->m->nfille;
    to->m->ntotal    = from->m->ntotal;
}
void
sac_data_copy(sac *to, sac *from) {
    if(from->y) {
        sac_alloc(to);
        memcpy(to->y, from->y, sizeof(float) * from->h->npts);
        if(sac_comps(to) == 2 && from->x) {
            memcpy(to->x, from->x, sizeof(float) * from->h->npts);
        }
    }
}


/**
 * @brief      Check the number of npts in a sac file
 *
 * @details    Check the number of npts in a sac file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      npts   Number of points
 *
 * @return     - 0 if  npts > 0
 *             - 115 if the npts <= 0
 */
int
sac_check_npts(int npts) {
    if (npts <= 0) {
        return ERROR_WRITING_FILE;
    }
    return SAC_OK;
}

/**
 * @brief Size of header
 */
#define SAC_HEADER_SIZE 632

/**
 * @brief      Get the size of a sac file in bytes
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get the size of a sac file in bytes if stored on disk
 *
 * @param      s    sac file
 *
 * @return     size of file in bytes
 */
size_t
sac_size(sac *s) {
    return SAC_HEADER_SIZE + (4 * s->h->npts * sac_comps(s));
}


/**
 * @brief      Check the overwrite flag
 *
 * @details    Check the overwrite flat
 *
 * @param      lovrok  - overwrite flag (L - Logical, ovr - OVerwRite, ok - OK)
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @return     - 0 if overwrite flag is != 0
 *             - 1303 if overwrite flag is 0
 */
int
sac_check_lovrok(int lovrok) {
    if (!lovrok) {
        return ERROR_OVERWRITE_FLAG_IS_OFF;
    }
    return SAC_OK;
}

/**
 * @brief      Get the number of data components
 *
 * @details    Get the number of data components:
 *             1 - Evenly spaced time (ITIME), general xy (IXY)
 *                 or unknown (IUKN), xyz (IXYZ)
 *             2 - Unevenly spcaed time (ITIME), real-imag (IRLIM),
 *                 amp-phase (IAMPH)
 *
 * @param      s   sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @return     Number of data components
 */
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

/**
 * @brief      Get the minimum value in an array
 *
 * @details    Get the minimum value in an array
 *
 * @param      y   array
 * @param      n   length of \p y
 *
 * @return     min value in array
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 */
static float
array_min(float *y, int n) {
    int i = 0;
    float v = y[0];
    for(i = 0; i < n; i++) {
        v = fmin(v, y[i]);
    }
    return v;
}
/**
 * @brief      Get the maximum value in an array
 *
 * @details    Get the maximum value in an array
 *
 * @param      y   array
 * @param      n   length of \p y
 *
 * @return     max value in array
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 */
static float
array_max(float *y, int n) {
    int i = 0;
    float v = y[0];
    for(i = 0; i < n; i++) {
        v = fmax(v, y[i]);
    }
    return v;
}
/**
 * @brief      Get the mean value in an array
 *
 * @details    Get the mean value in an array
 *
 * @param      y   array
 * @param      n   length of \p y
 *
 * @return     mean value in array
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 */

static float
array_mean(float *y, int n) {
    int i = 0;
    double v = 0.0;
    for(i = 0; i < n; i++) {
        v += y[i];
    }
    return v / n;
}

/**
 * @brief      check if min and max values are valid floating point numbers
 *
 * @details    check if min and max values are valid floating point numbers
 *
 * @param      vmin    miniumum value
 * @param      vmax    maxiumum value
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 */
static void
check_value(float vmin, float vmax) {
    if(! isfinite(vmin) || ! isfinite(vmax) || vmin < -3.40282e38 || vmax > 3.40282e38) {
        printf(" WARNING: Data value outside system storage bounds\n");
        printf(" Maxvalue = %.5g ", vmax);
        printf(" Minvalue = %.5g\n", vmin);
    }
}

/**
 * @brief      compute depmin, depmax, and depmen in sac header
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    compute depmin, depmax and depmen value for the sac header
 *             report if any of the values are not finite numbers, i.e.
 *             inf or nan
 *
 * @param      s   sac file
 *
 * @note       This sets the following sac header values:
 *             - depmin
 *             - depmax
 *             - depmen
 *
 */
void
sac_extrema(sac * s) {
    s->h->depmin = array_min(s->y, s->h->npts);
    s->h->depmax = array_max(s->y, s->h->npts);
    s->h->depmen = array_mean(s->y, s->h->npts);
    check_value(s->h->depmin, s->h->depmax);
}

/**
 * @brief      Get a reference to a character string sac header value
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Return a pointer to the first character of a particular
 *             sac header value.
 *
 * @param      s   sac file
 * @param      k   character string header id. Conversion from header id
 *                 is done by \f$kid = hid - SAC_STA + 1\f$
 *                 - kstnm  = 1
 *                 - kevnm  = 2
 *                 - khole  = 4
 *                 - ko     = 5
 *                 - ka     = 6
 *                 - kt0    = 7
 *                 - kt1    = 8
 *                 - kt2    = 9
 *                 - kt3    = 10
 *                 - kt4    = 11
 *                 - kt5    = 12
 *                 - kt6    = 13
 *                 - kt7    = 14
 *                 - kt8    = 15
 *                 - kt9    = 16
 *                 - kf     = 17
 *                 - kuser0 = 18
 *                 - kuser1 = 19
 *                 - kuser2 = 20
 *                 - kcmpnm = 21
 *                 - knetwk = 22
 *                 - kdatrd = 23
 *                 - kinst  = 24
 *
 * @return     return type
 */
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

/**
 * @brief      check the precision of a value against the time sampling
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    check the precision of a value against the time sampling
 *             Check is made by computing the difference between \p val
 *             and the next floating value and that difference to the
 *             time sampling.  Uses the nextafterf from
 *             https://en.cppreference.com/w/c/numeric/math/nextafter
 *
 * @param      dt   time sampling
 * @param      val  value to check
 *
 * @return     difference between floating point values if larger than
 *             time sampling, 0.0 otherwise
 *
 */
double
check_precision(float dt, float val) {
    float df = val - nextafterf(val,0.0);
    if(df > dt) {         // Return difference if larger than sampling rate
        return df;
    }
    return 0.0;           // Otherwise return 0.0
}

/**
 * @brief      check the time precision of all timing values in the sac header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    check all time related values in the sac header for loss of
 *             precision.  If the delta is undefined of the file type is
 *             \p ITIME, the function returns immediately. This routine only
 *             prints out a message if precision loss is detected.  Header
 *             values checked are b, e, a, o, f, t0 - t9
 *
 * @param      h     pointer to the sac header
 *
 *
 */
void
sac_check_time_precision(sac *s) {
    UNUSED(s);
    /* int i, n; */
    /* double df; */
    /* char *names[] = {"b","e","a","o","t0","t1","t2","t3","t4","t5","t6","t7","t8","t9","f"}; */
    /* float values[] = {s->z->b, h->e, h->a, h->o, */
    /*                   h->t0,h->t1,h->t2,h->t3,h->t4, h->t5,h->t6,h->t7,h->t8,h->t9, */
    /*                   h->f}; */
    /* if(h->delta == SAC_FLOAT_UNDEFINED || h->iftype != ITIME) { */
    /*     return; */
    /* } */
    /* n = sizeof(values)/sizeof(float); */
    /* for(i = 0; i < n; i++) { */
    /*     if(values[i] == SAC_FLOAT_UNDEFINED) { */
    /*         continue; */
    /*     } */
    /*     if((df = check_precision(h->delta, values[i])) != 0) { */
    /*         printf(" WARNING:  minimum precision > sampling rate: %s = %f\n" */
    /*             "       sampling rate (delta):      %f\n" */
    /*             "       32-bit minimum precision:   %f\n", */
    /*             names[i], values[i], h->delta, df); */
    /*         //outmsg(); */
    /*         //clrmsg(); */
    /*     } */
    /* } */
}

/**
 * @brief      byteswap data
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    byteswap data of 2, 4, or 8 bytes in place
 *
 * @param      v    data to be swapped
 * @param      n    size of data
 *
 */
static void
byteswap_bsd(void *v, size_t n) {
    size_t i = 0;
    char t = 0;
    char *p = v;
    for(i = 0; i < n/2; i++) {
        size_t j = n-i-1;
        t = p[i];
        p[i] = p[j];
        p[j] = t;
    }
}

/**
 * @brief Swap the Sac Header
 *
 * @details Swap all floating point and integer values of the sac header
 *
 * @private
 * @memberof sac
 * @ingroup  sac
 *
 * @param hdr    Sac Header to Swap, packed
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 *
 */
void
sac_header_swap(void *hdr) {
    int i;
    float *ptr;
    for (i = 0, ptr = hdr; i < SAC_HEADER_NUMBERS; i++, ptr++) {
        byteswap_bsd(ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}

/**
 * @brief      Swap an array of data in place
 *
 * @details    Swap an array of data in place,
 *             typically the data component of a sac file
 *
 * @private
 * @memberof sac
 * @ingroup  sac
 *
 * @param      y   array to to be swapped in place
 * @param      n   length of \p y
 *
 */
void
sac_data_swap(float *y, int n) {
    int i;
    float *ptr;
    for (i = 0, ptr = y; i < n; i++, ptr++) {
        byteswap_bsd((void *) ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}

/**
 * @brief      Copy strings from a sac file to a sac structure in memory
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy character string from a sac file to a sac structure
 *             in memory, appending C-termination characters at the end
 *             This is the inverse of sac_copy_strings_strip_terminator().
 *             Input \p src data is assumed to be concatenated unterminated
 *             character strings as exists for a sac file saved to disk
 *
 *
 * @param      s     Where to place the character strings with terminators
 * @param      src   Packed Input character strings without terminators
 *
 */
void
sac_copy_strings_add_terminator(sac *s, char *src) {
    int i = 0;
    void *p = src;
    for(i = SAC_STA; i <= SAC_INST; i++) {
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

/**
 * @brief      Copy strings from a sac file to a packed string
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy character strings from a sac structure in memory
 *             to a concatenated character string as would exist in a sac
 *             file on disk.  Terminators are removed and strings are appended
 *             in order as they appear on disk.  This is the inverse operation
 *             of sac_copy_strings_add_terminator().  Order is important.
 *
 * @param      s     sac file to take strings from
 * @param      dst   concatenated character string to place unterminated
 *                   character string into.  
 *
 */
void
sac_copy_strings_strip_terminator(sac *s, char *dst) {
    int i = 0;
    void *p = dst;
    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_EVENT2) {
            continue;
        }
        size_t n = (i == SAC_EVENT) ? 16 : 8;
        memcpy(p, khdr(s, i-SAC_STA+1), n);
        p += n;
    }
}

/**
 * @brief Write a Sac Header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @param nun   Logical file unite to write SAC Header To
 * @param nerr  Statue code, 0 on success, non-zero on failure
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

/**
 * @brief      Write a single data component
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Write a single data component to a file descriptor
 *
 * @param      nun    File descriptor to write to
 * @param      data   Data to write
 * @param      npts   Length of data to write
 * @param      swap   To swap dat before writing
 * @param      nerr   Status code, 0 on success, non-zero on error
 *
 */
void
sac_data_write1(int nun, float *data, int npts, int swap, int *nerr) {
    size_t n;
    if (swap) {
        sac_data_swap(data, npts);
    }
    n = write(nun, data, npts * SAC_DATA_SIZE);
    if (n != (size_t) npts * SAC_DATA_SIZE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
    if(swap) {
        sac_data_swap(data, npts);
    }
}

/**
 * @brief      Write two data components to a file descriptor
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Write two data components to a file descriptor,
 *             calls sac_data_write1()
 *
 * @param      nun    file descriptor to write to
 * @param      y      first data component to write
 * @param      x      second data component to write
 * @param      npts   length of \p y and \p x
 * @param      swap   to swap data before writing
 * @param      nerr   status code, 0 on succes, non-zero on failure
 *
 */
void
sac_data_write2(int nun, float *y, float *x, int npts, int swap, int *nerr) {
    sac_data_write1(nun, y, npts, swap, nerr);
    if (*nerr) {
        return;
    }
    sac_data_write1(nun, x, npts, swap, nerr);
}

/**
 * @brief      Write data components to a file descriptor
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Write 1 or 2 data components to a file descriptor, calls
 *             sac_data_write1() or sac_data_write2()
 *
 * @param      nun    file descriptor to write to
 * @param      y      first data component to write
 * @param      x      second data component to write, maybe NULL
 * @param      comps  number of data components to write
 * @param      npts   length of \p y and \p x
 * @param      swap   to swap data before writing
 * @param      nerr   status code, 0 on success, non-zero on failure
 *
 */
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

/**
 * @brief      internal sac data writing function
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    internal sac data writing function.
 *             - Checks the number of points
 *             - checks if the overwrite flag is "ok"
 *             - updates the extra values (depmin, depmax, depmen)
 *             - updates the distance fields (dist, az, baz, gcarc)
 *             - checks and reports any time precison issues
 *             - create a new file if writing data or update the header of
 *               and existing file
 *             - write the header
 *             - write the data
 *
 * @param      s           sac file to write
 * @param      filename    filename to write to
 * @param      write_data  whether to write data or just the header
 * @param      swap        whether to swap the data and header before writing
 * @param      nerr        status code, 0 on success, non-zero on failure
 *
 */
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

    //sac_check_time_precision(s->h);

    if(write_data) {
        nin = creat(filename, 0666);
    } else {
        nin = open(filename, O_RDWR, 0666);
        swap = s->m->swap;
    }
    if(nin < 0) {
        if(!write_data) {
            *nerr = ERROR_FILE_DOES_NOT_EXIST;
        } else {
            *nerr = ERROR_OPENING_FILE;
        }
        return;
    }
    sac_copy_f64_to_f32(s);
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
/**
 * @brief      read sac data from a file pointer
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    read sac data from a file pointer.  Memory should already
 *             be allocated and data will be swapped if the \p swap flag
 *             is defined in the meta data
 *
 * @param      s    sac data to read data into
 * @param      fp   file pointer to read data from
 *
 */
int
sac_data_read(sac *s, FILE *fp) {
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
 * @brief Check the Sac Header Version
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details This routine checks the header version for a number between
 *          1 and \p SAC_HEADER_MAJOR_VERSION.  It check both the swapped
 *          and non-swapped version to determine the byte-order of the file
 *
 * @param hdr    Sac Header to Check
 * @param nerr   status code, 0 on success, non-zero on failure
 *
 * @return if header needs to be swapped or not or is not a sac file
 *    - 1 if file needs to be swapped
 *    - 0 if file does not need to be swapped
 *    - -1 if there was an error checking the header
 *
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


/**
 * @brief      Read a sac header from a file pointer
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac header from a file pointer
 *
 * @param      s   sac file to place header into
 * @param      fp  file pointer to read header from
 *
 * @return     status code, 0 on success, non-zero on failure
 */
int
sac_header_read(sac *s, FILE *fp) {
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

    //sac_check_time_precision(s->h);

    return 0;
}

/**
 * @brief      Read a sac file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac file. Header values b, e, dist, az, baz, gcarc
 *             updated after read.  Data is read from either byte order
 *             and converted to the system's byte order
 *
 * @param      filename   file to read sac file from
 * @param      read_data  whether to read the header and data or just the header
 * @param      nerr       status code, 0 on success, non-zero on failure
 *
 * @return     sac file structure, NULL on failure
 */
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
        *nerr = ERROR_FILE_DOES_NOT_EXIST;
        return NULL;
    }
    s = sac_new();

    s->m->filename = strdup(filename);
    *nerr = sac_header_read(s, fp);
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
        if((*nerr = sac_data_read(s, fp))) {
            goto ERROR;
        }
    }
    sac_be(s);
    update_distaz(s);
    if(read_data) {
        sac_extrema(s);
    }

    sac_copy_f32_to_f64(s);

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

/**
 * @brief      initialize / fill a sac header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    initialize / fill a sac header with default values
 *             - nvhdr  = \p SAC_HEADER_MAJOR_VERSION
 *             - level  = TRUE
 *             - lpspol = FALSE
 *             - lovrok = TRUE
 *             - lcalda = TRUE
 *             - level  = ITIME
 *             - all other values are set to undefined, -12345
 *
 * @param      sh   pointer sac header
 *
 */
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

/**
 * @brief      Create a new sac meta component
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Create and initialize a new sac meta component
 *
 * @return     sac meta component
 */
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

/**
 * @brief      Calculate the end time for an evenly spaced file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Calculate the end time for an evenly spaced file.
 *             For a time domain file, \f$ e = b + dt * (npts-1) \f$
 *             For a frequency domain file, \f$ e = b + nfreq * df\f$
 *             where nfreq = npts/2 or (npts-1)/2 if npts is even or odd
 *
 * @param      s   sac file
 *
 * @return     end time
 */
double
calc_e_even(sac *s) {
    double b, dt;
    sac_get_float(s, SAC_B, &b);
    sac_get_float(s, SAC_DELTA, &dt);
    switch (s->h->iftype) {
    case ITIME:
    case IXY:
    case IUNKN:
        return b + dt * (float)(s->h->npts - 1);
        break;
    case IRLIM:
    case IAMPH: {
        int nfreq = 0;
        if(s->h->npts % 2 == 0) {
            nfreq = s->h->npts / 2;
        } else {
            nfreq = (s->h->npts-1) / 2;
        }
        return b + (float)nfreq * dt;
    }
        break;
    case IXYZ:
        break;
    }
    return SAC_FLOAT_UNDEFINED;
}

/**
 * @brief      Set a character string in a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a character stirng in a sac file
 *
 * @param      s     sac file
 * @param      hdr   Header ID
 * @param      v     character string to set
 *
 * @return     success code, 1 on success, 0 on failure
 */
int
sac_set_string(sac *s, int hdr, char *v) {
    char *k = NULL;
    if(!s || hdr < SAC_STA || hdr > SAC_INST || !v) {
        return 0;
    }
    if(!(k = khdr(s, hdr-SAC_STA+1))) {
        return 0;
    }
    sacio_strlcpy(k, v, (hdr == SAC_EVENT) ? 17 : 9);
    return 1;
}

/**
 * @brief      format a timing mark with name
 *
 * @details    format a timing mark with its name
 *
 * @param      s         sac file
 * @param      time_id   time mark id
 * @param      name_id   name id
 * @param      dst       output character string
 * @param      n         length of \p dst
 *
 * @private
 * @memberof sac
 * @ingroup sac
 */
void
sac_timing_mark(sac *s, int time_id, int name_id, char *dst, size_t n) {
    if(sac_hdr_defined(s, time_id, NULL)) {
        double time = 0.0;
        char name[32] = {0};
        sac_get_float(s, time_id, &time);
        if(sac_hdr_defined(s, name_id, NULL)) {
            sac_get_string(s, name_id, name, sizeof(name));
            snprintf(dst, n, "%16.5g (%s)", time, name);
        } else {
            snprintf(dst, n, "%16.5g", time);
        }
    }
}

/**
 * @brief      Get a character string from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a character string from a sac file
 *
 * @param      s    sac file
 * @param      hdr  Header ID
 * @param      v    output character string
 * @param      n    length of \p v
 *
 * @return     status code, 0 on failure, 1 on success
 */
int
sac_get_string(sac *s, int hdr, char *v, size_t n) {
    char *k = NULL;
    if(!s || hdr < SAC_STA || hdr > SAC_T9MARKER || !v) {
        return 0;
    }
    if(hdr >= SAC_STA && hdr <= SAC_INST) {
        if(!(k = khdr(s, hdr-SAC_STA+1))) {
            return 0;
        }
        sacio_strlcpy(v, k, n);
    } else {
        switch(hdr) {
        case SAC_DATE: {
            timespec64 t = {0,0};
            sac_get_time_ref(s, &t);
            strftime64t(v, n, "%b %d (%j), %Y", &t);
        } break;
        case SAC_TIME: {
            timespec64 t = {0,0};
            sac_get_time_ref(s, &t);
            strftime64t(v, n, "%H:%M:%S.%03f", &t);
        } break;
        case SAC_FILENAME:
            sacio_strlcpy(v, s->m->filename, n);
            break;
        case SAC_AMARKER: sac_timing_mark(s, SAC_A, SAC_KA, v, n); break;
        case SAC_OMARKER: sac_timing_mark(s, SAC_O, SAC_KO, v, n); break;
        case SAC_T0MARKER: sac_timing_mark(s, SAC_T0, SAC_KT0, v, n); break;
        case SAC_T1MARKER: sac_timing_mark(s, SAC_T1, SAC_KT1, v, n); break;
        case SAC_T2MARKER: sac_timing_mark(s, SAC_T2, SAC_KT2, v, n); break;
        case SAC_T3MARKER: sac_timing_mark(s, SAC_T3, SAC_KT3, v, n); break;
        case SAC_T4MARKER: sac_timing_mark(s, SAC_T4, SAC_KT4, v, n); break;
        case SAC_T5MARKER: sac_timing_mark(s, SAC_T5, SAC_KT5, v, n); break;
        case SAC_T6MARKER: sac_timing_mark(s, SAC_T6, SAC_KT6, v, n); break;
        case SAC_T7MARKER: sac_timing_mark(s, SAC_T7, SAC_KT7, v, n); break;
        case SAC_T8MARKER: sac_timing_mark(s, SAC_T8, SAC_KT8, v, n); break;
        case SAC_T9MARKER: sac_timing_mark(s, SAC_T9, SAC_KT9, v, n); break;
        case SAC_STCMP:
            if(!sac_hdr_defined(s, SAC_STA, NULL)) {
                sacio_strlcpy(v, "UNDEFINED", n);
            }
            if(sac_hdr_defined(s, SAC_CHA, NULL)) {
                sac_fmt(v, n, "%S %C", s);
            } else {
                sac_fmt(v, n, "%S %c", s);
            }
            break;
        }
    }
    return 1;
}

/**
 * @brief      Set a floating point value in a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a flaoting point value in a sac file
 *
 * @param      s     sac file
 * @param      hdr   Header ID
 * @param      v     floating point to set
 *
 * @return     success code, 1 on success, 0 on failure
 */
int
sac_set_float(sac *s, int hdr, double v) {
    if(!s || hdr < SAC_DELTA || hdr > SAC_UN70) {
        return 0;
    }
    return sac_set_f64(s, hdr, v);
}
/**
 * @brief      Get a floating point value from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a floating point value from a sac file
 *
 * @param      s    sac file
 * @param      hdr  Header ID
 * @param      v    output floating point value
 *
 * @return     status code, 0 on failure, 1 on success
 */
int
sac_get_float(sac *s, int hdr, double *v) {
    if(!s || hdr < SAC_DELTA || hdr > SAC_UN70 || !v) {
        return 0;
    }
    fprintf(stderr, "hdr: %d\n", hdr);
    return sac_get_f64(s, hdr, v);
}
/**
 * @brief      Set a integer value in a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a integer in a sac file
 *
 * @param      s     sac file
 * @param      hdr   Header ID
 * @param      v     integer value to set
 *
 * @return     success code, 1 on success, 0 on failure
 */
int
sac_set_int(sac *s, int hdr, int v) {
    if(!s || hdr < SAC_YEAR || hdr > SAC_UN110) {
        return 0;
    }
    int *ip = (int *) (&(s->h->nzyear));
    ip[hdr - SAC_YEAR] = v;
    return 1;
}
/**
 * @brief      Get a integer value from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a integer value from a sac file
 *
 * @param      s    sac file
 * @param      hdr  Header ID
 * @param      v    output integer value
 *
 * @return     status code, 0 on failure, 1 on success
 */
int
sac_get_int(sac *s, int hdr, int *v) {
    if(!s || hdr < SAC_YEAR || hdr > SAC_UN110 || !v) {
        return 0;
    }
    int *ip = (int *) (&(s->h->nzyear));
    *v = ip[hdr - SAC_YEAR];
    return 1;
}

/** \cond NO_DOCS */
#define is_float(x) ( x >= SAC_DELTA && h <= SAC_UN70 )
#define is_int(x)   ( x >= SAC_YEAR && h <= SAC_UN105 )
#define is_str(x)   ( x >= SAC_STA && h <= SAC_INST )
/** \endcond */

/**
 * @brief      Check if multiple header values are defined
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Check if mulutple header values are defined
 *
 * @param      s    sac file
 * @param      ...  Header id, NULL terminatated
 *
 * @return     0 if any are undefined, 1 if all are defined
 */
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
            double v = 0.0;
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
        } else {
            printf("Warning: unknown header id: %d\n", h);
        }
    }
    va_end(ap);
    return 1;
}

/**
 * @brief     Get the reference time from a sac file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details   Get the refereence time from a sac file
 *
 * @param s   sac file
 * @param t   timespec64 output value
 *
 * @return 1 on success, 0 in failure
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
 * @brief      Get time value from sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get time value from sac file
 *
 *
 * @param s    sac file
 * @param hdr  header index value
 * @param t    timespec64 output value
 *
 * @return 1 in success, 0 on failure
 *
 */
int
sac_get_time(sac *s, int hdr, timespec64 *t) {
    double v = 0.0;
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
 * @ingroup  sac
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
    int j = 0;
    struct TM tm = {0};
    timespec64 t0;
    double dt = 0.0;
    if(sac_get_time_ref(s, &t0)) {
        int64_t dtf = t.tv_sec - t0.tv_sec;
        int64_t dtp = t.tv_nsec - t0.tv_nsec;
        dt = (double) dtf + (double) dtp / 1e9;
    }
    gmtime64_r(&t.tv_sec, &tm);
    s->h->iztype = IO;
    s->h->nzyear = tm.tm_year + 1900;
    s->h->nzjday = tm.tm_yday + 1;;
    s->h->nzhour = tm.tm_hour;
    s->h->nzmin  = tm.tm_min;
    s->h->nzsec  = tm.tm_sec;
    s->h->nzmsec = t.tv_nsec / 1000000;
    sac_set_float(s, SAC_O, dt);
    for(j = SAC_B; j <= SAC_F; j++) {
        double v = 0.0;
        sac_get_float(s, j, &v);
        if(v != SAC_FLOAT_UNDEFINED) {
            sac_set_float(s, j, v - dt);
        }
    }
    return 1;
}


/**
 * @brief      Copy a sac character string or "" if undefined
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy a sac character string or "" if undefined.
 *             If the \p src value is defined, not "-12345   "
 *             then \p src is copied to \dst and trailing whitespace
 *             is removed
 *
 * @param      dst  Ouptut character string
 * @param      src  Input charater string
 * @param      n    Length of \p dst
 *
 * @return     full length of output dst
 */
size_t
sac_strlcat(char *dst, char *src, size_t n) {
    char tmp[20] = {0};
    if(strcmp(src, SAC_CHAR_UNDEFINED) != 0) {
        sacio_strlcpy(tmp, src, sizeof(tmp));
        sacio_rstrip(tmp);
        return sacio_strlcat(dst, tmp, n);
    }
    return strlen(dst);
}

/**
 * @brief      Copy an absolute time value to a string, if defined
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy an absolute time value in the form of
 *             1994/06/09T00:33:16.123 if a refernce time is defined in
 *             the sac file 
 *
 * @param      dst    Output character string
 * @param      s      sac file to get time value from
 * @param      hdr    header ID
 * @param      n      length of output \p dst
 *
 * @return     full length of \p dst
 */
size_t
sac_timelcat(char *dst, sac *s, int hdr, size_t n) {
    char tmp[64] = {0};
    timespec64 t = {0,0};
    if(sac_get_time(s, hdr, &t)) {
        strftime64t(tmp, sizeof(tmp), "%FT%T.%3f", &t);
        return sacio_strlcat(dst, tmp, n);
    }
    return strlen(dst);
}

/**
 * @brief      Copy a floating point value to a string
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy a floating point value to a string using '%g'
 *
 * @param      dst   Output character string
 * @param      s     sac file to get header value from
 * @param      hdr   Header ID
 * @param      n     Length of \p dst
 *
 * @return     full length of \p dst
 */
size_t
sac_floatlcat(char *dst, sac *s, int hdr, size_t n) {
    double v = 0.0;
    char tmp[64] = {0};
    sac_get_float(s, hdr, &v);
    if(v != SAC_FLOAT_UNDEFINED) {
        snprintf(tmp, sizeof(dst), "%g", v);
    }
    return sacio_strlcat(dst, tmp, n);
}


/**
 * @brief      create a new sac header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    create and initialize a new sac header
 *
 * @return     newly created sac header
 */
sac_hdr *
sac_hdr_new() {
    sac_hdr *sh;
    sh = (sac_hdr *) malloc(sizeof(sac_hdr));
    if (sh) {
        sac_hdr_init(sh);
    }
    return sh;
}

/**
 * @brief      format sac values into a string
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    format sac values into a string
 *             - %%  Output the '%' character
 *             - %%E Event Name (kevnm)
 *             - %%I Instrument (kinst)
 *             - %%N Network (knetwk)
 *             - %%S Station (kstnm)
 *             - %%C Channel (kcmpnm)
 *             - %%H Location or Hole (khole)
 *             - %%L Location or Hole (khole), if value is "" or undefined, use "--".
 *             - %%t Relative time formatting (seconds from reference) `12.64` .
 *               All relative timing uses lowercase characters
 *               - %%tb begin time
 *               - %%te end time
 *               - %%to origin time
 *               - %%ta arrival time
 *               - %%t0 .. %%t9 time picks 0 to 9
 *             - %%T Absolute time formatting, e.g. `1994/06/09T00:33:16.123` .
 *               All absolute timing uses uppercase characters
 *               - %%tB begin time
 *               - %%tE end time
 *               - %%tO origin time
 *               - %%tA arrival time
 *               - %%t0 .. %%t9 time picks 0 to 9
 *             - %%Z alias for `%%N.%%S.%%H.%%C`  net.sta.loc.cha
 *             - %%R alias for `%%N %%S %%L %%C %%TB %%TE` similar to a data request
 *
 * @param      dst  Output character string
 * @param      n    Length of \p dst
 * @param      fmt  Formatting string
 * @param      s    sac file to get values from
 *
 * @return     full length of \p dst, -1 on error
 */
int 
sac_fmt(char *dst, size_t n, const char *fmt, sac *s) {
    int i = 0;
    int j = 0;
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
        case 'c': {
            if(s->h->cmpinc == 0.0) {
                i = sac_strlcat(dst, "VERT", n);
            } else if(s->h->cmpinc == 90.0) {
                float az = fmodf(s->h->cmpaz + 360.0, 360.0);
                if(az < 0.0) { az += 360.0; }

                if(fabs(az - 0.0) < 0.1) {
                    i = sac_strlcat(dst, "NORTH", n);
                } else if(fabs(az - 90.0) < 0.1) {
                    i = sac_strlcat(dst, "EAST", n);
                } else if(fabs(az - 180.0) < 0.1) {
                    i = sac_strlcat(dst, "SOUTH", n);
                } else if(fabs(az - 270.0) < 0.1) {
                    i = sac_strlcat(dst, "WEST", n);
                } else if(sac_hdr_defined(s, SAC_CMPAZ, SAC_CMPINC, NULL)) {
                    snprintf(dst, n, "%s %4d %4d", dst, (int)round(s->h->cmpaz), (int)round(s->h->cmpinc));
                    i = strlen(dst);
                }
            } else if(sac_hdr_defined(s, SAC_CMPAZ, SAC_CMPINC, NULL)) {
                snprintf(dst, n, "%s %4d %4d", dst, (int)round(s->h->cmpaz), (int)round(s->h->cmpinc));
                i = strlen(dst);
            }
        } break;
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
    sac_set_float(s, SAC_B, 0.0);
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

