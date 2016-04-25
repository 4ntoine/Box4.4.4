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
//#include "object.h"
//#include "jniInternal.h"
#include "dexstuff.h"
#include "dalvik_hook.h"

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
#define LOG_TAG "TEST"


static struct dexstuff_t d;
static struct dalvik_hook_t sb1;
static struct dalvik_hook_t sb2;
static struct dalvik_hook_t sb3;
void init_MyDvm();
typedef void* (*jniptr)(JNIEnv *, jobject);
static jobject main_thread;
static jobject new_intent;
static jclass mainCls;

jstring Java_com_example_mono_box444_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz);
jlong Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread, jobject intent);
void Java_com_example_mono_box444_MainActivity_SetOriginal(JNIEnv *env, jobject thiz, jlong handle);
void Java_com_example_mono_box444_AppStart_whoamI(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_com_example_mono_box444_MainActivity_whoamI(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_stub(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_whoamI(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_mono_box444_MatinActivity_stub(JNIEnv *, jobject);

typedef struct _nativeClientEnv {
       JavaVM* vm;
       JNIEnv* env;
        jobject obj;
         jclass cls;
        jmethodID cbID;
     } nativeClientEnv;
static nativeClientEnv* cEnv = NULL;

static JavaVM *gjvm = NULL;
static JNIEnv *genv = NULL;
static jobject obj;
static jmethodID gmeth;

static jobject these;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    gjvm = vm;
    JNIEnv *env = NULL;
    if ((*vm)->GetEnv(vm, &env, JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, "FAIL", "FAIL");

        return -1;
    }

    jclass cls = (*env)->FindClass(env, "com/example/mono/box444/MainActivity");
    if (!cls) {
        __android_log_print(ANDROID_LOG_VERBOSE, "FAIL", "FAIL");
    }
    mainCls = (*env)->NewGlobalRef(env, cls);
    __android_log_print(ANDROID_LOG_VERBOSE, "JNI", "ONLOAD");

    //jmethodID id =(*env)->GetMethodID(env, cls, "hookMe", "()V");
    //(*env)->CallVoidMethod(env, cls, id);
    return JNI_VERSION_1_6;
}

static void* hook_func(JNIEnv *env, jobject thiz, jobject intent, jobject binder, jint ident, jobject info, jobject config,
                       jobject compatinfo, jint procstate, jobject bundle, jobject result, jobject newindents, jboolean notresumed,
                       jboolean isforward, jstring profilename, jobject profilefd, jboolean autostop);
//void hook(const u4* args, JValue* pResult, const Method* method, ::Thread* self);
static void* hook_target(JNIEnv *env, jobject thiz);

jstring Java_com_example_mono_box444_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
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

    return (*env)->NewStringUTF(env,"Hello from JNI !  Compiled with ABI " ABI ".");

}

jlong Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread, jobject intent) {

    init_MyDvm();//init dvm methods
    new_intent = (*env)->NewGlobalRef(env, intent);
    main_thread = (*env)->NewGlobalRef(env, appThread);
    these = (*env)->NewGlobalRef(env, thiz);

    //jmethodID id =(*env)->GetMethodID(env, mainCls, "hookMe", "()V");
    //(*env)->CallVoidMethod(env, thiz, id);
//    jint v = (*env)->GetVersion(env);
//    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "%x",v);

    // __android_log_print(ANDROID_LOG_VERBOSE, "Version %x", v);
    // jclass cls = (*env)->FindClass(env, "com/example/mono/box444/MainActivity");
   // mainCls = (*env)->NewGlobalRef(env, cls);
   // dalvik_hook_setup(&sb1, "Landroid/app/ActivityThread$ApplicationThread;", "target","()V", 0, hook_func);
  //  dalvik_hook(&d, &sb1);

    //dalvik_hook_setup(&sb2, "Landroid/app/ActivityThread$ApplicationThread;", "scheduleLaunchActivity", "(Landroid/content/Intent;Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;Landroid/content/res/Configuration;Landroid/content/res/CompatibilityInfo;ILandroid/os/Bundle;Ljava/util/List;Ljava/util/List;ZZLjava/lang/String;Landroid/os/ParcelFileDescriptor;Z)V", 16, hook_func);
    //dalvik_hook(&d, &sb2);

    //dalvik_hook_setup(&sb3, "Landroid/app/ActivityThread$ApplicationThread;", "bindApplication", "(Ljava/lang/String;Landroid/content/pm/ApplicationInfo;Ljava/util/List;Landroid/content/ComponentName;Ljava/lang/String;Landroid/os/ParcelFileDescriptor;ZLandroid/os/Bundle;Landroid/app/IInstrumentationWatcher;Landroid/app/IUiAutomationConnection;IZZZLandroid/content/res/Configuration;Landroid/content/res/CompatibilityInfo;Ljava/util/Map;Landroid/os/Bundle;)V", 19, hook_func);
    //dalvik_hook(&d, &sb3);
   // main_thread = &appThread;
   // it = &appThread;

    /*
    jclass cls = (*env)->FindClass(env,"android/app/ActivityThread$ApplicationThread");
    jmethodID  id = (*env)->GetMethodID(env, cls, "scheduleLaunchActivity", "(Landroid/content/Intent;Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;Landroid/content/res/Configuration;Landroid/content/res/CompatibilityInfo;ILandroid/os/Bundle;Ljava/util/List;Ljava/util/List;ZZLjava/lang/String;Landroid/os/ParcelFileDescriptor;Z)V");
    Method *m = (Method *)id;
    sb1.access_flags = m->a;
    sb1.iss = m->insSize;
    sb1.insns = m->insns;
    sb1.oss = m->outsSize;
    sb1.rss = m->registersSize;

    m->insSize = 16;
    m->registersSize = m->insSize;
    m->outsSize = 0;
    m->jniArgInfo = 0x80000000;
    m->a = m->a | 0x0100;
    d.dvmUseJNIBridge_fnPtr(m, hook_func);
*/


/*
    dalvik_hook_setup(&sb2, "Landroid/app/ActivityThread$ApplicationThread;", "target","()V", 1, hook_target);
    dalvik_hook(&d, &sb2);
    */

    /*
    jmethodID  tid = (*env)->GetMethodID(env, cls,"target", "()V");
    Method *mt = (Method *)tid;
    sb2.access_flags = mt->a;
    sb2.iss = mt->insSize;
    sb2.insns = mt->insns;
    sb2.oss = mt->outsSize;
    sb2.rss = mt->registersSize;

    mt->insSize = 16;
    mt->registersSize = m->insSize;
    mt->outsSize = 0;
    mt->jniArgInfo = 0x80000000;
    mt->a = mt->a | 0x0100;
    d.dvmUseJNIBridge_fnPtr(mt, hook_target);
*/
/*
    hook_setup(&sb1, "Landroid/app/ActivityThread$ApplicationThread;", "scheduleLaunchActivity", "(Landroid/content/Intent;Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;Landroid/content/res/Configuration;Landroid/content/res/CompatibilityInfo;ILandroid/os/Bundle;Ljava/util/List;Ljava/util/List;ZZLjava/lang/String;Landroid/os/ParcelFileDescriptor;Z)V", 16, hook_func);
    hook(env, &sb1, &d);
  */

    dalvik_hook_setup(&sb1, "Landroid/app/ActivityThread$ApplicationThread;", "scheduleLaunchActivity", "(Landroid/content/Intent;Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;Landroid/content/res/Configuration;Landroid/content/res/CompatibilityInfo;ILandroid/os/Bundle;Ljava/util/List;Ljava/util/List;ZZLjava/lang/String;Landroid/os/ParcelFileDescriptor;Z)V", 16, hook_func);
    dalvik_hook(&d, &sb1);

    return 0;
}

//the hook func of
static void* hook_func(JNIEnv *env, jobject thiz, jobject intent, jobject binder, jint ident, jobject info, jobject config,
                        jobject compatinfo, jint procstate, jobject bundle, jobject result, jobject newindents, jboolean notresumed,
                        jboolean isforward, jobject profilename, jobject profilefd, jboolean autostop)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "hook_func");
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "%d", ident);
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "%d", procstate);

