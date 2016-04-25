//
// Created by moslab on 4/19/16.
//

#ifndef BOX4_4_4_DEXSTUFF_H
#define BOX4_4_4_DEXSTUFF_H

#include <jni.h>
#include "common.h"
#include "object.h"
typedef void* (*dvmCreateStringFromCstr_func)(const char* utf8Str, int len, int allocFlags);
typedef void* (*dvmGetSystemClassLoader_func)(void);
typedef void* (*dvmThreadSelf_func)(void);

typedef void* (*dvmIsClassInitialized_func)(void*);
typedef void* (*dvmInitClass_func)(void*);
typedef void* (*dvmFindVirtualMethodHierByDescriptor_func)(void*,const char*, const char*);
typedef void* (*dvmFindDirectMethodByDescriptor_func)(void*,const char*, const char*);
typedef void* (*dvmIsStaticMethod_func)(void*);
typedef void* (*dvmAllocObject_func)(void*, unsigned int);
typedef void* (*dvmCallMethodV_func)(void*,void*,void*,void*,va_list);
typedef void* (*dvmCallMethodA_func)(void*,void*,void*,bool,void*,jvalue*);
typedef void* (*dvmAddToReferenceTable_func)(void*,void*);
typedef void (*dvmDumpAllClasses_func)(int);
typedef void* (*dvmFindLoadedClass_func)(const char*);

typedef void (*dvmUseJNIBridge_func)(void*, void*);

typedef void* (*dvmDecodeIndirectRef_func)(void*,void*);

typedef void* (*dvmGetCurrentJNIMethod_func)();

typedef void (*dvmLinearSetReadWrite_func)(void*,void*);

typedef void* (*dvmFindInstanceField_func)(void*,const char*,const char*);

typedef void* (*dvmSetNativeFunc_func)(void*,void*, void*);
typedef void (*dvmCallJNIMethod_func)(const u4*, void*, void*, void*);

typedef void (*dvmHashTableLock_func)(void*);
typedef void (*dvmHashTableUnlock_func)(void*);
typedef void (*dvmHashForeach_func)(void*,void*,void*);

typedef void (*dvmDumpClass_func)(void*,void*);

typedef int (*dvmInstanceof_func)(void*,void*);


struct DalvikNativeMethod {
    const char* name;
    const char* signature;
    DalvikNativeFunc  fnPtr;
};

struct dexstuff_t
{
    void *dvm_hand;

    dvmCreateStringFromCstr_func dvmStringFromCStr_fnPtr;
    dvmGetSystemClassLoader_func dvmGetSystemClassLoader_fnPtr;
    dvmThreadSelf_func dvmThreadSelf_fnPtr;

    dvmIsClassInitialized_func dvmIsClassInitialized_fnPtr;
    dvmInitClass_func dvmInitClass_fnPtr;
    dvmFindVirtualMethodHierByDescriptor_func dvmFindVirtualMethodHierByDescriptor_fnPtr;
    dvmFindDirectMethodByDescriptor_func dvmFindDirectMethodByDescriptor_fnPtr;
    dvmIsStaticMethod_func dvmIsStaticMethod_fnPtr;
    dvmAllocObject_func dvmAllocObject_fnPtr;
    dvmCallMethodV_func dvmCallMethodV_fnPtr;
    dvmCallMethodA_func dvmCallMethodA_fnPtr;
    dvmAddToReferenceTable_func dvmAddToReferenceTable_fnPtr;
    dvmDecodeIndirectRef_func dvmDecodeIndirectRef_fnPtr;
    dvmUseJNIBridge_func dvmUseJNIBridge_fnPtr;
    dvmFindInstanceField_func dvmFindInstanceField_fnPtr;
    dvmFindLoadedClass_func dvmFindLoadedClass_fnPtr;
    dvmDumpAllClasses_func dvmDumpAllClasses_fnPtr;

    dvmGetCurrentJNIMethod_func dvmGetCurrentJNIMethod_fnPtr;
    dvmLinearSetReadWrite_func dvmLinearSetReadWrite_fnPtr;

    dvmSetNativeFunc_func dvmSetNativeFunc_fnPtr;
    dvmCallJNIMethod_func dvmCallJNIMethod_fnPtr;

    dvmHashTableLock_func dvmHashTableLock_fnPtr;
    dvmHashTableUnlock_func dvmHashTableUnlock_fnPtr;
    dvmHashForeach_func dvmHashForeach_fnPtr;

    dvmDumpClass_func dvmDumpClass_fnPtr;
    dvmInstanceof_func dvmInstanceof_fnPtr;

    DalvikNativeMethod *dvm_dalvik_system_DexFile;
    DalvikNativeMethod *dvm_java_lang_Class;

    void *gDvm; // dvm globals !

    int done;
};
void dexstuff_resolv_dvm(struct dexstuff_t *d);
#endif //BOX4_4_4_DEXSTUFF_H
