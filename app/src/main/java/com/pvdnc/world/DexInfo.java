package com.pvdnc.world;

import android.app.Application;
import android.content.Context;
import android.system.Os;
import android.util.Log;

import com.google.gson.Gson;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Arrays;

import dalvik.system.DexClassLoader;

public class DexInfo {
    private static final String TAG=DexInfo.class.getSimpleName();

    public String mName;
    public byte[] mDexData;
    public String mLibSearchDir;

    private static final String DEX_DIR_NAME="loadedDex";

    public static DexInfo newInstance(String json) throws IllegalArgumentException,NullPointerException,IOException{
        DexInfo dexInfo=null;
        try {
            dexInfo = new Gson().fromJson(json, DexInfo.class);
        }catch (Exception e){
            throw new IllegalArgumentException(e);
        }
       Application app= ContextUtils.getApplication();
        //写入dex
       File dir=ContextUtils.getDir(DEX_DIR_NAME);
        File savedDexFile=new File(dir,dexInfo.mName+".dex");
        IOUtils.write(savedDexFile,dexInfo.mDexData,false);
        //加载dex
        File optDir=ContextUtils.getDir("optDex");
        ClassLoader parent=app!=null?app.getClassLoader():ClassLoader.getSystemClassLoader();
        DexClassLoader dcl=new DexClassLoader(savedDexFile.getAbsolutePath(),
                optDir.getAbsolutePath(),
                dexInfo.mLibSearchDir,parent);
        //设置ClassLoader
        dexInfo.setClassLoader(dcl);
        Log.d(TAG,"return new instance of dexInfo:"+dexInfo);
        return dexInfo;
    }

    public String mEntryClass;
    public String mMethodName;
    public String mParamJson;

    private transient ClassLoader mLoader;
    public void setClassLoader(ClassLoader loader) throws IllegalArgumentException{
        if(!(loader instanceof DexClassLoader)) {
            throw new IllegalArgumentException("mLoader must be set as a "+DexClassLoader.class.getName());
        }
        synchronized (this){
            mLoader=loader;
        }
    }

    public Class<?> loadClass(String name) throws NullPointerException, ClassNotFoundException {
        synchronized (this) {
            if (mLoader == null)
                throw new NullPointerException("mLoader has not been set");
            return mLoader.loadClass(name);
        }
    }

    public synchronized int executeNoException(ClassLoader loader){
        int retCode=-9;
        try {
            if(mLoader==null)
                setClassLoader(loader);
            Class<?> entryClass = loadClass(mEntryClass);
            Log.d(TAG,"entryClass:"+entryClass.getName()+" in pid:"+ Os.getpid());
            Method method= entryClass.getDeclaredMethod(mMethodName,String.class);
            method.setAccessible(true);
            Log.d(TAG,"method:"+method.getName()+"("+ Arrays.toString(method.getParameterTypes())+")"+" -> "+method.getReturnType());
            retCode= (int) method.invoke(null,mParamJson);
        }catch (Exception e){
            Log.e(TAG,"exception:"+e.getMessage()+" occurred while executing entry");
            e.printStackTrace();
        }
        Log.d(TAG,"ret:"+retCode);
        return retCode;
    }

    public static final String DEFAULT_ENTRY_CLASS_NAME="com.pvdnc.world.injector.Injector";
    public static final String DEFAULT_ENTRY_METHOD_NAME="start";
    public DexInfo(String name,byte[] dexData,String libSearchDir,
                   String entryClass,String methodName,String paramJson){
        mName=name;
        mDexData=dexData;
        mLibSearchDir=libSearchDir;

        mEntryClass=entryClass;
        if(mEntryClass==null)
            mEntryClass=DEFAULT_ENTRY_CLASS_NAME;
        mMethodName=methodName;
        if(mMethodName==null)
            mMethodName=DEFAULT_ENTRY_METHOD_NAME;
        mParamJson=paramJson;
    }
}
