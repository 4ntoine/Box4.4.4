//
// Created by Mono on 4/1/16.
//

#ifndef BOX_SYSUTIL_H
#define BOX_SYSUTIL_H
/*
 * Use this to keep track of mapped segments.
 */
#include <sys/types.h>
struct MemMapping {
    void*   addr;           /* start of data */
    size_t  length;         /* length of data */

    void*   baseAddr;       /* page-aligned base address */
    size_t  baseLength;     /* length of mapping */
};
#endif //BOX_SYSUTIL_H
