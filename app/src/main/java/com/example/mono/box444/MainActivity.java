package com.example.mono.box444;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.ProviderInfo;
import android.content.res.Configuration;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Message;
import android.os.Messenger;
import android.os.Parcel;
import android.os.ParcelFileDescriptor;
import android.os.Parcelable;
import android.os.RemoteException;
import android.os.Process;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.telecom.Call;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.example.mono.box444.Instrumentation;
import com.google.gson.Gson;

import dalvik.system.DexClassLoader;
import dalvik.system.DexFile;

public class MainActivity extends AppCompatActivity {

    TextView mCallbackText;
    IBinder mAppThreadBinder;
    Object targetThread;
    int cPid;
    ApplicationInfo appInfo;
    long handle;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
        mCallbackText = (TextView)findViewById(R.id.textView2);

    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStart(){
        super.onStart();
        Intent intent = new Intent(this, SandboxService.class);
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
        int pid = Process.myPid();
        //mCallbackText.setText(Integer.toString(pid));
        //Log.d("SS", "COOL");
        mCallbackText.setText(stringFromJNI());
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    /** Messenger for communicating with service. */
    Messenger mService = null;
    /** Flag indicating whether we have called bind on the service. */
    boolean mBound;

    /**
     * Handler of incoming messages from service.
     */
    /** Some text view we are using to show state information. */
    //setContentView(R.layout.content_main);

    // View v = getLayoutInflater().inflate(R.layout.content_main, null);

    //TextView m = (TextView)(findViewById(R.id.fab));
    //TextView mCallbackText;
    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case SandboxService.MSG_SET_VALUE:
                    //mCallbackText.setText("Received from service: " + msg.arg1);
                    break;
                case SandboxService.MSG_SAY_HELLO:
                    mCallbackText.setText("HELLO from service: " + msg.arg1 + msg.arg2);
                    break;

                case SandboxService.MSG_PREPAPRE:
                    //mCallbackText.setText(msg.getData().getString("ProcessName"));
                    String p = msg.getData().getString("Pid");
                    mCallbackText.setText(msg.getData().getString("Pid"));
                    cPid = Integer.parseInt(p);
                    mAppThreadBinder = msg.getData().getBinder("ThreadBinder");
                    String tarThread = msg.getData().getString("ThreadString");
                    targetThread = new Gson().fromJson(tarThread, Object.class);

                    //if(AppThreadBinder.isBinderAlive())
                    // mCallbackText.setText(("BinderAlive"));
                    break;
                default:
                    super.handleMessage(msg);
            }
        }
    }

    /**
     * Target we publish for clients to send messages to IncomingHandler.
     */
    final Messenger mMessenger = new Messenger(new IncomingHandler());


    /*
     * Class for interacting with the main interface of the service.
     */
    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            // This is called when the connection with the service has been
            // established, giving us the object we can use to
            // interact with the service.  We are communicating with the
            // service using a Messenger, so here we get a client-side
            // representation of that from the raw IBinder object.
            mService = new Messenger(service);
            Log.d("TEST", "onServiceConnected");
            mBound = true;
        }

        public void onServiceDisconnected(ComponentName className) {
            // This is called when the connection with the service has been
            // unexpectedly disconnected -- that is, its process crashed.
            mService = null;
            mBound = false;
        }
    };

    public void sayHello(View v) {
        //if (!mBound) return;
        // Create and send a message to the service, using a supported 'what' value
        Message msg = Message.obtain(null, SandboxService.MSG_SAY_HELLO, 18, 0);
        msg.replyTo = mMessenger;
        try {
            mService.send(msg);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    public void prepare(View v){
        Message msg = Message.obtain(null, SandboxService.MSG_PREPAPRE);
        msg.replyTo = mMessenger;
        try {
            mService.send(msg);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    public void binderCall(View v) {
        //if (!mBound) return;
        // Create and send a message to the service, using a supported 'what' value
        // Message msg = Message.obtain(null, SandboxService.MSG_SAY_HELLO, 18, 0);

        //String s = mAppThreadBinder.;

    }
    public void buttonAddClick(View v)
    {
        Log.d("TEST", "buttonAddClick\n\n");
        prepare(v);
        sayHello(v);

        // int num = mService.getNum();
        //Toast.makeText(this, "num:" + 19, Toast.LENGTH_SHORT).show();
        List<ApplicationInfo> mApplications = new ArrayList<ApplicationInfo>();
        final int flag = PackageManager.GET_UNINSTALLED_PACKAGES | PackageManager.GET_DISABLED_COMPONENTS | PackageManager.GET_DISABLED_UNTIL_USED_COMPONENTS;
        PackageManager pm = this.getPackageManager();
        mApplications = pm.getInstalledApplications(flag);
        for(int i =0; i<mApplications.size(); i++){
            //ApplicationInfo info = mApplications.get(i);
            //Log.d("TEST", info.processName+info.sourceDir);

        }
        try{
            appInfo = pm.getApplicationInfo("com.example.xin.tester", 0);
            Log.d("TEST", appInfo.processName+appInfo.sourceDir);
        }
        catch (PackageManager.NameNotFoundException e){
            Log.d("TEST", "tester not found");
        }


        //File dexFile = getFileStreamPath("tester-2.apk");
       // Log.d("main", dexFile.getAbsolutePath());
    }


    public void buttonBindClick(View v) {

        Log.d("TEST", "buttonRemoveClick\n\n");

        String cName = "android.app.ActivityThread$ApplicationThread";

        String descriptor = "android.app.IApplicationThread";
        Parcel data = Parcel.obtain();
        Parcel reply;
        Configuration config;
        config = this.getResources().getConfiguration();
        boolean hour = true;
        Bundle b = new Bundle();
        String DEBUG_VIEW_ATTRIBUTES = "debug_view_attributes";
        b.putInt(DEBUG_VIEW_ATTRIBUTES, 0);

        data.writeInterfaceToken(descriptor);
        data.writeString(appInfo.processName);
        appInfo.writeToParcel(data, 0);
        data.writeTypedList(null);
        data.writeInt(0);//testName
        data.writeString("doesnotmatter");//profilename kitkat
        data.writeInt(0);//profilerInfo
        data.writeInt(0);//autoStopProfiler kitkat
        data.writeBundle(null);
        data.writeStrongInterface(null);
        data.writeStrongInterface(null);
        data.writeInt(1);//debug mode
        data.writeInt(0);//opengl
        data.writeInt(0);//restrictedBackupMode
        data.writeInt(0);//persistent
        config.writeToParcel(data, 0);
        //compatInfo.writeToParcel(data, 0);
        data.writeInt(4);//compatinfo
        data.writeInt(DisplayMetrics.DENSITY_DEFAULT);//compatinfo
        data.writeFloat(1.0f);//compatinfo
        data.writeFloat(1.0f);//compatinfo
        data.writeMap(null);//services
        //data.writeBundle(coreSettings);
        data.writeBundle(b);
        //  data.writeStrongBinder(token);
        //  data.writeInt(showWindow ? 1 : 0);
        //  data.writeInt(configChanges);
        //  mRemote.transact(SCHEDULE_STOP_ACTIVITY_TRANSACTION, data, null,
        //  IBinder.FLAG_ONEWAY);

        try {
            // mAppThreadBinder.transact(IBinder.FIRST_CALL_TRANSACTION + 51, data, null, IBinder.FLAG_ONEWAY);
            mAppThreadBinder.transact(IBinder.FIRST_CALL_TRANSACTION + 12, data, null, IBinder.FLAG_ONEWAY);
        } catch (final RemoteException e) {
            Log.d("TEST", "remote exception\n\n");
        }

        data.recycle();
    }
    public void callTarget()
    {
        String processName;
        Object mThread;
        IBinder binder = new Binder();
        String descriptor = "android.app.IApplicationThread";
        try {
            // Find ContextImpl
            Context context = this;
            while (context instanceof ContextWrapper) {
                context = ((ContextWrapper) context).getBaseContext();
            }

            // Get ContextImpl.mMainThread
            Field mainThreadField = Class.forName("android.app.ContextImpl").getDeclaredField("mMainThread");
            mainThreadField.setAccessible(true);
            Object mainThread = mainThreadField.get(context);

            // Get application thread and return it as binder
            Object applicationThread = Class.forName("android.app.ActivityThread").getMethod("getApplicationThread").invoke(mainThread);

            try{
                 processName = (String)Class.forName("android.app.ActivityThread").getMethod("getProcessName").invoke(mainThread);
            } catch (final ClassNotFoundException e) {
                // handle exception
            } catch (final NoSuchMethodException e) {
                // handle exception
            } catch (final IllegalArgumentException e) {
                // handle exception
            } catch (final IllegalAccessException e) {
                // handle exception
            } catch (final InvocationTargetException e) {
                // handle exception
            }
            //Toast.makeText(getApplicationContext(),processName, Toast.LENGTH_SHORT);
            mThread = applicationThread;

             binder = ((IInterface) applicationThread).asBinder();
            //return null;
        } catch (Exception e) {
            e.printStackTrace();
        }


        Parcel data = Parcel.obtain();
        data.writeInterfaceToken(descriptor);
        Parcel reply = Parcel.obtain();;

        try{
        binder.transact(IBinder.FIRST_CALL_TRANSACTION + 52, data, reply , IBinder.FLAG_ONEWAY);}
        catch(final RemoteException e){Log.d("Test", "remote exception");}

        data.recycle();
        reply.recycle();
    }

    public void buttonTarget(View V){
        callTarget();
    }
    public void buttonReplaceClick(View v) {
        //Intent i = new Intent(this, General.class);
        //startActivity(i);
        try {
            // Find ContextImpl
            Context context = this;
            while (context instanceof ContextWrapper) {
                context = ((ContextWrapper) context).getBaseContext();
            }

            // Get ContextImpl.mMainThread
            Field mainThreadField = Class.forName("android.app.ContextImpl").getDeclaredField("mMainThread");
            mainThreadField.setAccessible(true);
            Object mainThread = mainThreadField.get(context);

            // Get application thread and return it as binder
            Object applicationThread = Class.forName("android.app.ActivityThread").getMethod("getApplicationThread").invoke(mainThread);
           // mCallbackText.setText(Replace(applicationThread));

            //activity inside the same process
            Intent it = new Intent();
            String targetpackage = "com.example.mono.box444";
            ComponentName name = new ComponentName(targetpackage, Main3Activity.class.getCanonicalName());//may need hard write for name part here
            Log.d("CanonicalName", Main3Activity.class.getCanonicalName());
            it.setComponent(name);

            //main activity inside target
            Intent targetIntent = new Intent();
            String targetpack = "com.example.xin.tester";
            ComponentName targetName = new ComponentName(targetpack, "com.example.xin.tester.MainActivity");
            targetIntent.setComponent(targetName);

            handle = Replace(applicationThread, targetIntent);
        }catch (Exception e) {
            e.printStackTrace();
        }
    }
    public native String  stringFromJNI();
    public native long Replace(Object appThread, Object intent);
    public native void SetOriginal(long handle);
    public native void whoamI();
    public native void stub();
    public void hookMe()
    {
        Log.d("Test", "Hook me!");
    }
    public void buttonHookMe(View v)
    {
        hookMe();
    }
    public void CallOriginal(long handle)
    {
        SetOriginal(handle);
        hookMe();
    }
    public void buttonOrigin(View v)
    {
        CallOriginal(handle);
    }
    public void buttonStartActivity(View V)
    {
        Intent g = new Intent(this, Main2Activity.class);
        startActivity(g);
    }
    Instrumentation i = new Instrumentation();
    public void buttonJniHook(View V)
    {
        whoamI();
    }

    public void targetSchedule(Intent intent, IBinder token, int ident,
                               ActivityInfo info, Configuration curConfig,
                               int procState, Bundle state,
                               List<Intent> pendingNewIntents, boolean notResumed, boolean isForward,
                               String profileName, ParcelFileDescriptor profileFd, boolean autoStopProfiler)
    {
        Log.d("TargetSch", "targetSchedule");
        String descriptor = "android.app.IApplicationThread";
        Parcel data = Parcel.obtain();
        data.writeInterfaceToken(descriptor);
        intent.writeToParcel(data, 0);
        data.writeStrongBinder(token);
        data.writeInt(ident);
        info.writeToParcel(data, 0);
        curConfig.writeToParcel(data, 0);
        //compatInfo.writeToParcel(data, 0);
        //decompose compatinfo struct, hard write
        data.writeInt(0);
        data.writeInt(480);
        data.writeFloat(1.0f);
        data.writeFloat(1.0f);
        data.writeInt(procState);
        data.writeBundle(state);
        //data.writeTypedList(pendingResults);
        //pending result null, write -1
        data.writeInt(-1);
        data.writeTypedList(pendingNewIntents);
        data.writeInt(notResumed ? 1 : 0);
        data.writeInt(isForward ? 1 : 0);
        data.writeString(profileName);
        if (profileFd != null) {
            data.writeInt(1);
            profileFd.writeToParcel(data, Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
        } else {
            data.writeInt(0);
        }

            data.writeInt(autoStopProfiler ? 1 : 0);
        try {
            mAppThreadBinder.transact(IBinder.FIRST_CALL_TRANSACTION + 6, data, null,
                    IBinder.FLAG_ONEWAY);
        }catch (final RemoteException e) {
            Log.d("targetSchedule", "remote exception\n\n");
        }
        data.recycle();
    }

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
            Log.d("Instrumentation", "indent"+ident);
            Log.d("Instrumentation", "profileName"+profileName);
            Log.d("Instrumentation", "procstate"+procState);
           // Log.d("Instrumentation",mAppThreadBinder.toString());
            //mAppThreadBinder.tra
        }
    }

    public static void patchClassLoader(ClassLoader cl, File apkFile, File optDexFile)
            throws IllegalAccessException, NoSuchMethodException, IOException,
            InvocationTargetException, InstantiationException, NoSuchFieldException {

        // 获取 BaseDexClassLoader : pathList
        Field pathListField = DexClassLoader.class.getSuperclass().getDeclaredField("pathList");
        pathListField.setAccessible(true);
        Object pathListObj = pathListField.get(cl);

        // 获取 PathList: Element[] dexElements
        Field dexElementArray = pathListObj.getClass().getDeclaredField("dexElements");
        dexElementArray.setAccessible(true);
        Object[] dexElements = (Object[]) dexElementArray.get(pathListObj);

        // Element 类型
        Class<?> elementClass = dexElements.getClass().getComponentType();

        // 创建一个数组, 用来替换原始的数组
        Object[] newElements = (Object[]) Array.newInstance(elementClass, dexElements.length + 1);

        // 构造插件Element(File file, boolean isDirectory, File zip, DexFile dexFile) 这个构造函数
        Constructor<?> constructor = elementClass.getConstructor(File.class, boolean.class, File.class, DexFile.class);
        Object o = constructor.newInstance(apkFile, false, apkFile, DexFile.loadDex(apkFile.getCanonicalPath(), optDexFile.getAbsolutePath(), 0));

        Object[] toAddElementArray = new Object[] { o };
        // 把原始的elements复制进去
        System.arraycopy(dexElements, 0, newElements, 0, dexElements.length);
        // 插件的那个element复制进去
        System.arraycopy(toAddElementArray, 0, newElements, dexElements.length, toAddElementArray.length);

        // 替换
        dexElementArray.set(pathListObj, newElements);


    }
}

/*
        public void bindApplication(String packageName, ApplicationInfo info,
                List<ProviderInfo> providers, ComponentName testName, ProfilerInfo profilerInfo,
                Bundle testArgs, IInstrumentationWatcher testWatcher,
                IUiAutomationConnection uiAutomationConnection, int debugMode,
        boolean openGlTrace, boolean restrictedBackupMode, boolean persistent,
        Configuration config, CompatibilityInfo compatInfo, Map<String, IBinder> services,
                Bundle coreSettings) throws RemoteException {
            Parcel data = Parcel.obtain();
            data.writeInterfaceToken(IApplicationThread.descriptor);
            data.writeString(packageName);
            info.writeToParcel(data, 0);
            data.writeTypedList(providers);
            if (testName == null) {
                data.writeInt(0);
            } else {
                data.writeInt(1);
                testName.writeToParcel(data, 0);
            }
            if (profilerInfo != null) {
                data.writeInt(1);
                profilerInfo.writeToParcel(data, Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
            } else {
                data.writeInt(0);
            }
            data.writeBundle(testArgs);
            data.writeStrongInterface(testWatcher);
            data.writeStrongInterface(uiAutomationConnection);
            data.writeInt(debugMode);
            data.writeInt(openGlTrace ? 1 : 0);
            data.writeInt(restrictedBackupMode ? 1 : 0);
            data.writeInt(persistent ? 1 : 0);
            config.writeToParcel(data, 0);
            compatInfo.writeToParcel(data, 0);
            data.writeMap(services);
            data.writeBundle(coreSettings);
            mRemote.transact(BIND_APPLICATION_TRANSACTION, data, null,
                    IBinder.FLAG_ONEWAY);
            data.recycle();
        }*/
/*

        try {
            Method mm = Class.forName("android.app.ApplicationThreadNative").getMethod("asInterface");
          //mm.setAccessible(true);
           // mm.invoke(null, mAppThreadBinder);
           //Class.forName("android.app.ActivityThread").getMethod("getApplicationThread");
            //.invoke(null,mAppThreadBinder);
        } catch (final ClassNotFoundException e) {
            Log.d("TEST", "Class not found\n\n");
        } catch (final NoSuchMethodException e) {
            Log.d("TEST", "No such method\n\n");
        } catch (final IllegalArgumentException e) {
            Log.d("TEST", "Arguments\n\n");
        } /*catch (final IllegalAccessException e) {
            // handle exception
        } catch (final InvocationTargetException e) {
            // handle exception
        }*/
/*
public void buildData(String packageName, ApplicationInfo info,
                            Configuration config,
                            Bundle coreSettings) throws RemoteException {
    Parcel data = Parcel.obtain();
    Bundle b = new Bundle();
    String DEBUG_VIEW_ATTRIBUTES = "debug_view_attributes";
    b.putInt(DEBUG_VIEW_ATTRIBUTES, 0);
    data.writeInterfaceToken("android.app.IApplicationThread");
    data.writeString(packageName);
    info.writeToParcel(data, 0);
    data.writeTypedList(null);

    data.writeInt(0);//testName

    data.writeInt(0);//profilerInfo

    data.writeBundle(null);
    data.writeStrongInterface(null);
    data.writeStrongInterface(null);
    data.writeInt(0);//debug mode
    data.writeInt(0);//opengl
    data.writeInt(0);//restrictedBackupMode
    data.writeInt(0);//persistent
    config.writeToParcel(data, 0);
    //compatInfo.writeToParcel(data, 0);
    //data.writeInt(0);//compatinfo
    data.writeMap(null);//services
    data.writeBundle(b);


   // mRemote.transact(BIND_APPLICATION_TRANSACTION, data, null,
            //IBinder.FLAG_ONEWAY);
    //data.recycle();
    }*/