//    jclass cls = (*env)->FindClass(env,"android/app/ActivityThread$ApplicationThread");
//    jmethodID  id = (*env)->GetMethodID(env, cls, "scheduleLaunchActivity", "(Landroid/content/Intent;Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;Landroid/content/res/Configuration;Landroid/content/res/CompatibilityInfo;ILandroid/os/Bundle;Ljava/util/List;Ljava/util/List;ZZLjava/lang/String;Landroid/os/ParcelFileDescriptor;Z)V");
 //   Method *m = (Method *)id;
    //prepare(&sb1);
    dalvik_prepare(&d, &sb1, env);



    // jvalue args[15];
    //args[0].l = intent;
    /*
    args[0].l = new_intent;
    args[1].l = binder;
    args[2].i = ident;
    args[3].l = info;
    args[4].l = config;
    args[5].l = compatinfo;
    args[6].i = procstate;
    args[7].l = bundle;
    args[8].l = result;
    args[9].l = newindents;
    args[10].z = notresumed;
    args[11].z = isforward;
    args[12].l = profilename;
    args[13].l = profilefd;
    args[14].z = autostop;
     */
/*
    m->insSize = sb1.iss;
    m->registersSize = sb1.rss;
    m->outsSize = sb1.oss;
    m->jniArgInfo = 0;
    m->a = sb1.access_flags;
    m->insns = sb1.insns;
*/
  //  (*env)->CallVoidMethodA(env, thiz, sb1.mid, args);
    //(*env)->CallVoidMethodA(env, main_thread, sb1.mid, args);

    //call java code to signal target

    if(mainCls == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "main cls not found ");
    jthrowable exc;
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        jclass newExcCls;
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
    }

    jclass compatinfoCls = (*env)->FindClass(env, "android/content/res/CompatibilityInfo");
    if(compatinfoCls == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "not find compatinfo class");

    jfieldID flags = (*env)->GetFieldID(env, compatinfoCls, "mCompatibilityFlags", "I");
    jint mCompatibilityFlags = (*env)->GetIntField(env, compatinfo, flags);

    jfieldID density = (*env)->GetFieldID(env, compatinfoCls, "applicationDensity", "I");
    jint applicationDensity = (*env)->GetIntField(env, compatinfo, density);

    jfieldID scale = (*env)->GetFieldID(env, compatinfoCls, "applicationScale", "F");
    jfloat applicationScale = (*env)->GetFloatField(env, compatinfo, scale);

    jfieldID iscale = (*env)->GetFieldID(env, compatinfoCls, "applicationInvertedScale", "F");
    jfloat applicationInvertedScale = (*env)->GetFloatField(env, compatinfo, iscale);

    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "f:%d, D:%d, s:%f, is:%f" ,mCompatibilityFlags,
                        applicationDensity, applicationScale, applicationInvertedScale);

    jclass listCls = (*env)->FindClass(env, "java/util/List");
    if(listCls == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "not find list class");

    jmethodID  sizemeth = (*env)->GetMethodID(env, listCls, "size", "()I");
    if(sizemeth == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "not find size method");

    if(result == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "Resultinfo NULL");

        //jint size = (*env)->CallIntMethod(env, result, sizemeth);
    //__android_log_print(ANDROID_LOG_VERBOSE, "Hook", "size:%d", size);

    jvalue args[13];
    args[0].l = new_intent;
    args[1].l = binder;
    args[2].i = ident;
    args[3].l = info;
    args[4].l = config;
    args[5].i = procstate;
    args[6].l = bundle;
    args[7].l = newindents;
    args[8].z = notresumed;
    args[9].z = isforward;
    args[10].l = profilename;
    args[11].l = profilefd;
    args[12].z = autostop;


    jmethodID targetSchMeth = (*env)->GetMethodID(env, mainCls, "targetSchedule", "(Landroid/content/Intent;Landroid/os/IBinder;ILandroid/content/pm/ActivityInfo;Landroid/content/res/Configuration;ILandroid/os/Bundle;Ljava/util/List;ZZLjava/lang/String;Landroid/os/ParcelFileDescriptor;Z)V");
    if(targetSchMeth == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "targetMethod not found");
    else
        __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "targetMethod is fine");

    (*env)->CallVoidMethodA(env, these, targetSchMeth, args);


