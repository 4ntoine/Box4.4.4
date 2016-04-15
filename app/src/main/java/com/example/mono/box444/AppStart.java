package com.example.mono.box444;

import android.app.Application;
import android.util.Log;

/**
 * Created by mono on 4/6/16.
 */
public class AppStart extends Application {
    @Override
    public void onCreate()
    {
        super.onCreate();
    }
    /* this is used to load the 'hello-jni' library on application
 * startup. The library has already been unpacked into
 * /data/data/com.example.hellojni/lib/libhello-jni.so at
 * installation time by the package manager.
 */
    static {
        System.loadLibrary("hello-jni");
    }
    public void stub(String s)
    {
        Log.d("AppStart", "From stub");
    }
    public void stub2()
    {
        Log.d("AppStart", "From stub");
    }

    public void callOrigin()
    {

    }
}
