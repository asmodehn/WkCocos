package com.gameparkstudio.wkcocos.app;

import android.util.Log;

import com.gameparkstudio.wkcocos.lib.MainActivity;
import com.gameparkstudio.wkcocos.lib.Utils;

public class AppActivity extends MainActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    @Override protected void onPause() {
        super.onPause();
    }

    @Override protected void onResume() {
        //Log.d(TAG, "Resuming app version " + Utils.getVersionName() );
        super.onResume();
    }
}
