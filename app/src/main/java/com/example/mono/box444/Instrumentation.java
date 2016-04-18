package com.example.mono.box444;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.IBinder;
import android.os.ParcelFileDescriptor;
import android.os.Parcelable;
import android.util.Log;

import java.util.List;

/**
 * Created by moslab on 4/17/16.
 */
public class Instrumentation {
    public native void whoamI();
    public native void stub();
    public void replace() {

    }
    public void test(){
       // Log.d("Instrumentation", "I'm Test");
        whoamI();
      //  Log.d("Instrumentation", "I'm Test");
        }

    public final void hookLaunch(Intent intent, IBinder token, int ident,
                                 ActivityInfo info, Configuration curConfig, Configuration cuConfig,
                                 int procState, Bundle state, List<Configuration> pendingResults,
                                 List<Intent> pendingNewIntents, boolean notResumed, boolean isForward,
                                 String profileName, ParcelFileDescriptor profileFd, boolean autoStopProfiler)
    {
        Log.d("Instrumentatino", "indent"+ident);

    }
}
