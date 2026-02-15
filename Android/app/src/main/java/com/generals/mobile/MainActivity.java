package com.generals.mobile;

import com.google.androidgamesdk.GameActivity;

public class MainActivity extends GameActivity {
    static {
        System.loadLibrary("generals_mobile");
    }
}
