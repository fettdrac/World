package com.pvdnc.world;

public class UserUtils {
    public static boolean isPrivilegedUid(int uid){
        if(uid==0||uid==1000)
            return true;
        return false;
    }
}
