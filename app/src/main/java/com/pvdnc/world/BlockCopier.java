package com.pvdnc.world;

import androidx.annotation.NonNull;

import java.util.Arrays;

public class BlockCopier {
    public static abstract class BlockCallback{
        protected abstract void onBlockGenerated(@NonNull byte[] block,int currentPosition);
    }

    public BlockCopier(byte[] fullData){
        mFullData=fullData;
    }

    private BlockCallback mCallback;

    public void setCallback(BlockCallback callback) {
        this.mCallback = callback;
    }

    private byte[] mFullData;

    public void start(int step){
        if(mCallback==null)
            return;
        int fullLength=mFullData.length;
        for (int i = 0; i < fullLength; i += step) {
            byte[] block=null;
            if(i+step<fullLength){
                block=new byte[step];
            }else{
                block=new byte[fullLength-i];
            }
            block = Arrays.copyOfRange(mFullData, i, i+block.length);
            mCallback.onBlockGenerated(block,i);
        }
    }
}
