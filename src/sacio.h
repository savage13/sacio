/**
 * @file sacio.h
 *
 * @brief SAC Input/Output Functions
 */

#ifndef __SACIO_H__
#define __SACIO_H__

#include <stdlib.h>

#include "timespec.h"

/** 
 * Define an automatic string length from C
 *
 */
#define SAC_STRING_LENGTH   -1

#ifndef TRUE
#define TRUE  1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#define SAC_FLOAT_UNDEFINED      -12345.0
#define SAC_REAL_UNDEFINED       SAC_FLOAT_UNDEFINED
#define SAC_INT_UNDEFINED        -12345
#define SAC_INTEGER_UNDEFINED    SAC_INT_UNDEFINED
#define SAC_NUMBER_UNDEFINED     SAC_INT_UNDEFINED
#define SAC_CHAR_UNDEFINED       "-12345  "
#define SAC_CHARACTER_UNDEFINED  SAC_CHAR_UNDEFINED
#define SAC_ENUM_UNDEFINED       SAC_INT_UNDEFINED
#define SAC_LOGICAL_UNDEFINED    SAC_INT_UNDEFINED
#define SAC_CHAR_UNDEFINED_2     "-12345          "    /* Must be 16 characters */

void getfhv(char      *kname, 
            float     *fvalue, 
            int       *nerr, 
            int        kname_s);

void getihv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void getkhv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void getlhv(char      *kname, 
            int       *lvalue, 
            int       *nerr, 
            int        kname_s);

void getnhv(char      *kname, 
            int       *nvalue, 
            int       *nerr, 
            int        kname_s);

void newhdr (void);

void rsac1(char      *kname, 
           float      yarray[], 
           int       *nlen, 
           float     *beg, 
           float     *del, 
           int       *max_, 
           int       *nerr, 
           int        kname_s);

void rsac2(char      *kname, 
           float      yarray[], 
           int       *nlen, 
           float      xarray[], 
           int       *max_, 
           int       *nerr, 
           int        kname_s);

void rsach(char      *kname,
           int       *nerr,
           int        kname_s);

void setfhv(char      *kname, 
            float     *fvalue, 
            int       *nerr, 
            int        kname_s);

void setihv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s,
            int        kvalue_s);

void setkhv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s,
            int        kvalue_s);

void setlhv(char      *kname, 
            int       *lvalue, 
            int       *nerr, 
            int        kname_s);

void setnhv(char      *kname, 
            int       *nvalue, 
            int       *nerr, 
            int        kname_s);

void wsac0(char      *kname, 
           float     *xarray, 
           float     *yarray,
           int       *nerr,
           int        kname_s);

void wsac1(char      *kname, 
           float     *yarray, 
           int       *nlen, 
           float     *beg, 
           float     *del, 
           int       *nerr, 
           int        kname_s);

void wsac2(char      *kname, 
           float     *yarray, 
           int       *nlen, 
           float     *xarray, 
           int       *nerr, 
           int        kname_s);

void wsac3(char      *kname, 
           float     *xarray, 
           float     *yarray,
           int       *nerr,
           int        kname_s);

void getbbv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void writebbf(char      *kname, 
              int       *nerr, 
              int        kname_s);

void readbbf(char      *kname, 
             int       *nerr, 
             int        kname_s);


void setbbv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void sac_warning_stdout(void);
void sac_warning_stderr(void);
void sac_warning_off(void);

void sac_error_stdout(void);
void sac_error_stderr(void);
void sac_error_off(void);

void sac_output_stdout(void);
void sac_output_stderr(void);
void sac_output_off(void);


