package com.pvdnc.world;

import android.content.Context;
import android.os.Binder;
import android.os.IBinder;
import android.os.RemoteException;
import android.system.Os;
import android.util.Log;
import android.util.SparseArray;

import java.util.concurrent.atomic.AtomicReference;

import androidx.annotation.Nullable;

public class DexService extends IDexService.Stub {
    private static final String TAG=DexService.class.getSimpleName();

    private static final AtomicReference<DexService> sInstance=new AtomicReference<>();

    public static void systemReady(@Nullable Context context) throws DoubleFetchException{
        if(sInstance.get()!=null){
            throw new DoubleFetchException();
        }
        sInstance.set(new DexService(context));
    }

    public static DexService get(){
        return sInstance.get();
    }

    public static final String SERVICE_NAME="DexService";
    private DexService(@Nullable Context context){
        mContext=context;
        if(mContext!=null)
            return;
        if(!ServiceManager.listService().contains(SERVICE_NAME)) {//作为服务端
            if(!UserUtils.isPrivilegedUid(Os.getuid()))
                return;
            Log.e(TAG,"attempt to load as server at pid:"+ Os.getpid());
            boolean addResult= ServiceManager.addService(SERVICE_NAME, this);
            Log.d(TAG, "add service directly:"+addResult);
        }
    }

    private Context mContext;

    private final SparseArray<IDexService> mPidTargetMap=new SparseArray<>();

    private class RemoteDeathListener implements DeathRecipient{
        private int mPid;
        private IBinder mCaller;

        RemoteDeathListener(int pid,IBinder caller){
            mPid=pid;
            mCaller=caller;
        }

        @Override
        public void binderDied() {
            try {
                removeClient(mPid, mCaller);//客户端进程死亡后从Map中移除
                mCaller.unlinkToDeath(this, 0);
            }catch (Exception e){
                e.printStackTrace();
            }

        }
    }

    @Override
    public void addClient(final IBinder caller) throws RemoteException {
        final int pid= Binder.getCallingPid();
        long token=Binder.clearCallingIdentity();
        synchronized (mPidTargetMap) {
            if (mPidTargetMap.get(pid) == null) {
                Log.d(TAG, "add client of pid:" + pid);
                //注册进程死亡监听
                try {
                    caller.linkToDeath(new RemoteDeathListener(pid, caller), 0);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                mPidTargetMap.put(pid, asInterface(caller));
            } else {
                Log.e(TAG, "re-connect to the service is not allowed");
            }
        }
        Binder.restoreCallingIdentity(token);
    }

    private void removeClient(int pid,IBinder caller){
        Log.d( TAG,"attempt to remove client pid:"+pid+" remote:"+caller);
        synchronized (mPidTargetMap){
            IDexService proxy= mPidTargetMap.get(pid);
            if(proxy==null) {
                Log.e(TAG,"unexpected remove request of pid:"+pid+" remote:"+caller);
                return;
            }
            if(!proxy.asBinder().equals(caller))//必须保证两个都相同
                return;
            Log.e( TAG,"remove client pid:"+pid+" remote:"+caller);
            mPidTargetMap.remove(pid);
        }
    }

    @Override
    public int requestLoadDex(int targetPid, String json) throws RemoteException {
        int fromPid=Binder.getCallingPid();
        long token=Binder.clearCallingIdentity();
        IDexService target=null;
        synchronized (mPidTargetMap){
            Log.d(TAG,"get client:"+targetPid+" for pid:"+fromPid);
            target=mPidTargetMap.get(targetPid);
        }
        if(target==null) {
            Log.e(TAG,"targetPid:"+targetPid+" has not been registered");
            return -1;
        }
        target.scheduleLoadDex(fromPid,json);//回调目标

        Binder.restoreCallingIdentity(token);
        return 0;
    }

    @Override
    public int scheduleLoadDex(int fromPid, String json) throws RemoteException {
        Log.d(TAG,"receive request of loading dex from pid:"+fromPid);
        Log.d(TAG,"json:"+json);
        return 0;
    }
}
