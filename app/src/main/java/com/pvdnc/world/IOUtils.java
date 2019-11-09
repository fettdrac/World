package com.pvdnc.world;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class IOUtils {
    private static void ensureFile(File file) throws IOException {
        if (file.exists())
            return;
            if(!file.createNewFile())
                throw new IOException("fail to create file:"+file.getAbsolutePath());
    }

    public static void write(File file,byte[] data,boolean appendable) throws IOException {
        ensureFile(file);
        try (FileOutputStream fos = new FileOutputStream(file, appendable)) {
            fos.write(data);
        }
        if(!file.exists())
            throw new IOException("unknown reason of the failure of write file");
    }

    private static byte[] readSizedStream(InputStream inStream) throws IOException{
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int len = -1;
        while((len=inStream.read(buffer))!=-1){
            outStream.write(buffer,0,len);
        }
        outStream.close();
        inStream.close();
        return outStream.toByteArray();
    }

    public static byte[] read(File file) throws IOException{
        try(FileInputStream fis=new FileInputStream(file)){
            return readSizedStream(fis);
        }
    }
}