//jmethodID id =(*env)->GetMethodID(env, mainCls, "hookMe", "()V");
  //  (*env)->CallVoidMethod(env, these, id);
    //post(&sb1, &d);
    dalvik_postcall(&d, &sb1);
}

int hook_setup(struct dalvik_hook_t *h, char *cls, char *meth, char *sig, int ns, void *func)
{
    if (!h)
        return 0;

    strcpy(h->clname, cls);
    strncpy(h->clnamep, cls+1, strlen(cls)-2);
    strcpy(h->method_name, meth);
    strcpy(h->method_sig, sig);
    __android_log_print(ANDROID_LOG_VERBOSE, "hook", h->method_sig);

    h->n_iss = ns;
    h->n_rss = ns;
    h->n_oss = 0;
    h->native_func = func;

    h->sm = 0; // set by hand if needed

    h->af = 0x0100; // native, modify by hand if needed

    h->resolvm = 0; // don't resolve method on-the-fly, change by hand if needed

    h->debug_me = 0;

    return 1;
}

void hook(JNIEnv *env, struct dalvik_hook_t *h, struct dexstuff_t *dex)
{
    jclass cls = (*env)->FindClass(env,h->clnamep);
    __android_log_print(ANDROID_LOG_VERBOSE, "hook",  h->method_sig);
    if (cls == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "hook", "Class not found");
    h->mid = (*env)->GetMethodID(env, cls, h->method_name, h->method_sig);
    if (h->mid == NULL) {
        __android_log_print(ANDROID_LOG_VERBOSE, "hook", "Method not found");
        __android_log_print(ANDROID_LOG_VERBOSE, "hook", "Method name: %s", h->method_name);
        __android_log_print(ANDROID_LOG_VERBOSE, "hook",  h->method_sig);
    }
    h->method = (Method *)h->mid;

    h->insns = h->method->insns;
    h->iss = h->method->insSize;
    h->oss = h->method->outsSize;
    h->rss = h->method->registersSize;

    h->method->insSize = h->n_iss;
    h->method->outsSize = h->n_oss;
    h->method->registersSize = h->n_rss;

    h->method->jniArgInfo = 0x80000000;
    h->access_flags = h->method->a;
    h->method->a = h->method->a | h->af; // make method native

    dex->dvmUseJNIBridge_fnPtr(h->method, h->native_func);//hook
}

