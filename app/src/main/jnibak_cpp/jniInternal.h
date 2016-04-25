//
// Created by moslab on 4/15/16.
//

#ifndef BOX4_4_4_JNIINTERNAL_H
#define BOX4_4_4_JNIINTERNAL_H

#include "thread.h"

struct JNIEnvExt {
    const struct JNINativeInterface* funcTable;     /* must be first */

    const struct JNINativeInterface* baseFuncTable;

    u4      envThreadId;
    Thread* self;

    /* if nonzero, we are in a "critical" JNI call */
    int     critical;

    struct JNIEnvExt* prev;
    struct JNIEnvExt* next;
};

#endif //BOX4_4_4_JNIINTERNAL_H
