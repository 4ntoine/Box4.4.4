//
// Created by xin on 4/1/16.
//

#ifndef BOXIFY_COMMON_H
#define BOXIFY_COMMON_H
#include <stdint.h>
#include <stdbool.h>
typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;


//ZX: choose one here?
union JValue {
#if defined(HAVE_LITTLE_ENDIAN)
    u1      z;
    s1      b;
    u2      c;
    s2      s;
    s4      i;
    s8      j;
    float   f;
    double  d;
    Object* l;
#endif
#if defined(HAVE_BIG_ENDIAN)
    struct {
        u1    _z[3];
        u1    z;
    };
    struct {
        s1    _b[3];
        s1    b;
    };
    struct {
        u2    _c;
        u2    c;
    };
    struct {
        s2    _s;
        s2    s;
    };
    s4      i;
    s8      j;
    float   f;
    double  d;
    void*   l;
#endif
};

#endif //BOXIFY_COMMON_H
