package com.pvdnc.world;

import androidx.annotation.NonNull;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class ArrayCopyTest {
    @Test
    public void checkCopy(){
        byte[] fullData="12345678901".getBytes();
        int step=7;
        try(ByteArrayOutputStream bos=new ByteArrayOutputStream()) {
            for (int i = 0; i < fullData.length; i += step) {
                byte[] block=null;
                if(i+step<fullData.length){
                    block=new byte[step];
                }else{
                    block=new byte[fullData.length-i];
                }
                block = Arrays.copyOfRange(fullData, i, i+block.length);
                bos.write(block, 0, block.length);
            }
            byte[] restoredData=bos.toByteArray();
            if(fullData.length==restoredData.length)
                System.out.println("length is correct");
            System.out.println(new String(restoredData));
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    @Test
    public void checkCopier(){
        byte[] fullData="12345678901".getBytes();
        BlockCopier copier=new BlockCopier(fullData);
        final ByteArrayOutputStream bos=new ByteArrayOutputStream();
        copier.setCallback(new BlockCopier.BlockCallback() {
            @Override
            protected void onBlockGenerated(@NonNull byte[] block, int currentPosition) {
                try {
                    bos.write(block);
                    System.out.println(currentPosition+":"
                            +new String(block)
                            +"("+block.length+")");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        copier.start(1);
        byte[] restoredData=bos.toByteArray();
        if(fullData.length==restoredData.length)
            System.out.println("length is correct");
        System.out.println(new String(restoredData));
    }
}
