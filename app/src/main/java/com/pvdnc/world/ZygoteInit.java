package com.pvdnc.world;

import android.annotation.SuppressLint;
import android.util.Log;

import java.lang.reflect.Method;
import java.util.Arrays;

public class ZygoteInit {
    private static final String TAG=ZygoteInit.class.getSimpleName();

    public static void checkHandle(){
        //com.android.internal.os.ZygoteInit#handleSystemServerProcess
        try {
            @SuppressLint("PrivateApi")
            Class<?> ziClass=Class.forName("com.android.internal.os.ZygoteInit");
            Log.d(TAG,"ziClass:"+ziClass);
            for (Method method:ziClass.getDeclaredMethods()){
                Log.d(TAG,method.getName()+"("+ Arrays.toString(method.getParameterTypes())+")"+" -> "+method.getReturnType());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
