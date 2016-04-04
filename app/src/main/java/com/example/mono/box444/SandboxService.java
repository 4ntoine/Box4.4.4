package com.example.mono.box444;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.Application;
import android.app.Service;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;
import android.os.Process;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

//isolated process service

public class SandboxService extends Service {
    public SandboxService() {
    }

    /** Command to the service to display a message */
    static final int MSG_SAY_HELLO = 1;
    static final int MSG_SET_VALUE = 2;
    static final int MSG_PREPAPRE = 3;
    static final int MSG_TERMINATE = 4;
    static int callCount = 0;
    int mValue = 0;
    Messenger activityMessenger;
    String processName;
    Object mThread;
    //final Class<?> activityThreadClass = Class.forName("android.app.ActivityThread");


    /**
     * Handler of incoming messages from clients.
     */
    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_SAY_HELLO:
                    Toast.makeText(getApplicationContext(), "hello!", Toast.LENGTH_SHORT).show();
                    try{
                        callCount++;
                        msg.replyTo.send(Message.obtain(null, SandboxService.MSG_SAY_HELLO, callCount, 0));

                    }catch (RemoteException e){

                    }
                    break;
                case MSG_SET_VALUE:
                    // mValue = msg.arg1;
                    mValue = 999;
                    Message m = Message.obtain(null, SandboxService.MSG_SET_VALUE, mValue, 0);
                    try {
                        activityMessenger.send(Message.obtain(null,
                                MSG_SET_VALUE, mValue, 0));
                    } catch (RemoteException e) {
                        // The client is dead.  Remove it from the list;
                        // we are going through the list from back to front
                        // so this is safe to do inside the loop.

                    }

                    break;
                case MSG_PREPAPRE:
                    activityMessenger = msg.replyTo;
                    String name = Thread.currentThread().getName();
                    String pname = new String();
                    String ppname = new String();
/*
                    try {
                        final Class<?> activityThreadClass =
                                Class.forName("android.app.ActivityThread");
                     //   final Method method = activityThreadClass.getMethod("currentPackageName");
                         //(Application)method.invoke(null, (Object[]) null);
                      //   pname = (String)method.invoke(null,null);

                     //   final Method method2 = activityThreadClass.getMethod("currentProcessName");
                       // ppname = (String)method2.invoke(null,null);

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
*/
                    int pid = Process.myPid();
                    IBinder mThread = getApplicationThreadBinder();

                    Bundle b = new Bundle();
                    // b.putString("ThreadName", name);
                    b.putString("PackName", processName);
                    b.putString("ProcessName", processName);
                    b.putString("Pid", Integer.toString(pid));
                    b.putBinder("ThreadBinder", mThread);
                    Message mm = Message.obtain(null, SandboxService.MSG_PREPAPRE);
                    mm.setData(b);
                    try{
                        activityMessenger.send(mm);
                    }catch (RemoteException e){}
                    break;
                case MSG_TERMINATE:
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

    /**
     * When binding to the service, we return an interface to our messenger
     * for sending messages to the service.
     */
    @Override
    public IBinder onBind(Intent intent) {
        //Toast.makeText(getApplicationContext(), "binding", Toast.LENGTH_SHORT).show();
        return mMessenger.getBinder();
    }


    public int getNum(){
        return 50;
    }

    public IBinder getApplicationThreadBinder()
    {
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
            Toast.makeText(getApplicationContext(),processName, Toast.LENGTH_SHORT);
            mThread = applicationThread;

        try{
            Field[] ff = null;
            Method [] mmm = null;
            Field a;
            Object b;
            String cName = "android.app.ActivityThread$ApplicationThread";
            //try private call here to see if it works or not

            Class<?> [] t = Class.forName("android.app.ActivityThread").getDeclaredClasses();
            for(int i = 0; i < t.length; i++)
            {
                //Log.d("TEST", t[i].getName());
                if(t[i].getName().equals(cName) ) {
                   // Log.d("TEST", "here");
                    ff = t[i].getDeclaredFields();
                    //for (int j = 0; j < ff.length; j++)
                       // Log.d("TEST", ff[j].getName());
                    a = t[i].getDeclaredField("ONE_COUNT_COLUMN");
                    a.setAccessible(true);
                    b= a.get(mThread);
                    //Integer.toString(a);
                    Log.d("TEST", b.toString());
                }
            }


        } catch (final ClassNotFoundException e) {
            // handle exception
        } //catch (final NoSuchMethodException e) {}
            // handle exception
         catch (final IllegalArgumentException e) {
            // handle exception
        }// catch (final IllegalAccessException e) {
            // handle exception
        //} //catch (final InvocationTargetException e) {
            // handle exception
        //}

            return  ((IInterface) applicationThread).asBinder();
            //return null;
        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }
}
