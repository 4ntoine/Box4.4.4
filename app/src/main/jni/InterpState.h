//
// Created by moslab on 4/15/16.
//

#ifndef BOX4_4_4_INTERPSTATE_H
#define BOX4_4_4_INTERPSTATE_H

#include "common.h"
#include "object.h"

struct InterpSaveState {
    const u2*       pc;         // Dalvik PC
    u4*             curFrame;   // Dalvik frame pointer
    const Method    *method;    // Method being executed
    DvmDex*         methodClassDex;
    JValue          retval;
    void*           bailPtr;
#if defined(WITH_TRACKREF_CHECKS)
    int             debugTrackedRefStart;
#else
    int             unused;        // Keep struct size constant
#endif
    struct InterpSaveState* prev;  // To follow nested activations
} __attribute__ ((__packed__));

#endif //BOX4_4_4_INTERPSTATE_H
