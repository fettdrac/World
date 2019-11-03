// IDexService.aidl
package com.pvdnc.world;

// Declare any non-default types here with import statements

interface IDexService {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
     void addClient(IBinder caller);

     int requestLoadDex(int targetPid,String json);

     int scheduleLoadDex(int fromPid,String json);
}
