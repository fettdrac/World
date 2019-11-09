package com.pvdnc.world;

import android.annotation.SuppressLint;
import android.app.Application;
import android.content.Context;
import android.system.Os;
import android.util.Log;

import java.io.File;
import java.lang.reflect.Method;

import androidx.annotation.NonNull;

public class ContextUtils {
    private static final String TAG=ContextUtils.class.getSimpleName();

    public static Application getApplication(){
        Application app=null;
        try {
            @SuppressLint("PrivateApi")
            Class<?> atClass=Class.forName("android.app.ActivityThread");
            @SuppressLint("DiscouragedPrivateApi")
            Method getApp=atClass.getDeclaredMethod("currentApplication");
            getApp.setAccessible(true);
            app= (Application) getApp.invoke(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return app;
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    @NonNull
    public static File getDir(String name) {
        Application app=getApplication();
        File dir=null;
        if(app==null) {
            dir=new File("/data/system/"+name);
            if(!dir.exists()||!dir.isDirectory())
                dir.mkdirs();
            if(!UserUtils.isPrivilegedUid(Os.getuid()))
                throw new NullPointerException("fail to get app instance");
        }else{
            dir=app.getDir(name, Context.MODE_PRIVATE);
        }
        Log.d(TAG,"dir:"+dir);
        return dir;
    }
}