typedef struct SACheader sac_hdr;
/* SAC header structure */
struct SACheader {
    float delta;                /* RF time increment, sec    */
    float depmin;               /*    minimum amplitude      */
    float depmax;               /*    maximum amplitude      */
    float scale;                /*    amplitude scale factor */
    float odelta;               /*    observed time inc      */
    float b;                    /* RD initial value, time    */
    float e;                    /* RD final value, time      */
    float o;                    /*    event start, sec < nz. */
    float a;                    /*    1st arrival time       */
    float fmt;                  /*    internal use           */
    float t0;                   /*    user-defined time pick */
    float t1;                   /*    user-defined time pick */
    float t2;                   /*    user-defined time pick */
    float t3;                   /*    user-defined time pick */
    float t4;                   /*    user-defined time pick */
    float t5;                   /*    user-defined time pick */
    float t6;                   /*    user-defined time pick */
    float t7;                   /*    user-defined time pick */
    float t8;                   /*    user-defined time pick */
    float t9;                   /*    user-defined time pick */
    float f;                    /*    event end, sec > nz    */
    float resp0;                /*    instrument respnse parm */
    float resp1;                /*    instrument respnse parm */
    float resp2;                /*    instrument respnse parm */
    float resp3;                /*    instrument respnse parm */
    float resp4;                /*    instrument respnse parm */
    float resp5;                /*    instrument respnse parm */
    float resp6;                /*    instrument respnse parm */
    float resp7;                /*    instrument respnse parm */
    float resp8;                /*    instrument respnse parm */
    float resp9;                /*    instrument respnse parm */
    float stla;                 /*  T station latititude     */
    float stlo;                 /*  T station longitude      */
    float stel;                 /*  T station elevation, m   */
    float stdp;                 /*  T station depth, m      */
    float evla;                 /*    event latitude         */
    float evlo;                 /*    event longitude        */
    float evel;                 /*    event elevation        */
    float evdp;                 /*    event depth            */
    float mag;                  /*    reserved for future use */
    float user0;                /*    available to user      */
    float user1;                /*    available to user      */
    float user2;                /*    available to user      */
    float user3;                /*    available to user      */
    float user4;                /*    available to user      */
    float user5;                /*    available to user      */
    float user6;                /*    available to user      */
    float user7;                /*    available to user      */
    float user8;                /*    available to user      */
    float user9;                /*    available to user      */
    float dist;                 /*    stn-event distance, km */
    float az;                   /*    event-stn azimuth      */
    float baz;                  /*    stn-event azimuth      */
    float gcarc;                /*    stn-event dist, degrees */
    float sb;                   /*    internal use           */
    float sdelta;               /*    internal use           */
    float depmen;               /*    mean value, amplitude  */
    float cmpaz;                /*  T component azimuth     */
    float cmpinc;               /*  T component inclination */
    float xminimum;             /*    reserved for future use */
    float xmaximum;             /*    reserved for future use */
    float yminimum;             /*    reserved for future use */
    float ymaximum;             /*    reserved for future use */
    float unused6;              /*    reserved for future use */
    float unused7;              /*    reserved for future use */
    float unused8;              /*    reserved for future use */
    float unused9;              /*    reserved for future use */
    float unused10;             /*    reserved for future use */
    float unused11;             /*    reserved for future use */
    float unused12;             /*    reserved for future use */
    int nzyear;                 /*  F zero time of file, yr  */
    int nzjday;                 /*  F zero time of file, day */
    int nzhour;                 /*  F zero time of file, hr  */
    int nzmin;                  /*  F zero time of file, min */
    int nzsec;                  /*  F zero time of file, sec */
    int nzmsec;                 /*  F zero time of file, msec */
    int nvhdr;                  /*    internal use           */
    int norid;                  /*    origin ID              */
    int nevid;                  /*    event ID               */
    int npts;                   /* RF number of samples      */
    int nsnpts;                 /*    internal use           */
    int nwfid;                  /*    waveform ID            */
    int nxsize;                 /*    reserved for future use */
    int nysize;                 /*    reserved for future use */
    int unused15;               /*    reserved for future use */
    int iftype;                 /* RA type of file          */
    int idep;                   /*    type of amplitude      */
    int iztype;                 /*    zero time equivalence  */
    int unused16;               /*    reserved for future use */
    int iinst;                  /*    recording instrument   */
    int istreg;                 /*    stn geographic region  */
    int ievreg;                 /*    event geographic region */
    int ievtyp;                 /*    event type             */
    int iqual;                  /*    quality of data        */
    int isynth;                 /*    synthetic data flag    */
    int imagtyp;                /*    reserved for future use */
    int imagsrc;                /*    reserved for future use */
    int unused19;               /*    reserved for future use */
    int unused20;               /*    reserved for future use */
    int unused21;               /*    reserved for future use */
    int unused22;               /*    reserved for future use */
    int unused23;               /*    reserved for future use */
    int unused24;               /*    reserved for future use */
    int unused25;               /*    reserved for future use */
    int unused26;               /*    reserved for future use */
    int leven;                  /* RA data-evenly-spaced flag */
    int lpspol;                 /*    station polarity flag  */
    int lovrok;                 /*    overwrite permission   */
    int lcalda;                 /*    calc distance, azimuth */
    int unused27;               /*    reserved for future use */
    char kstnm[9];              /*  F station name           */
    char kevnm[18];             /*    event name             */
    char khole[9];              /*    man-made event name    */
    char ko[9];                 /*    event origin time id   */
    char ka[9];                 /*    1st arrival time ident */
    char kt0[9];                /*    time pick 0 ident      */
    char kt1[9];                /*    time pick 1 ident      */
    char kt2[9];                /*    time pick 2 ident      */
    char kt3[9];                /*    time pick 3 ident      */
    char kt4[9];                /*    time pick 4 ident      */
    char kt5[9];                /*    time pick 5 ident      */
    char kt6[9];                /*    time pick 6 ident      */
    char kt7[9];                /*    time pick 7 ident      */
    char kt8[9];                /*    time pick 8 ident      */
    char kt9[9];                /*    time pick 9 ident      */
    char kf[9];                 /*    end of event ident     */
    char kuser0[9];             /*    available to user      */
    char kuser1[9];             /*    available to user      */
    char kuser2[9];             /*    available to user      */
    char kcmpnm[9];             /*  F component name         */
    char knetwk[9];             /*    network name           */
    char kdatrd[9];             /*    date data read         */
    char kinst[9];              /*    instrument name        */
};
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)

