package com.pvdnc.world;

import android.annotation.SuppressLint;
import android.os.IBinder;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ServiceManager {
    public static boolean addService(String name, IBinder service){
        try{
            @SuppressLint("PrivateApi")
            Class<?> smClass= Class.forName("android.os.ServiceManager");
            //android.os.ServiceManager#addService(String name.IBinder service)->V
            @SuppressLint("DiscouragedPrivateApi")
            Method addService=smClass.getDeclaredMethod("addService",String.class,IBinder.class);
            addService.setAccessible(true);
            addService.invoke(null,name,service);
        }catch (Exception e){
            e.printStackTrace();
        }
        return listService().contains(name);
    }

    public static List<String> listService(){
        List<String> serviceList=new ArrayList<>();
        try{
            @SuppressLint("PrivateApi")
            Class<?> smClass= Class.forName("android.os.ServiceManager");
            //android.os.ServiceManager#listServices()->String[]
            @SuppressLint("DiscouragedPrivateApi")
            Method listServices=smClass.getDeclaredMethod("listServices");
            listServices.setAccessible(true);
            String[] services= (String[]) listServices.invoke(null);
            serviceList.addAll(Arrays.asList(services));
        }catch (Exception e){
            e.printStackTrace();
        }
        return serviceList;
    }

    public static IBinder getService(String name){
        try{
            @SuppressLint("PrivateApi")
            Class<?> smClass= Class.forName("android.os.ServiceManager");
            //public static IBinder getService(String name)
            @SuppressLint("DiscouragedPrivateApi")
            Method getService=smClass.getDeclaredMethod("getService",String.class);
            getService.setAccessible(true);
            return (IBinder) getService.invoke(null,name);
        }catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }
}
