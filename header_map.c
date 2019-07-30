/* C code produced by gperf version 3.0.3 */
/* Command-line: /Library/Developer/CommandLineTools/usr/bin/gperf -t -T --output-file=header_map.c -N sac_keyword_to_header header_map.txt  */
/* Computed positions: -k'1-3,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "header_map.txt"

#include <stdio.h>
#include "sacio.h"
#include <string.h>

#define TOTAL_KEYWORDS 128
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 351
/* maximum key range = 351, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
static unsigned char gperf_downcase[256] =
  {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255
  };
#endif

#ifndef GPERF_CASE_STRCMP
#define GPERF_CASE_STRCMP 1
static int
gperf_case_strcmp (s1, s2)
     register const char *s1;
     register const char *s2;
{
  for (;;)
    {
      unsigned char c1 = gperf_downcase[(unsigned char)*s1++];
      unsigned char c2 = gperf_downcase[(unsigned char)*s2++];
      if (c1 != 0 && c1 == c2)
        continue;
      return (int)c1 - (int)c2;
    }
}
#endif

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static unsigned short asso_values[] =
    {
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352,  75,  60,
       10, 140, 135, 120, 110, 105, 100,  90, 352, 352,
      352, 352, 352, 352, 352,   5,  80,  10,  35,   0,
       60, 120, 145,  15,   0,   0,  25,   5,   0,  55,
       35,  60,  15,   5,  25,  70,  65,  20,  50, 180,
        0, 352, 352, 352, 352, 352, 352,   5,  80,  10,
       35,   0,  60, 120, 145,  15,   0,   0,  25,   5,
        0,  55,  35,  60,  15,   5,  25,  70,  65,  20,
       50, 180,   0, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
      352, 352, 352, 352, 352, 352
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct hid *
sac_keyword_to_header (str, len)
     register const char *str;
     register unsigned int len;
{
  static struct hid wordlist[] =
    {
      {"",-1,-1},
#line 18 "header_map.txt"
      {"e",	    SAC_FLOAT_TYPE,	SAC_E},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 120 "header_map.txt"
      {"knetwk",	SAC_STRING_TYPE,	SAC_NET},
#line 63 "header_map.txt"
      {"az",	    SAC_FLOAT_TYPE,	SAC_AZ},
      {"",-1,-1}, {"",-1,-1},
#line 75 "header_map.txt"
      {"nzmin",	SAC_INT_TYPE,	SAC_MIN},
#line 20 "header_map.txt"
      {"a",	    SAC_FLOAT_TYPE,	SAC_A},
#line 104 "header_map.txt"
      {"ka",	    SAC_STRING_TYPE,	SAC_KA},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 82 "header_map.txt"
      {"nsnpts",	SAC_INT_TYPE,	SAC_NSNPTS},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 76 "header_map.txt"
      {"nzsec",	SAC_INT_TYPE,	SAC_SEC},
#line 77 "header_map.txt"
      {"nzmsec",	SAC_INT_TYPE,	SAC_MSEC},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 15 "header_map.txt"
      {"scale",	SAC_FLOAT_TYPE, SAC_SCALE},
#line 119 "header_map.txt"
      {"kcmpnm",	SAC_STRING_TYPE,	SAC_CHA},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 124 "header_map.txt"
      {"kztime",	SAC_AUX_TYPE,	SAC_TIME},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 35 "header_map.txt"
      {"resp2",	SAC_FLOAT_TYPE,	SAC_RESP2},
      {"",-1,-1},
#line 127 "header_map.txt"
      {"amarker",	SAC_AUX_TYPE,	SAC_AMARKER},
      {"",-1,-1}, {"",-1,-1},
#line 100 "header_map.txt"
      {"kstnm",	SAC_STRING_TYPE,	SAC_STA},
#line 123 "header_map.txt"
      {"kzdate",	SAC_AUX_TYPE,	SAC_DATE},
#line 95 "header_map.txt"
      {"imagsrc",	SAC_ENUM_TYPE,	SAC_MAG_SRC},
      {"",-1,-1}, {"",-1,-1},
#line 122 "header_map.txt"
      {"kinst",	SAC_STRING_TYPE,	SAC_INST},
#line 88 "header_map.txt"
      {"iztype",	SAC_ENUM_TYPE,	SAC_ZERO_TIME},
#line 24 "header_map.txt"
      {"t2",	    SAC_FLOAT_TYPE,	SAC_T2},
#line 107 "header_map.txt"
      {"kt2",	  SAC_STRING_TYPE,	SAC_KT2},
      {"",-1,-1}, {"",-1,-1},
#line 99 "header_map.txt"
      {"lcalda",	SAC_BOOL_TYPE,	SAC_CALC_DIST_AZ},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 66 "header_map.txt"
      {"cmpaz",	SAC_FLOAT_TYPE,	SAC_CMPAZ},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 45 "header_map.txt"
      {"stel",	  SAC_FLOAT_TYPE,	SAC_STEL},
#line 89 "header_map.txt"
      {"iinst",	SAC_ENUM_TYPE,	SAC_INST_TYPE},
#line 84 "header_map.txt"
      {"nxsize",	SAC_INT_TYPE,	SAC_NX},
      {"",-1,-1},
#line 132 "header_map.txt"
      {"t2marker",	SAC_AUX_TYPE,	SAC_T2MARKER},
#line 43 "header_map.txt"
      {"stla",	  SAC_FLOAT_TYPE,	SAC_STLA},
      {"",-1,-1},
#line 67 "header_map.txt"
      {"cmpinc",	SAC_FLOAT_TYPE,	SAC_CMPINC},
#line 94 "header_map.txt"
      {"imagtyp",	SAC_ENUM_TYPE,	SAC_MAG_TYPE},
      {"",-1,-1},
#line 81 "header_map.txt"
      {"npts",	  SAC_INT_TYPE,	SAC_NPTS},
#line 12 "header_map.txt"
      {"delta",	SAC_FLOAT_TYPE, SAC_DELTA},
#line 125 "header_map.txt"
      {"kstcmp",	SAC_AUX_TYPE,	SAC_STCMP},
      {"",-1,-1},
#line 69 "header_map.txt"
      {"xmaximum",	SAC_FLOAT_TYPE,	SAC_XMAX},
      {"",-1,-1},
#line 101 "header_map.txt"
      {"kevnm",	SAC_LONG_STRING_TYPE,	SAC_EVENT},
#line 13 "header_map.txt"
      {"depmin",	SAC_FLOAT_TYPE, SAC_DEPMIN},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 121 "header_map.txt"
      {"kdatrd",	SAC_STRING_TYPE,	SAC_DATRD},
      {"",-1,-1},
#line 68 "header_map.txt"
      {"xminimum",	SAC_FLOAT_TYPE,	SAC_XMIN},
#line 62 "header_map.txt"
      {"dist",	  SAC_FLOAT_TYPE,	SAC_DIST},
#line 34 "header_map.txt"
      {"resp1",	SAC_FLOAT_TYPE,	SAC_RESP1},
      {"",-1,-1},
#line 128 "header_map.txt"
      {"omarker",	SAC_AUX_TYPE,	SAC_OMARKER},
#line 64 "header_map.txt"
      {"baz",	  SAC_FLOAT_TYPE,	SAC_BAZ},
#line 87 "header_map.txt"
      {"idep",	  SAC_ENUM_TYPE,	SAC_DEP_TYPE},
#line 54 "header_map.txt"
      {"user2",	SAC_FLOAT_TYPE,	SAC_USER2},
#line 118 "header_map.txt"
      {"kuser2",	SAC_STRING_TYPE,	SAC_USER2},
#line 129 "header_map.txt"
      {"fmarker",	SAC_AUX_TYPE,	SAC_FMARKER},
      {"",-1,-1},
#line 49 "header_map.txt"
      {"evel",	  SAC_FLOAT_TYPE,	SAC_EVEL},
#line 96 "header_map.txt"
      {"leven",	SAC_BOOL_TYPE,	SAC_EVEN},
#line 97 "header_map.txt"
      {"lpspol",	SAC_BOOL_TYPE,	SAC_POLARITY},
      {"",-1,-1}, {"",-1,-1},
#line 47 "header_map.txt"
      {"evla",	  SAC_FLOAT_TYPE,	SAC_EVLA},
#line 33 "header_map.txt"
      {"resp0",	SAC_FLOAT_TYPE,	SAC_RESP0},
#line 16 "header_map.txt"
      {"odelta",	SAC_FLOAT_TYPE,	SAC_ODELTA},
      {"",-1,-1}, {"",-1,-1},
#line 46 "header_map.txt"
      {"stdp",	  SAC_FLOAT_TYPE,	SAC_STDP},
#line 80 "header_map.txt"
      {"nevid",	SAC_INT_TYPE,	SAC_EVID},
#line 86 "header_map.txt"
      {"iftype",	SAC_ENUM_TYPE,	SAC_FILE_TYPE},
      {"",-1,-1},
#line 126 "header_map.txt"
      {"filename",	SAC_AUX_TYPE,	SAC_FILENAME},
      {"",-1,-1},
#line 79 "header_map.txt"
      {"norid",	SAC_INT_TYPE,	SAC_ORID},
#line 19 "header_map.txt"
      {"o",	    SAC_FLOAT_TYPE,	SAC_O},
#line 103 "header_map.txt"
      {"ko",	    SAC_STRING_TYPE,	SAC_KO},
#line 131 "header_map.txt"
      {"t1marker",	SAC_AUX_TYPE,	SAC_T1MARKER},
#line 44 "header_map.txt"
      {"stlo",	  SAC_FLOAT_TYPE,	SAC_STLO},
#line 42 "header_map.txt"
      {"resp9",	SAC_FLOAT_TYPE,	SAC_RESP9},
      {"",-1,-1}, {"",-1,-1},
#line 21 "header_map.txt"
      {"fmt",	  SAC_FLOAT_TYPE,	SAC_FMT},
      {"",-1,-1},
#line 83 "header_map.txt"
      {"nwfid",	SAC_INT_TYPE,	SAC_WFID},
#line 32 "header_map.txt"
      {"f",	    SAC_FLOAT_TYPE,	SAC_F},
#line 115 "header_map.txt"
      {"kf",	    SAC_STRING_TYPE,	SAC_KF},
      {"",-1,-1}, {"",-1,-1},
#line 41 "header_map.txt"
      {"resp8",	SAC_FLOAT_TYPE,	SAC_RESP8},
#line 14 "header_map.txt"
      {"depmax",	SAC_FLOAT_TYPE, SAC_DEPMAX},
      {"",-1,-1},
#line 130 "header_map.txt"
      {"t0marker",	SAC_AUX_TYPE,	SAC_T0MARKER},
      {"",-1,-1},
#line 40 "header_map.txt"
      {"resp7",	SAC_FLOAT_TYPE,	SAC_RESP7},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 39 "header_map.txt"
      {"resp6",	SAC_FLOAT_TYPE,	SAC_RESP6},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 50 "header_map.txt"
      {"evdp",	  SAC_FLOAT_TYPE,	SAC_EVDP},
#line 53 "header_map.txt"
      {"user1",	SAC_FLOAT_TYPE,	SAC_USER1},
#line 117 "header_map.txt"
      {"kuser1",	SAC_STRING_TYPE,	SAC_USER1},
      {"",-1,-1},
#line 139 "header_map.txt"
      {"t9marker",	SAC_AUX_TYPE,	SAC_T9MARKER},
      {"",-1,-1},
#line 38 "header_map.txt"
      {"resp5",	SAC_FLOAT_TYPE,	SAC_RESP5},
      {"",-1,-1},
#line 23 "header_map.txt"
      {"t1",	    SAC_FLOAT_TYPE,	SAC_T1},
#line 106 "header_map.txt"
      {"kt1",	  SAC_STRING_TYPE,	SAC_KT1},
#line 48 "header_map.txt"
      {"evlo",	  SAC_FLOAT_TYPE,	SAC_EVLO},
#line 65 "header_map.txt"
      {"gcarc",	SAC_FLOAT_TYPE,	SAC_GCARC},
#line 98 "header_map.txt"
      {"lovrok",	SAC_BOOL_TYPE,	SAC_OVERWRITE},
      {"",-1,-1},
#line 138 "header_map.txt"
      {"t8marker",	SAC_AUX_TYPE,	SAC_T8MARKER},
      {"",-1,-1},
#line 52 "header_map.txt"
      {"user0",	SAC_FLOAT_TYPE,	SAC_USER0},
#line 116 "header_map.txt"
      {"kuser0",	SAC_STRING_TYPE,	SAC_USER0},
      {"",-1,-1},
#line 137 "header_map.txt"
      {"t7marker",	SAC_AUX_TYPE,	SAC_T7MARKER},
      {"",-1,-1},
#line 37 "header_map.txt"
      {"resp4",	SAC_FLOAT_TYPE,	SAC_RESP4},
#line 17 "header_map.txt"
      {"b",	    SAC_FLOAT_TYPE,	SAC_B},
      {"",-1,-1},
#line 136 "header_map.txt"
      {"t6marker",	SAC_AUX_TYPE,	SAC_T6MARKER},
      {"",-1,-1},
#line 36 "header_map.txt"
      {"resp3",	SAC_FLOAT_TYPE,	SAC_RESP3},
#line 74 "header_map.txt"
      {"nzhour",	SAC_INT_TYPE,	SAC_HOUR},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 61 "header_map.txt"
      {"user9",	SAC_FLOAT_TYPE,	SAC_USER9},
#line 90 "header_map.txt"
      {"istreg",	SAC_ENUM_TYPE,	SAC_STREG},
      {"",-1,-1},
#line 135 "header_map.txt"
      {"t5marker",	SAC_AUX_TYPE,	SAC_T5MARKER},
      {"",-1,-1},
#line 92 "header_map.txt"
      {"iqual",	SAC_ENUM_TYPE,	SAC_QUAL},
      {"",-1,-1},
#line 22 "header_map.txt"
      {"t0",	    SAC_FLOAT_TYPE,	SAC_T0},
#line 105 "header_map.txt"
      {"kt0",	  SAC_STRING_TYPE,	SAC_KT0},
      {"",-1,-1},
#line 60 "header_map.txt"
      {"user8",	SAC_FLOAT_TYPE,	SAC_USER8},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 59 "header_map.txt"
      {"user7",	SAC_FLOAT_TYPE,	SAC_USER7},
#line 73 "header_map.txt"
      {"nzjday",	SAC_INT_TYPE,	SAC_DAY},
      {"",-1,-1},
#line 134 "header_map.txt"
      {"t4marker",	SAC_AUX_TYPE,	SAC_T4MARKER},
      {"",-1,-1},
#line 58 "header_map.txt"
      {"user6",	SAC_FLOAT_TYPE,	SAC_USER6},
#line 85 "header_map.txt"
      {"nysize",	SAC_INT_TYPE,	SAC_NY},
      {"",-1,-1},
#line 133 "header_map.txt"
      {"t3marker",	SAC_AUX_TYPE,	SAC_T3MARKER},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 57 "header_map.txt"
      {"user5",	SAC_FLOAT_TYPE,	SAC_USER5},
#line 72 "header_map.txt"
      {"nzyear",	SAC_INT_TYPE,	SAC_YEAR},
      {"",-1,-1},
#line 71 "header_map.txt"
      {"ymaximum",	SAC_FLOAT_TYPE,	SAC_YMAX},
      {"",-1,-1},
#line 102 "header_map.txt"
      {"khole",	SAC_STRING_TYPE,	SAC_HOLE},
#line 91 "header_map.txt"
      {"ievreg",	SAC_ENUM_TYPE,	SAC_EVREG},
#line 31 "header_map.txt"
      {"t9",	    SAC_FLOAT_TYPE,	SAC_T9},
#line 114 "header_map.txt"
      {"kt9",	  SAC_STRING_TYPE,	SAC_KT9},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 70 "header_map.txt"
      {"yminimum",	SAC_FLOAT_TYPE,	SAC_YMIN},
      {"",-1,-1},
#line 56 "header_map.txt"
      {"user4",	SAC_FLOAT_TYPE,	SAC_USER4},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 55 "header_map.txt"
      {"user3",	SAC_FLOAT_TYPE,	SAC_USER3},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 30 "header_map.txt"
      {"t8",	    SAC_FLOAT_TYPE,	SAC_T8},
#line 113 "header_map.txt"
      {"kt8",	  SAC_STRING_TYPE,	SAC_KT8},
      {"",-1,-1},
#line 78 "header_map.txt"
      {"nvhdr",	SAC_INT_TYPE,	SAC_HDR},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 29 "header_map.txt"
      {"t7",	    SAC_FLOAT_TYPE,	SAC_T7},
#line 112 "header_map.txt"
      {"kt7",	  SAC_STRING_TYPE,	SAC_KT7},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 28 "header_map.txt"
      {"t6",	    SAC_FLOAT_TYPE,	SAC_T6},
#line 111 "header_map.txt"
      {"kt6",	  SAC_STRING_TYPE,	SAC_KT6},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 51 "header_map.txt"
      {"mag",	  SAC_FLOAT_TYPE,	SAC_MAG},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1},
#line 27 "header_map.txt"
      {"t5",	    SAC_FLOAT_TYPE,	SAC_T5},
#line 110 "header_map.txt"
      {"kt5",	  SAC_STRING_TYPE,	SAC_KT5},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 26 "header_map.txt"
      {"t4",	    SAC_FLOAT_TYPE,	SAC_T4},
#line 109 "header_map.txt"
      {"kt4",	  SAC_STRING_TYPE,	SAC_KT4},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
#line 25 "header_map.txt"
      {"t3",	    SAC_FLOAT_TYPE,	SAC_T3},
#line 108 "header_map.txt"
      {"kt3",	  SAC_STRING_TYPE,	SAC_KT3},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1}, {"",-1,-1}, {"",-1,-1},
      {"",-1,-1}, {"",-1,-1},
#line 93 "header_map.txt"
      {"isynth",	SAC_ENUM_TYPE,	SAC_SYNTH}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 140 "header_map.txt"


#ifdef __TESTING__
//struct hid { char *name; int  type; int  id; };
int
main() {
       struct hid *h = NULL;
       h = sac_keyword_to_header("delta", 5);
       printf("%s %d %d\n", h->name, h->type, h->id);
       h = sac_keyword_to_header("delt", 4);
       if(h) {
          printf("%s %d %d\n", h->name, h->type, h->id);
       }
}
#endif