#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
  typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

CASSERT(sizeof(struct SACheader) == 656, SacHeader_h)

typedef struct _sacmeta sacmeta;
struct _sacmeta {
    int swap;
    char *filename;
    int data_read;
    int nstop;
    int nstart;
    int nfillb;
    int nfille;
    int ntotal;
};

typedef struct _sac sac;
struct _sac {
    struct SACheader *h;
    int n;
    float *y;
    float *x;
    sacmeta *m;
    int *sddhdr;           /* SDD Header - Length MWESHD - 164 */
};

typedef struct complexf_t complexf;
typedef struct complexd_t complexd;

struct complexf_t {
    float re;      /** Real Part */
    float im;      /** Imaginary Part */
};
struct complexd_t {
    double re;      /** Real Part */
    double im;      /** Imaginary Part */
};

typedef struct _datetime datetime;

struct _datetime {
    int year;                   /* Year */
    int month;                  /* Month */
    int day;                    /* Day of the month: */
    int doy;                    /* Day of the year: 1 - 366 */
    int hour;                   /* Hour:   0 - 23 */
    int minute;                 /* Minute: 0 - 59 */
    int second;                 /* Second: 0 - 59 */
    long long int nanosecond;   /* Nanoseconds */
    long long int time;
    int set;
};

sac * sac_new(void);
void  sac_free(sac *s);
sac * sac_read(char *filename, int *nerr);
sac * sac_read_header(char *filename, int *nerr);
void  sac_write(sac *s, char *filename, int *nerr);
sac * sac_copy(sac *s);
void  sac_extrema(sac *s);
int   sac_comps(sac * s);

char *rstrip(char *s);

int sac_get_float(sac *s, int hdr, float *v);
int sac_set_float(sac *s, int hdr, float v);
int sac_get_string(sac *s, int hdr, char *v, size_t n);
int sac_set_string(sac *s, int hdr, char *v);
int sac_get_int(sac *s, int hdr, int *v);
int sac_set_int(sac *s, int hdr, int v);
int sac_hdr_defined(sac *s, ...);
size_t sac_fmt(char *dst, size_t n, const char *fmt, sac *s);
int sac_get_time(sac *s, int hdr, timespec64 *t);
int sac_set_time(sac *s, timespec64 t);
size_t sac_size(sac *s);
void sac_be(sac *s);

