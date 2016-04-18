//
// Created by moslab on 4/15/16.
//

#ifndef BOX4_4_4_THREAD_H
#define BOX4_4_4_THREAD_H

#include "InterpState.h"

struct Thread {
    /*
     * Interpreter state which must be preserved across nested
     * interpreter invocations (via JNI callbacks).  Must be the first
     * element in Thread.
     */
    InterpSaveState interpSave;
};

#endif //BOX4_4_4_THREAD_H
