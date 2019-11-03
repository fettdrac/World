package com.pvdnc.world;

public class DoubleFetchException extends IllegalStateException {
    public DoubleFetchException(){
        this("double-fetch detected");
    }

    public DoubleFetchException(String s) {
        super(s);
    }
}