#define SAC_WRITE_HEADER_AND_DATA 1
#define SAC_READ_HEADER_AND_DATA  1
#define SAC_WRITE_HEADER          0
#define SAC_READ_HEADER           0
#define SAC_NO_BYTESWAP_FILE      0
#define SAC_BYTESWAP_FILE         1

void update_distaz(sac * s);
void distaz(double the, double phe, float *ths, float *phs,
            int ns, float *dist, float *az, float *baz, float *xdeg,
            int *nerr);


#define ITIME  1
#define IO    11

enum {
    SAC_DELTA        = 1,
    SAC_DEPMIN       = 2,
    SAC_DEPMAX       = 3,
    SAC_SCALE        = 4,
    SAC_ODELTA       = 5,
    SAC_B            = 6,
    SAC_E            = 7,
    SAC_O            = 8,
    SAC_A            = 9,
    SAC_FMT          = 10,
    SAC_T0           = 11,
    SAC_T1           = 12,
    SAC_T2           = 13,
    SAC_T3           = 14,
    SAC_T4           = 15,
    SAC_T5           = 16,
    SAC_T6           = 17,
    SAC_T7           = 18,
    SAC_T8           = 19,
    SAC_T9           = 20,
    SAC_F            = 21,
    SAC_RESP0        = 22,
    SAC_RESP1        = 23,
    SAC_RESP2        = 24,
    SAC_RESP3        = 25,
    SAC_RESP4        = 26,
    SAC_RESP5        = 27,
    SAC_RESP6        = 28,
    SAC_RESP7        = 29,
    SAC_RESP8        = 30,
    SAC_RESP9        = 31,
    SAC_STLA         = 32,
    SAC_STLO         = 33,
    SAC_STEL         = 34,
    SAC_STDP         = 35,
    SAC_EVLA         = 36,
    SAC_EVLO         = 37,
    SAC_EVEL         = 38,
    SAC_EVDP         = 39,
    SAC_MAG          = 40,
    SAC_USER0        = 41,
    SAC_USER1        = 42,
    SAC_USER2        = 43,
    SAC_USER3        = 44,
    SAC_USER4        = 45,
    SAC_USER5        = 46,
    SAC_USER6        = 47,
    SAC_USER7        = 48,
    SAC_USER8        = 49,
    SAC_USER9        = 50,
    SAC_DIST         = 51,
    SAC_AZ           = 52,
    SAC_BAZ          = 53,
    SAC_GCARC        = 54,
    SAC_SB           = 55,
    SAC_SDELTA       = 56,
    SAC_DEPMEN       = 57,
    SAC_CMPAZ        = 58,
    SAC_CMPINC       = 59,
    SAC_XMIN         = 60,
    SAC_XMAX         = 61,
    SAC_YMIN         = 62,
    SAC_YMAX         = 63,
    SAC_UN64         = 64,
    SAC_UN65         = 65,
    SAC_UN66         = 66,
    SAC_UN67         = 67,
    SAC_UN68         = 68,
    SAC_UN69         = 69,
    SAC_UN70         = 70,
    SAC_YEAR         = 71,
    SAC_DAY          = 72,
    SAC_HOUR         = 73,
    SAC_MIN          = 74,
    SAC_SEC          = 75,
    SAC_MSEC         = 76,
    SAC_HDR          = 77,
    SAC_ORID         = 78,
    SAC_EVID         = 79,
    SAC_NPTS         = 80,
    SAC_NSNPTS       = 81,
    SAC_WFID         = 82,
    SAC_NX           = 83,
    SAC_NY           = 84,
    SAC_UN84         = 85,
    SAC_FILE_TYPE    = 86,
    SAC_DEP_TYPE     = 87,
    SAC_ZERO_TIME    = 88,
    SAC_UN88         = 89,
    SAC_INST_TYPE    = 90,
    SAC_STREG        = 91,
    SAC_EVREG        = 92,
    SAC_EVENT_TYPE   = 93,
    SAC_QUAL         = 94,
    SAC_SYNTH        = 95,
    SAC_MAG_TYPE     = 96,
    SAC_MAG_SRC      = 97,
    SAC_UN98         = 98,
    SAC_UN99         = 99,
    SAC_UN100        = 100,
    SAC_UN101        = 101,
    SAC_UN102        = 102,
    SAC_UN103        = 103,
    SAC_UN104        = 104,
    SAC_UN105        = 105,
    SAC_EVEN         = 106,
    SAC_POLARITY     = 107,
    SAC_OVERWRITE    = 108,
    SAC_CALC_DIST_AZ = 109,
    SAC_UN110        = 110,
    SAC_STA          = 111,
    SAC_EVENT        = 112,
    SAC_EVENT2       = 113,
    SAC_HOLE         = 114,
    SAC_KO           = 115,
    SAC_KA           = 116,
    SAC_KT0          = 117,
    SAC_KT1          = 118,
    SAC_KT2          = 119,
    SAC_KT3          = 120,
    SAC_KT4          = 121,
    SAC_KT5          = 122,
    SAC_KT6          = 123,
    SAC_KT7          = 124,
    SAC_KT8          = 125,
    SAC_KT9          = 126,
    SAC_KF           = 127,
    SAC_KUSER0       = 128,
    SAC_KUSER1       = 129,
    SAC_KUSER2       = 130,
    SAC_CHA          = 131,
    SAC_NET          = 132,
    SAC_DATRD        = 133,
    SAC_INST         = 134,
};

