package com.gameparkstudio.wkcocos.lib;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import com.gameparkstudio.wkcocos.lib.Utils;

public class MainActivity extends Activity {

    private final static String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "version=" + Utils.getVersionName() );

        setContentView(R.layout.activity_main);

    }
}
