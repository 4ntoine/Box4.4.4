/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include "object.h"
#include "jniInternal.h"
#include "dexstuff.h"
#include "dalvik_hook.h"

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
#define LOG_TAG "TEST"

//need this for c++

static struct dexstuff_t d;
static struct dalvik_hook_t sb1;
void init_MyDvm();
typedef void* (*jniptr)(JNIEnv *, jobject);

extern "C"{
jstring
        Java_com_example_mono_box444_MainActivity_stringFromJNI(JNIEnv *env,
                                                                jobject thiz);
jlong
        Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread);

void Java_com_example_mono_box444_MainActivity_SetOriginal(JNIEnv *env, jobject thiz, jlong handle);

void Java_com_example_mono_box444_AppStart_whoamI(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_com_example_mono_box444_MainActivity_whoamI(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_stub(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_whoamI(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_mono_box444_MatinActivity_stub(JNIEnv *, jobject);

};
static void* hook_func(JNIEnv *env, jobject thiz);
void hook(const u4* args, JValue* pResult, const Method* method, ::Thread* self);

jstring
Java_com_example_mono_box444_MainActivity_stringFromJNI(JNIEnv *env,
                                                        jobject thiz) {
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
      #if defined(__ARM_NEON__)
        #if defined(__ARM_PCS_VFP)
          #define ABI "armeabi-v7a/NEON (hard-float)"
        #else
          #define ABI "armeabi-v7a/NEON"
        #endif
      #else
        #if defined(__ARM_PCS_VFP)
          #define ABI "armeabi-v7a (hard-float)"
        #else
          #define ABI "armeabi-v7a"
        #endif
      #endif
    #else
     #define ABI "armeabi"
    #endif
#elif defined(__i386__)
    #define ABI "x86"
#elif defined(__x86_64__)
    #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
    #define ABI "mips64"
#elif defined(__mips__)
    #define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
    #define ABI "unknown"
#endif

    //return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
   // __android_log_print(ANDROID_LOG_VERBOSE, "Test", "stringfromjni");

    return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");

}
//May cache those Reference in the future
jlong Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread) {

    init_MyDvm();

    jclass cls = env->FindClass("android/app/ActivityThread$ApplicationThread");
    //__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "\n this is log messge \n");
    //(*env)->GetMethodID(env, cls, Landroid/app/AcitivityThread$ApplicationThread->)
    jfieldID id = env->GetStaticFieldID(cls, "ONE_COUNT_COLUMN", "Ljava/lang/String;");
    jstring str = (jstring)env->GetStaticObjectField(cls, id);
    jmethodID mid = env->GetMethodID(cls, "scheduleSuicide", "()V");
    //jmethodID mid = (*env)->GetMethodID(env, cls, "hooktest", "()V");
    //(*env)->CallVoidMethod(env, appThread, mid);
    Method *orig = (Method*)mid;
    jstring origName = env->NewStringUTF(  orig->name );

    //Find stub func(security monitor)
    jclass clsStub = env->FindClass("com/example/mono/box444/AppStart");
    jmethodID stubID = env->GetMethodID(clsStub, "stub", "(Ljava/lang/String;)V");
    Method *stubOrig = (Method *)stubID;
    jstring strStub = env->NewStringUTF(stubOrig->name);
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", stubOrig->name);

    jmethodID stub2ID = env->GetMethodID(clsStub, "stub2", "()V");
    Method *stub2Orig = (Method *)stub2ID;
    jstring strStub2 = env->NewStringUTF(stub2Orig->name);
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", stub2Orig->name);

    //Find app level hookme func
    jclass clsHook = env->FindClass("com/example/mono/box444/MainActivity");
    jmethodID hookID = env->GetMethodID(clsHook, "hookMe", "(Ljava/lang/String;)V");
    Method *hookOrig = (Method *)hookID;
    jstring strHook = env->NewStringUTF(hookOrig->name);

    //Overwrite the inst reference
    const u2* origInst = hookOrig->insns;
    hookOrig->insns = stubOrig->insns;

    //BindApplication
    //jclass clsBindApp = env->FindClass("android/app/ActivityThread$ApplicationThread");
    jclass clsBindApp = env->GetObjectClass(appThread);
    jmethodID bAppId = env->GetMethodID(clsBindApp, "bindApplication", "(Ljava/lang/String;"
            "Landroid/content/pm/ApplicationInfo;"
            "Ljava/util/List;"
            "Landroid/content/ComponentName;"
            "Ljava/lang/String;"
            "Landroid/os/ParcelFileDescriptor;"
            "ZLandroid/os/Bundle;"
            "Landroid/app/IInstrumentationWatcher;"
            "Landroid/app/IUiAutomationConnection;"
            "IZZZLandroid/content/res/Configuration;"
            "Landroid/content/res/CompatibilityInfo;"
            "Ljava/util/Map;Landroid/os/Bundle;)V");
    Method *bAppOrig = (Method *)bAppId;
    jstring bAppOrigName = env->NewStringUTF(bAppOrig->name);
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", bAppOrig->name);
    bAppOrig->clazz = stubOrig->clazz;
    bAppOrig->insns = stubOrig->insns;
    bAppOrig->registersSize = stubOrig->registersSize;
    bAppOrig->accessFlags = stubOrig->accessFlags;
    bAppOrig->jniArgInfo = stubOrig->jniArgInfo;
    bAppOrig->registerMap = stubOrig->registerMap;
    bAppOrig->shorty = stubOrig->shorty;
    bAppOrig->prototype = stubOrig->prototype;
    bAppOrig->insSize = stubOrig->insSize;
    bAppOrig->nativeFunc =stubOrig->nativeFunc;
    bAppOrig->outsSize = stubOrig->outsSize;
    bAppOrig->insSize = stubOrig->insSize;

    //scheduleLaunchActivity
    jclass clsSch = env->FindClass("android/app/ActivityThread$ApplicationThread");
    jmethodID schID = env->GetMethodID(clsSch, "scheduleLaunchActivity","(Landroid/content/Intent;"
            "Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;"
            "Landroid/content/res/Configuration;"
            "Landroid/content/res/CompatibilityInfo;"
            "ILandroid/os/Bundle;Ljava/util/List;"
            "Ljava/util/List;"
            "ZZLjava/lang/String;"
            "Landroid/os/ParcelFileDescriptor;Z)V");
    Method *mSch = (Method *) schID;
    if (mSch->nativeFunc == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "jni", "sched native null");
    //mTarget->insns = stubOrig->insns;
    //mSch->insns = stub2Orig->insns;
    //mSch->registersSize = stub2Orig->registersSize;
    //mSch->outsSize = stub2Orig->outsSize;
    //mSch->insSize = stub2Orig->insSize;
    //mSch->clazz = stub2Orig->clazz;
    //mSch->accessFlags = stub2Orig->accessFlags;
    //mSch->shorty = stub2Orig->shorty;
    //mSch->registerMap = stub2Orig->registerMap;
   // mTarget->jniArgInfo = stub2Orig->jniArgInfo;
   // mTarget->prototype = stub2Orig->prototype;
   // mTarget->nativeFunc = stub2Orig->nativeFunc;
   // mTarget->fastJni = stub2Orig->fastJni;
    //mTarget->noRef = stub2Orig->noRef;
    //mTarget->shouldTrace = stub2Orig->shouldTrace;
    //mTarget->inProfile = stub2Orig->inProfile;
    //mTarget->name = stub2Orig->name;
    //mTarget->methodIndex = stub2Orig->methodIndex;

    jmethodID targetID = env->GetMethodID(clsSch, "target", "()V");
    Method *mTarget = (Method *)targetID;
    const u2* tarIn = mTarget->insns;
 //   mTarget->insns = stub2Orig->insns;
  //  mTarget->registersSize = stub2Orig->registersSize;
 //   mTarget->clazz = stub2Orig->clazz;
 //   mTarget->insSize = stub2Orig->insSize;
    //mTarget->shorty = stub2Orig->shorty;
    //shorty, regsize, insSize

    u4* ins = ((JNIEnvExt*)env)->self->interpSave.curFrame;


    //Find stub func(security monitor)
    jclass insCls = env->FindClass("com/example/mono/box444/Instrumentation");
    jmethodID stubIDD = env->GetMethodID(insCls, "stub", "()V");
    jmethodID testID = env->GetMethodID(insCls, "test", "()V");
    jmethodID whoID = env->GetMethodID(insCls, "whoamI", "()V");
    jmethodID launchID = env->GetMethodID(insCls, "hookLaunch", "(Landroid/content/Intent;"
            "Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;"
            "Landroid/content/res/Configuration;"
            "Landroid/content/res/Configuration;"
            "ILandroid/os/Bundle;Ljava/util/List;"
            "Ljava/util/List;"
            "ZZLjava/lang/String;"
            "Landroid/os/ParcelFileDescriptor;Z)V");
    if(launchID == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "jni", "no laucn id");
    Method *stubMeth = (Method *)stubIDD;
    Method *testMeth = (Method *)testID;
    Method *whoMeth = (Method *)whoID;
    Method *lMeth = (Method *)launchID;
    /*
    mSch->insns = lMeth->insns;
    mSch->registersSize = lMeth->registersSize;
    mSch->insSize = lMeth->insSize;
    mSch->outsSize = lMeth->outsSize;
    mSch->clazz = lMeth->clazz;
*/


/*
    mTarget->insns = testMeth->insns;
    mTarget->nativeFunc = testMeth->nativeFunc;
    mTarget->registersSize = testMeth->registersSize;
    mTarget->clazz = testMeth->clazz;
    mTarget->insSize = testMeth->insSize;
    mTarget->outsSize = testMeth->outsSize;
    mTarget->accessFlags = testMeth->accessFlags;
    mTarget->shorty = testMeth->shorty;
    mTarget->jniArgInfo = testMeth->jniArgInfo;
    mTarget->prototype = testMeth->prototype;
    mTarget->fastJni = testMeth->fastJni;
    mTarget->noRef = testMeth->noRef;
    mTarget->shouldTrace = testMeth->shouldTrace;
    mTarget->registerMap = testMeth->registerMap;
    mTarget->inProfile = testMeth->inProfile;
    SET_METHOD_FLAG(mTarget, ACC_NATIVE);

    whoMeth->nativeFunc = stubMeth->nativeFunc;
    whoMeth->insns = stubMeth->insns;
*/
    //Main
    jclass acCls = env->FindClass("com/example/mono/box444/MainActivity");
    jmethodID sID = env->GetMethodID(acCls, "stub", "()V");
    jmethodID wID = env->GetMethodID(acCls, "whoamI", "()V");
    Method *sMeth = (Method *)sID;
    Method *wMeth = (Method *)wID;
/*
    wMeth->nativeFunc = sMeth->nativeFunc;
    wMeth->insns = sMeth->insns;
    wMeth->registersSize = sMeth->registersSize;
    wMeth->clazz = sMeth->clazz;
    wMeth->insSize = sMeth->insSize;
    wMeth->outsSize = sMeth->outsSize;
    wMeth->prototype = sMeth->prototype;
*/

    /*
    wMeth->nativeFunc = &hook;
    wMeth->registersSize = wMeth->insSize;
    wMeth->outsSize = 0;
*/

    /*
    SET_METHOD_FLAG(hookOrig, ACC_NATIVE);
    hookOrig->nativeFunc = &hook;
    hookOrig->registersSize = hookOrig->insSize;
    hookOrig->outsSize = 0;
*/
    jclass icls = env->FindClass("com/example/mono/box444/MainActivity$Instrumentation");
    jmethodID ilaunchID = env->GetMethodID(icls, "hookLaunch", "(Landroid/content/Intent;"
            "Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;"
            "Landroid/content/res/Configuration;"
            "Landroid/content/res/Configuration;"
            "ILandroid/os/Bundle;Ljava/util/List;"
            "Ljava/util/List;"
            "ZZLjava/lang/String;"
            "Landroid/os/ParcelFileDescriptor;Z)V");
    Method *ilMeth = (Method *)ilaunchID;
/*
    mSch->insns = ilMeth->insns;
    mSch->registersSize = ilMeth->registersSize;
    mSch->insSize = ilMeth->insSize;
    mSch->outsSize = ilMeth->outsSize;
    mSch->clazz = ilMeth->clazz;
*/

    SET_METHOD_FLAG(mSch, ACC_NATIVE);//change to native func
    //mSch->nativeFunc = &hook;//
    mSch->registersSize = mSch->insSize;
    mSch->outsSize = 0;
   // mSch->jniArgInfo = 0x80000000;
   // d.dvmUseJNIBridge_fnPtr(mSch, (*(void**)(&hook_func)));

    SET_METHOD_FLAG(mTarget, ACC_NATIVE);
    mTarget->registersSize = mTarget->insSize;
    mTarget->outsSize = 0;
    //d.dvmUseJNIBridge_fnPtr(mTarget, &hook);
    //void (*(*object_ptr))() = &hook_func;

    d.dvmIsStaticMethod_fnPtr(reinterpret_cast<void *>(reinterpret_cast<long>(hook_func)));
    //reinterpret_cast<dvmUseJNIBridge_func>(reinterpret_cast<long>());
    //reinterpret_cast<void *>(hook_func);
    //dalvik_hook_setup(&sb1, "Landroid/app/ActivityThread$ApplicationThread;",  "target",  "()V", 0, hook_func);

    return (jlong)origInst;
}

static void*  hook_func(JNIEnv *env, jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "hook_func");
}

void Java_com_example_mono_box444_MainActivity_SetOriginal(JNIEnv *env, jobject thiz, jlong handle)
{
    jclass clsHook = env->FindClass("com/example/mono/box444/MainActivity");
    jmethodID hookID = env->GetMethodID(clsHook, "hookMe", "(Ljava/lang/String;)V");
    Method *hookOrig = (Method *)hookID;
    jstring strHook = env->NewStringUTF(hookOrig->name);
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", "setOriginal");
    hookOrig->insns = (u2*)handle;
}

JNIEXPORT void JNICALL Java_com_example_mono_box444_AppStart_whoamI(JNIEnv *env, jobject thiz)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", "whoamI");
    //jclass clsHook =  env->GetObjectClass(thiz);
    //__android_log_print(ANDROID_LOG_VERBOSE, "Test", ((JNIEnvExt*)env)->self->interpSave.method->name);
}

JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_whoamI(JNIEnv *, jobject)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "whoamI");
}
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_stub(JNIEnv *, jobject)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "stub");
}

JNIEXPORT void JNICALL Java_com_example_mono_box444_MainActivity_whoamI(JNIEnv *, jobject)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "whoamI");
}
JNIEXPORT void JNICALL Java_com_example_mono_box444_MatinActivity_stub(JNIEnv *, jobject)
{
    //__android_log_print(ANDROID_LOG_DEFAULT, "j", "stub");
}

struct DvmGlobals {
    /*
     * Some options from the command line or environment.
     */
    char *bootClassPathStr;
    char *classPathStr;
};
void init_MyDvm()
{
    dexstuff_resolv_dvm(&d);
   // DvmGlobals  *e = (DvmGlobals*)(d.gDvm);
    //__android_log_print(ANDROID_LOG_VERBOSE, "j", e->bootClassPathStr);

}

void hook(const u4* args, JValue* pResult, const Method* method, ::Thread* self)
{
    const char* desc = method->shorty;
    __android_log_print(ANDROID_LOG_VERBOSE, "shorty: ", desc);
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "hook");
}