#define SAC_KSTNM  SAC_STA
#define SAC_KNETWK SAC_NET
#define SAC_KHOLE  SAC_LOC
#define SAC_CHAN   SAC_CHA
#define SAC_KCMPNM SAC_CHA
#define SAC_KEVENT SAC_EVENT
#define SAC_KEVNM SAC_EVENT
#define SAC_KINST  SAC_INST
#define SAC_LOC    SAC_HOLE


/* definitions of constants for SAC enumerated data values */
#define IREAL   0               /* undocumented              */
#define ITIME   1               /* file: time series data    */
#define IRLIM   2               /* file: real&imag spectrum  */
#define IAMPH   3               /* file: ampl&phas spectrum  */
#define IXY     4               /* file: gen'l x vs y data   */
#define IUNKN   5               /* x data: unknown type      */
                                        /* zero time: unknown        */
                                        /* event type: unknown       */
#define IDISP   6               /* x data: displacement (nm) */
#define IVEL    7               /* x data: velocity (nm/sec) */
#define IACC    8               /* x data: accel (nm/sec/sec) */
#define IB      9               /* zero time: start of file  */
#define IDAY   10               /* zero time: 0000 of GMT day */
#define IO     11               /* zero time: event origin   */
#define IA     12               /* zero time: 1st arrival    */
#define IT0    13               /* zero time: user timepick 0 */
#define IT1    14               /* zero time: user timepick 1 */
#define IT2    15               /* zero time: user timepick 2 */
#define IT3    16               /* zero time: user timepick 3 */
#define IT4    17               /* zero time: user timepick 4 */
#define IT5    18               /* zero time: user timepick 5 */
#define IT6    19               /* zero time: user timepick 6 */
#define IT7    20               /* zero time: user timepick 7 */
#define IT8    21               /* zero time: user timepick 8 */
#define IT9    22               /* zero time: user timepick 9 */
#define IRADNV 23               /* undocumented              */
#define ITANNV 24               /* undocumented              */
#define IRADEV 25               /* undocumented              */
#define ITANEV 26               /* undocumented              */
#define INORTH 27               /* undocumented              */
#define IEAST  28               /* undocumented              */
#define IHORZA 29               /* undocumented              */
#define IDOWN  30               /* undocumented              */
#define IUP    31               /* undocumented              */
#define ILLLBB 32               /* undocumented              */
#define IWWSN1 33               /* undocumented              */
#define IWWSN2 34               /* undocumented              */
#define IHGLP  35               /* undocumented              */
#define ISRO   36               /* undocumented              */

