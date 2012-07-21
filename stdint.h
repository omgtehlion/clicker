#ifndef __TARGET_STDINT_H
#define __TARGET_STDINT_H

// C18 compiler types

typedef signed char             int8_t;
typedef signed char             int_fast8_t;
typedef unsigned char           uint8_t;
typedef unsigned char           uint_fast8_t;
typedef short int               int16_t;
typedef short int               int_fast16_t;
typedef unsigned short int      uint16_t;
typedef unsigned short int      uint_fast16_t;
typedef long int                int32_t;
typedef long int                int_fast32_t;
typedef unsigned long int       uint32_t;
typedef unsigned long int       uint_fast32_t;
typedef long long int           int64_t;
typedef long long int           int_fast64_t;
typedef unsigned long long int  uint64_t;
typedef unsigned long long int  uint_fast64_t;

typedef unsigned int            size_t; // c++ ::new(size_t)
typedef int                     ssize_t;

#endif // __TARGET_STDINT_H
