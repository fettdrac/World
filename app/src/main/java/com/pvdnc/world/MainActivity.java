package com.pvdnc.world;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.gson.Gson;

import java.io.File;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;

public class MainActivity extends AppCompatActivity {
    private static final String TAG=MainActivity.class.getSimpleName();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private EditText txtDexPath;

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

        txtDexPath=findViewById(R.id.txtDexPath);
        Button cmdBrowse=findViewById(R.id.cmdBrowse);
        cmdBrowse.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                //调用选择Activity
                Intent resultIntent= Intent.createChooser(intent,"Select File");
                startActivityForResult(resultIntent,REQUEST_SELECT_FILE);
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

                  byte[] dexData= IOUtils.read(new File(txtDexPath.getText().toString()));
                  int totalCopiedSize= writeDexPartly(targetPid,dexData);//远程布局dex
                   if(totalCopiedSize!=dexData.length){
                       Log.e(TAG,"fail to copy all dexData to remote ("+totalCopiedSize+"/"+dexData.length);
                       return;
                   }
                   DexInfo dexInfo=new DexInfo("Demo",DEFAULT_SESSION_NAME,null,
                           null,null,"233");
                   int result= mService.requestLoadDex(targetPid,
                           new Gson().toJson(dexInfo));

                   if(result==-1) {
                       Log.e(TAG, "targetPid:" + targetPid + " has not been registered");
                       return;
                   }
                   Log.d(TAG,"requestLoadDex ret:"+result);
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

    private static final String DEFAULT_SESSION_NAME="DemoCopySession";
    private int writeDexPartly(final int targetPid, byte[] data){
        final String sessionName=DEFAULT_SESSION_NAME;
        final AtomicInteger aCopiedSize=new AtomicInteger(0);
        BlockCopier copier=new BlockCopier(data);
        copier.setCallback(new BlockCopier.BlockCallback() {
            @Override
            protected void onBlockGenerated(@NonNull byte[] block, int currentPosition) {
                String blockBase64=Base64.encodeToString(block,Base64.DEFAULT);
                try {
                    int writtenLength= mService.requestWritePart(targetPid,sessionName,blockBase64);
                    Log.d(TAG,"written length:"+ aCopiedSize.getAndAdd(writtenLength));
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        });
        copier.start(128);
        Log.d(TAG,"total copied size:"+aCopiedSize.get());
        return aCopiedSize.get();
    }

    private static final int REQUEST_SELECT_FILE=0x67;
    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode){
            case REQUEST_SELECT_FILE:{
                Objects.requireNonNull(data);
                Uri uri=  data.getData();
                Log.d(TAG,"uri:"+uri);
                Objects.requireNonNull(uri);
                String path=uri.getPath();
                Log.d(TAG,"uri path:"+path);
                txtDexPath.setText(path);
                break;
            }
        }
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
