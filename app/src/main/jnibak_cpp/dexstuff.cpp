//
// Created by moslab on 4/19/16.
//

#include "dexstuff.h"
#include <dlfcn.h>
#include <android/log.h>

static void* mydlsym(void *hand, const char *name)
{
    void* ret = dlsym(hand, name);
    //log("%s = 0x%x\n", name, ret)
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "%s = 0x%x\n", name, ret);
    return ret;
}

//get all dvm func address
void dexstuff_resolv_dvm(struct dexstuff_t *d)
{
    d->dvm_hand = dlopen("libdvm.so", RTLD_NOW);

    if (d->dvm_hand) {

        d->dvm_dalvik_system_DexFile = (DalvikNativeMethod*) mydlsym(d->dvm_hand, "dvm_dalvik_system_DexFile");
        d->dvm_java_lang_Class = (DalvikNativeMethod*) mydlsym(d->dvm_hand, "dvm_java_lang_Class");
        /*
        *(void**)(&d->dvmThreadSelf_fnPtr) = mydlsym(d->dvm_hand, "_Z13dvmThreadSelfv");
        if (!d->dvmThreadSelf_fnPtr)
            *(void**)(&d->dvmThreadSelf_fnPtr) = mydlsym(d->dvm_hand, "dvmThreadSelf");

        *(void**)(&d->dvmStringFromCStr_fnPtr) = mydlsym(d->dvm_hand, "_Z32dvmCreateStringFromCstrAndLengthPKcj");
        *(void**)(&d->dvmGetSystemClassLoader_fnPtr) = mydlsym(d->dvm_hand, "_Z23dvmGetSystemClassLoaderv");
        *(void**)(&d->dvmIsClassInitialized_fnPtr) = mydlsym(d->dvm_hand, "_Z21dvmIsClassInitializedPK11ClassObject");
        *(void**)(&d->dvmInitClass_fnPtr) = mydlsym(d->dvm_hand, "dvmInitClass");

        *(void**)(&d->dvmFindVirtualMethodHierByDescriptor_fnPtr) = mydlsym(d->dvm_hand, "_Z36dvmFindVirtualMethodHierByDescriptorPK11ClassObjectPKcS3_");
        if (!d->dvmFindVirtualMethodHierByDescriptor_fnPtr)
            *(void**)(&d->dvmFindVirtualMethodHierByDescriptor_fnPtr) = mydlsym(d->dvm_hand, "dvmFindVirtualMethodHierByDescriptor");

        *(void**)(&d->dvmFindDirectMethodByDescriptor_fnPtr) = mydlsym(d->dvm_hand, "_Z31dvmFindDirectMethodByDescriptorPK11ClassObjectPKcS3_");
        if (!d->dvmFindDirectMethodByDescriptor_fnPtr)
            *(void**)(&d->dvmFindDirectMethodByDescriptor_fnPtr) = mydlsym(d->dvm_hand, "dvmFindDirectMethodByDescriptor");

        *(void**)(&d->dvmIsStaticMethod_fnPtr) = mydlsym(d->dvm_hand, "_Z17dvmIsStaticMethodPK6Method");
        *(void**)(&d->dvmAllocObject_fnPtr) = mydlsym(d->dvm_hand, "dvmAllocObject");
        *(void**)(&d->dvmCallMethodV_fnPtr) = mydlsym(d->dvm_hand, "_Z14dvmCallMethodVP6ThreadPK6MethodP6ObjectbP6JValueSt9__va_list");
        *(void**)(&d->dvmCallMethodA_fnPtr) = mydlsym(d->dvm_hand, "_Z14dvmCallMethodAP6ThreadPK6MethodP6ObjectbP6JValuePK6jvalue");
        *(void**)(&d->dvmAddToReferenceTable_fnPtr) = mydlsym(d->dvm_hand, "_Z22dvmAddToReferenceTableP14ReferenceTableP6Object");

        *(void**)(&d->dvmSetNativeFunc_fnPtr) = mydlsym(d->dvm_hand, "_Z16dvmSetNativeFuncP6MethodPFvPKjP6JValuePKS_P6ThreadEPKt");*/

        void *dvmUseJNIBridge_ptr = mydlsym(d->dvm_hand, "_Z15dvmUseJNIBridgeP6MethodPv");
        d->dvmUseJNIBridge_fnPtr = reinterpret_cast<dvmUseJNIBridge_func>(reinterpret_cast<long>(dvmUseJNIBridge_ptr));

       // *(void**)(&d->dvmUseJNIBridge_fnPtr) = mydlsym(d->dvm_hand, "_Z15dvmUseJNIBridgeP6MethodPv");
        if (!d->dvmUseJNIBridge_fnPtr) {
            *(void **) (&d->dvmUseJNIBridge_fnPtr) = mydlsym(d->dvm_hand, "dvmUseJNIBridge");
            dvmUseJNIBridge_ptr = mydlsym(d->dvm_hand, "dvmUseJNIBridge");
            d->dvmUseJNIBridge_fnPtr = reinterpret_cast<dvmUseJNIBridge_func>(reinterpret_cast<long>(dvmUseJNIBridge_ptr));
        }
        void *dvmIsStaticMethod_ptr = mydlsym(d->dvm_hand, "_Z17dvmIsStaticMethodPK6Method");
        //*(void**)(&d->dvmIsStaticMethod_fnPtr) = mydlsym(d->dvm_hand, "_Z17dvmIsStaticMethodPK6Method");
        d->dvmIsStaticMethod_fnPtr =reinterpret_cast<dvmIsStaticMethod_func>(reinterpret_cast<long>(dvmIsStaticMethod_ptr));

/*
        *(void**)(&d->dvmDecodeIndirectRef_fnPtr) =  mydlsym(d->dvm_hand, "_Z20dvmDecodeIndirectRefP6ThreadP8_jobject");

        *(void**)(&d->dvmLinearSetReadWrite_fnPtr) = mydlsym(d->dvm_hand, "_Z21dvmLinearSetReadWriteP6ObjectPv");

        *(void**)(&d->dvmGetCurrentJNIMethod_fnPtr) = mydlsym(d->dvm_hand, "_Z22dvmGetCurrentJNIMethodv");

        *(void**)(&d->dvmFindInstanceField_fnPtr) = mydlsym(d->dvm_hand, "_Z20dvmFindInstanceFieldPK11ClassObjectPKcS3_");

        //d->dvmCallJNIMethod_fnPtr = mydlsym(d->dvm_hand, "_Z21dvmCheckCallJNIMethodPKjP6JValuePK6MethodP6Thread");
        *(void**)(&d->dvmCallJNIMethod_fnPtr) = mydlsym(d->dvm_hand, "_Z16dvmCallJNIMethodPKjP6JValuePK6MethodP6Thread");

        *(void**)(&d->dvmDumpAllClasses_fnPtr) = mydlsym(d->dvm_hand, "_Z17dvmDumpAllClassesi");
        *(void**)(&d->dvmDumpClass_fnPtr) = mydlsym(d->dvm_hand, "_Z12dvmDumpClassPK11ClassObjecti");

        *(void**)(&d->dvmFindLoadedClass_fnPtr) = mydlsym(d->dvm_hand, "_Z18dvmFindLoadedClassPKc");
        if (!d->dvmFindLoadedClass_fnPtr)
            *(void**)(&d->dvmFindLoadedClass_fnPtr) = mydlsym(d->dvm_hand, "dvmFindLoadedClass");

        *(void**)(&d->dvmHashTableLock_fnPtr) = mydlsym(d->dvm_hand, "_Z16dvmHashTableLockP9HashTable");
        *(void**)(&d->dvmHashTableUnlock_fnPtr) = mydlsym(d->dvm_hand, "_Z18dvmHashTableUnlockP9HashTable");
        *(void**)(&d->dvmHashForeach_fnPtr) = mydlsym(d->dvm_hand, "_Z14dvmHashForeachP9HashTablePFiPvS1_ES1_");

        *(void**)(&d->dvmInstanceof_fnPtr) = mydlsym(d->dvm_hand, "_Z13dvmInstanceofPK11ClassObjectS1_");
*/
        d->gDvm = mydlsym(d->dvm_hand, "gDvm");
    }
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "dvm_hand = 0x%x\n", d->dvm_hand);
}