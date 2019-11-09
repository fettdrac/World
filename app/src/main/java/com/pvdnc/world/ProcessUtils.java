package com.pvdnc.world;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class ProcessUtils {
    public static String getProcessName(int pid) {
        String processName="";
        String cmdLinePath="/proc/" + pid + "/cmdline";
        try (BufferedReader reader=new BufferedReader(new FileReader(cmdLinePath))){
            processName= reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return processName.trim();//Parcel跨进程字符串遇到末尾空格会乱码
    }
}
