package com.pvdnc.world;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.List;

public class MainActivity extends AppCompatActivity {
    private static final String TAG=MainActivity.class.getSimpleName();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private IDexService mService;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Button cmdLoadService=findViewById(R.id.cmdLoadService);
        cmdLoadService.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String optDir=getDir("optDex",MODE_PRIVATE).getAbsolutePath();
                loadService(optDir);
                //connectToService();
            }
        });

        Button cmdListService=findViewById(R.id.cmdListService);
        cmdListService.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                List<String> serviceList=ServiceManager.listService();
                Log.d(TAG,"list of services\n"+serviceList);
                Log.d(TAG,"DexService status:"+serviceList.contains(DexService.SERVICE_NAME));

            }
        });

        final EditText txtTargetPid=findViewById(R.id.txtTargetPid);
        Button cmdPushRequest=findViewById(R.id.cmdPushRequest);
        cmdPushRequest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               connectToService();
               try{
                  int targetPid=Integer.parseInt(txtTargetPid.getText().toString());
                  int result= mService.requestLoadDex(targetPid,"233");
                  if(result==-1){
                      Log.e(TAG,"targetPid:"+targetPid+" has not been registered");
                      return;
                  }
               }catch (Exception e){
                   e.printStackTrace();
               }
            }
        });

        ZygoteInit.checkHandle();

        //Log.d(TAG,"actual value:"+getNegativeUnsigned());

        //DexService.systemReady(this);
        //Log.d(TAG,"add dex service:"+ ServiceManager.addService("DexService",DexService.get()));
    }

    private void connectToService(){
        if(mService!=null)
            return;
        IBinder remote = ServiceManager.getService(DexService.SERVICE_NAME);
        if (remote == null) {
            Log.e(TAG, "fail to get service");
            return;
        }
        mService = IDexService.Stub.asInterface(remote);
        try {
            DexService.systemReady(MainActivity.this);
            mService.addClient(DexService.get());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void loadService(String optDir);

    public native int getNegativeUnsigned();
}