/* Source types */
#define INUCL  37               /* event type: nuclear shot  */
#define IPREN  38               /* event type: nuke pre-shot */
#define IPOSTN 39               /* event type: nuke post-shot */
#define IQUAKE 40               /* event type: earthquake    */
#define IPREQ  41               /* event type: foreshock     */
#define IPOSTQ 42               /* event type: aftershock    */
#define ICHEM  43               /* event type: chemical expl */
#define IOTHER 44               /* event type: other source  */
#define IXYZ   51
#define IQB    72               /* Quarry Blast or mine expl. confirmed by quarry */
#define IQB1   73               /* Quarry or mine blast with designed shot information-ripple fired */
#define IQB2   74               /* Quarry or mine blast with observed shot information-ripple fired */
#define IQBX   75               /* Quarry or mine blast - single shot */
#define IQMT   76               /* Quarry or mining-induced events: tremors and rockbursts */
#define IEQ    77               /* Earthquake */
#define IEQ1   78               /* Earthquakes in a swarm or aftershock sequence */
#define IEQ2   79               /* Felt earthquake */
#define IME    80               /* Marine explosion */
#define IEX    81               /* Other explosion */
#define INU    82               /* Nuclear explosion */
#define INC    83               /* Nuclear cavity collapse */
#define IO_    84               /* Other source of known origin */
#define IL     85               /* Local event of unknown origin */
#define IR     86               /* Regional event of unknown origin */
#define IT     87               /* Teleseismic event of unknown origin */
#define IU     88               /* Undetermined or conflicting information  */
#define IEQ3   89               /* Damaging earthquake */
#define IEQ0   90               /* Probable earthquake */
#define IEX0   91               /* Probable explosion */
#define IQC    92               /* Mine collapse */
#define IQB0   93               /* Probable Mine Blast */
#define IGEY   94               /* Geyser */
#define ILIT   95               /* Light */
#define IMET   96               /* Meteoric Event */
#define IODOR  97               /* Odors */
#define IOS   103               /* Other source: Known origin */

                                        /* data quality: other problm */
#define IGOOD  45               /* data quality: good        */
#define IGLCH  46               /* data quality: has glitches */
#define IDROP  47               /* data quality: has dropouts */
#define ILOWSN 48               /* data quality: low s/n     */

#define IRLDTA 49               /* data is real data         */
#define IVOLTS 50               /* file: velocity (volts)    */
#define IXYZ   51               /* General XYZ (3-D) file */

/* Magnitude type and source */
#define IMB    52               /* Bodywave Magnitude */
#define IMS    53               /* Surface Magnitude */
#define IML    54               /* Local Magnitude  */
#define IMW    55               /* Moment Magnitude */
#define IMD    56               /* Duration Magnitude */
#define IMX    57               /* User Defined Magnitude */
#define INEIC  58               /* INEIC */
#define IPDEQ  59               /* IPDE */
#define IPDEW  60               /* IPDE */
#define IPDE   61               /* IPDE */

#define IISC   62               /* IISC */
#define IREB   63               /* IREB */
#define IUSGS  64               /* IUSGS */
#define IBRK   65               /* IBRK */
#define ICALTECH 66             /* ICALTECH */
#define ILLNL  67               /* ILLNL */
#define IEVLOC 68               /* IEVLOC */
#define IJSOP  69               /* IJSOP */
#define IUSER  70               /* IUSER */
#define IUNKNOWN 71             /* IUNKNOWN */

#define REGCONV 100

#ifdef SAC_NULL_HEADER_REQUIRED
/* a SAC structure containing all null values */

static struct SACheader NullSacHeader = {
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    {'-', '1', '2', '3', '4', '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
     ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}
};
#endif /* SAC_NULL_HEADER_REQUIRED */


#endif /* __SACIO_H__ */
