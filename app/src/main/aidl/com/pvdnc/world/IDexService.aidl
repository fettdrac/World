// IDexService.aidl
package com.pvdnc.world;

// Declare any non-default types here with import statements
import com.pvdnc.world.ProcessRecord;

interface IDexService {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
     void addClient(IBinder caller);

     List<ProcessRecord> listClient();

     int requestWritePart(int targetPid,String sessionName,String dataBase64);

     int scheduleWritePart(int fromPid,String sessionName,String dataBase64);

     void scheduleRemoveAllPartSessionCreatedByPid(int fromPid);

     int requestLoadDex(int targetPid,String json);

     int scheduleLoadDex(int fromPid,String json);
}
