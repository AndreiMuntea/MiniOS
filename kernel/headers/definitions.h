#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

typedef char                    INT8;
typedef short                   INT16;
typedef int                     INT32;
typedef long long               INT64;

typedef unsigned char           BYTE;
typedef unsigned short          WORD;
typedef unsigned int            DWORD;
typedef unsigned long long      QWORD;

typedef unsigned char           UINT8;
typedef unsigned short          UINT16;
typedef unsigned int            UINT32;
typedef unsigned long long      UINT64;

typedef char                    BOOLEAN;
#define FALSE                   (0)
#define TRUE                    (!FALSE)

#define CHR_MIN       (-128)                            // maximum (signed) char value
#define CHR_MAX       (127)                             // maximum (signed) char value
#define UCHR_MAX      (0xff)                            // minimum unsigned char value
#define SHRT_MIN      (-32768)                          // minimum (signed) short value
#define SHRT_MAX      (32767)                           // maximum (signed) short value
#define USHRT_MAX     (0xffff)                          // maximum unsigned short value
#define INT_MIN       (-2147483647 - 1)                 // minimum (signed) int value
#define INT_MAX       (2147483647)                      // maximum (signed) int value
#define UINT_MAX      (0xffffffff)                      // maximum unsigned int value
#define LONG_MIN      (-2147483647L - 1)                // minimum (signed) long value
#define LONG_MAX      (2147483647L)                     // maximum (signed) long value
#define ULONG_MAX     (0xffffffffUL)                    // maximum unsigned long value
#define LLONG_MAX     (9223372036854775807LL)           // maximum (signed) long long int value
#define LLONG_MIN     (-9223372036854775807LL - 1)      // minimum (signed) long long int value
#define ULLONG_MAX    (0xffffffffffffffffULL)           // maximum unsigned long long int value


#define BYTE_MAX    UCHR_MAX
#define WORD_MAX    USHRT_MAX
#define DWORD_MAX   LONG_MAX
#define QWORD_MAX   ULLONG_MAX

#define UINT8_MAX   BYTE_MAX
#define UINT16_MAX  WORD_MAX
#define UINT32_MAX  DWORD_MAX 
#define UINT64_MAX  QWORD_MAX

#define INT8_MAX    CHR_MAX
#define INT8_MIN    CHR_MIN

#define INT16_MAX   SHRT_MAX
#define INT16_MIN   SHRT_MIN

#define INT32_MAX   LONG_MAX
#define INT32_MIN   LONG_MIN

#define INT64_MAX   LONG_MAX
#define INT64_MIN   LONG_MIN

#ifndef NULL
#define NULL 0
#endif

#endif //__DEFINITIONS_H__