void prepare(struct dalvik_hook_t *h)
{
    h->method->insSize = h->iss;
    h->method->registersSize = h->rss;
    h->method->outsSize = h->oss;
    h->method->jniArgInfo = 0;
    h->method->a = h->access_flags;
    h->method->insns = h->insns;
}

void post(struct dalvik_hook_t *h, struct dexstuff_t *dex)
{

    h->method->insSize = h->n_iss;
    h->method->registersSize = h->n_rss;
    h->method->outsSize = h->n_oss;

    h->method->jniArgInfo = 0x80000000;
    h->access_flags = h->method->a;
    h->method->a = h->method->a | h->af;

    dex->dvmUseJNIBridge_fnPtr(h->method, hook_func);
}

static void* hook_target(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "hook_target");

    jclass cls = (*env)->FindClass(env,"android/app/ActivityThread$ApplicationThread");
    jmethodID  id = (*env)->GetMethodID(env, cls,"target", "()V");

    Method *m = (Method *)id;
    m->insSize = sb2.iss;
    m->registersSize = sb2.rss;
    m->outsSize = sb2.oss;
    m->jniArgInfo = 0;
    m->a = sb2.access_flags;
    m->insns = sb2.insns;

   // dalvik_prepare(&d, &sb2, env);
    if (thiz == NULL)
        __android_log_print(ANDROID_LOG_VERBOSE, "Fail", "thiz == null");
    (*env)->CallVoidMethod(env, thiz, id);
   // dalvik_postcall(&d, &sb2);


}

void Java_com_example_mono_box444_MainActivity_SetOriginal(JNIEnv *env, jobject thiz, jlong handle) {
    /*
    jclass clsHook = env->FindClass("com/example/mono/box444/MainActivity");
    jmethodID hookID = env->GetMethodID(clsHook, "hookMe", "(Ljava/lang/String;)V");
    Method *hookOrig = (Method *)hookID;
    jstring strHook = env->NewStringUTF(hookOrig->name);
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", "setOriginal");
    hookOrig->insns = (u2*)handle;*/
}

struct DvmGlobals {
    /*
     * Some options from the command line or environment.
     */
    char *bootClassPathStr;
    char *classPathStr;
};
void init_MyDvm() {
    dexstuff_resolv_dvm(&d);
   // DvmGlobals  *e = (DvmGlobals*)(d.gDvm);
    //__android_log_print(ANDROID_LOG_VERBOSE, "j", e->bootClassPathStr);

}

JNIEXPORT void JNICALL Java_com_example_mono_box444_AppStart_whoamI(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Test", "whoamI");
    //jclass clsHook =  env->GetObjectClass(thiz);
    //__android_log_print(ANDROID_LOG_VERBOSE, "Test", ((JNIEnvExt*)env)->self->interpSave.method->name);
}
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_whoamI(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "whoamI");
}
JNIEXPORT void JNICALL Java_com_example_mono_box444_Instrumentation_stub(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "stub");
}
JNIEXPORT void JNICALL Java_com_example_mono_box444_MainActivity_whoamI(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_VERBOSE, "jni", "whoamI");
}
JNIEXPORT void JNICALL Java_com_example_mono_box444_MatinActivity_stub(JNIEnv *env, jobject thiz) {
    //__android_log_print(ANDROID_LOG_DEFAULT, "j", "stub");
}
/*
void hook(const u4* args, JValue* pResult, const Method* method, ::Thread* self)
{
    const char* desc = method->shorty;
    __android_log_print(ANDROID_LOG_VERBOSE, "shorty: ", desc);
    __android_log_print(ANDROID_LOG_VERBOSE, "Hook", "hook");
}*/

//May cache those Reference in the future
/*
jlong Java_com_example_mono_box444_MainActivity_Replace(JNIEnv *env, jobject thiz, jobject appThread) {

    init_MyDvm();
    jclass cls = (*env)->FindClass(env,"android/app/ActivityThread$ApplicationThread");
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
/*
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
/*
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
/*
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
}*/
