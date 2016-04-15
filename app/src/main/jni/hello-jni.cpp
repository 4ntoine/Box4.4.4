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
/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
#define LOG_TAG "TEST"
extern "C"{
jstring
        Java_com_example_mono_box444_MainActivity_stringFromJNI(JNIEnv *env,
                                                                jobject thiz);
jlong
        Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread);

void Java_com_example_mono_box444_MainActivity_SetOriginal(JNIEnv *env, jobject thiz, jlong handle);
};

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
    return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");

}
//May cache those Reference in the future
jlong Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread) {

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
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", hookOrig->name);

    //check pointer
    if(hookOrig->insns == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Test", "hook insns null");
    if(stubOrig->insns == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Test", "stub insns null");

    //Overwrite the inst reference
    const u2* origInst = hookOrig->insns;
    hookOrig->insns = stubOrig->insns;


   //Find BindApplication
    //jclass clsBindApp = env->FindClass("android/app/ActivityThread$ApplicationThread");
    jclass clsBindApp = env->GetObjectClass(appThread);
    jmethodID bAppId = env->GetMethodID(clsBindApp, "fakeBindApp", "(Ljava/lang/String;"
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
    const u2* origIn = mSch->insns;
    mSch->insns = stub2Orig->insns;

    jmethodID targetID = env->GetMethodID(clsSch, "target", "()V");
    Method *mTarget = (Method *)targetID;
    const u2* tarIn = mTarget->insns;
    if(mTarget->insns == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Test", "insns null");

    //mTarget->insns = stubOrig->insns;
    mSch->insns = stub2Orig->insns;
    mSch->registersSize = stub2Orig->registersSize;
    mSch->outsSize = stub2Orig->outsSize;
    mSch->insSize = stub2Orig->insSize;
    //mSch->accessFlags = stub2Orig->accessFlags;
    //mSch->shorty = stub2Orig->shorty;
   // mTarget->jniArgInfo = stub2Orig->jniArgInfo;
   // mTarget->prototype = stub2Orig->prototype;
   // mTarget->nativeFunc = stub2Orig->nativeFunc;
   // mTarget->fastJni = stub2Orig->fastJni;
    //mTarget->noRef = stub2Orig->noRef;
    //mTarget->shouldTrace = stub2Orig->shouldTrace;
    //mSch->registerMap = stub2Orig->registerMap;
    //mTarget->inProfile = stub2Orig->inProfile;
    mSch->clazz = stub2Orig->clazz;
    //mTarget->name = stub2Orig->name;
    //mTarget->methodIndex = stub2Orig->methodIndex;


    return (jlong)origInst;
}

void Java_com_example_mono_box444_MainActivity_SetOriginal(JNIEnv *env, jobject thiz, jlong handle)
{
    jclass clsHook = env->FindClass("com/example/mono/box444/MainActivity");
    jmethodID hookID = env->GetMethodID(clsHook, "hookMe", "(Ljava/lang/String;)V");
    Method *hookOrig = (Method *)hookID;
    jstring strHook = env->NewStringUTF(hookOrig->name);
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", hookOrig->name);
    hookOrig->insns = (u2*)handle;
}
