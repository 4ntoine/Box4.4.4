//
// Created by xin on 4/1/16.
//

#ifndef BOXIFY_DVMDEX_H
#define BOXIFY_DVMDEX_H

#include "DexFile.h"
#include "SysUtil.h"
#include "jni.h"
#include <pthread.h>
struct ClassObject;
struct Method;
struct StringObject;
struct InstField;
/*
 * Some additional VM data structures that are associated with the DEX file.
 */
struct DvmDex {
    /* pointer to the DexFile we're associated with */
    DexFile*            pDexFile;

    /* clone of pDexFile->pHeader (it's used frequently enough) */
    const DexHeader*    pHeader;

    /* interned strings; parallel to "stringIds" */
    struct StringObject** pResStrings;

    /* resolved classes; parallel to "typeIds" */
    struct ClassObject** pResClasses;

    /* resolved methods; parallel to "methodIds" */
    struct Method**     pResMethods;

    /* resolved instance fields; parallel to "fieldIds" */
    /* (this holds both InstField and StaticField) */
    struct Field**      pResFields;

    /* interface method lookup cache */
    struct AtomicCache* pInterfaceCache;

    /* shared memory region with file contents */
    bool                isMappedReadOnly;
    MemMapping          memMap;

    jobject dex_object; //ZX:NOTE

    /* lock ensuring mutual exclusion during updates */
    pthread_mutex_t     modLock;
};
#endif //BOXIFY_DVMDEX_H
