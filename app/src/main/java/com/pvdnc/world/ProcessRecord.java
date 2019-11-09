package com.pvdnc.world;

import android.os.IBinder;
import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.Expose;

import androidx.annotation.NonNull;

public class ProcessRecord implements Parcelable {
    public int mPid;
    public String mName;

    @Expose(serialize = false,deserialize = false)
    public IBinder mTarget;

    public ProcessRecord(int pid,IBinder target){
        mPid=pid;
        mTarget=target;

        mName=ProcessUtils.getProcessName(mPid);
    }

    @NonNull
    @Override
    public String toString() {
        StringBuilder sb=new StringBuilder();
        sb.append(super.toString()).append("\n");
        sb.append("\tprocess:").append(mName)
                .append("(").append(mPid).append(")").append("\n");
        sb.append("\ttarget:"+mTarget);
        return sb.toString();
    }

    protected ProcessRecord(Parcel in) {
        mPid = in.readInt();
        mName = in.readString();
        mTarget = in.readStrongBinder();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(mPid);
        dest.writeString(mName);
        dest.writeStrongBinder(mTarget);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<ProcessRecord> CREATOR = new Creator<ProcessRecord>() {
        @Override
        public ProcessRecord createFromParcel(Parcel in) {
            return new ProcessRecord(in);
        }

        @Override
        public ProcessRecord[] newArray(int size) {
            return new ProcessRecord[size];
        }
    };
}
