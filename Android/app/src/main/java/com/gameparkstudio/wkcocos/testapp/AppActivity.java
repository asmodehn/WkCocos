package com.gameparkstudio.wkcocos.testapp;

import android.app.Activity;
import android.app.NotificationManager;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;
import com.gameparkstudio.wkcocos.lib.MainActivity;

public class AppActivity extends MainActivity {

    private final static String TAG = AppActivity.class.getSimpleName();

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        nativeInitGPGS(this);
        nativeOnActivityCreated(this, savedInstanceState);
	}

    public String getVersionName()
    {
        //gradle
        String versionName = BuildConfig.VERSION_NAME;
        return versionName;
    }

    public int getVersionCode()
    {
        //gradle // careful : current package !
        int versionCode = BuildConfig.VERSION_CODE;
        return versionCode;
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent result) {
        super.onActivityResult(requestCode, resultCode, result);
        Log.i(TAG, "OnActivityResult: " + requestCode);

        // Pass the activity result to the C++ SDK so that it can resolve any
        // errors related
        // to Google Play Games. The C++ SDK only acts on Activity results that
        // contain the
        // unique request code.
        nativeOnActivityResult(this, requestCode, resultCode, result);
    }

    @Override
    protected void onResume() {
        super.onResume();
        //TODO : JNI binding to remove this from here and make it doable in C++ app
        NotificationManager WKNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        WKNM.cancelAll();
        nativeOnActivityResumed(this);
    }


    @Override
    protected void onPause() {
        super.onPause();
        nativeOnActivityPaused(this);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeOnActivityDestroyed(this);
    }


    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        nativeOnActivitySaveInstanceState(this, outState);
    }


    @Override
    protected void onStart() {
        super.onStart();
        nativeOnActivityStarted(this);
    }


    @Override
    protected void onStop() {
        super.onStop();
        nativeOnActivityStopped(this);
    }


    // Implemented in C++.

    private static native void nativeInitGPGS(AppActivity act);

    public static native void nativeOnActivityResult(Activity activity, int requestCode, int resultCode, Intent data);

    public static native void nativeOnActivityCreated(Activity activity, Bundle savedInstanceState);

    private static native void nativeOnActivityDestroyed(Activity activity);

    private static native void nativeOnActivityPaused(Activity activity);

    private static native void nativeOnActivityResumed(Activity activity);

    private static native void nativeOnActivitySaveInstanceState(Activity activity, Bundle outState);

    private static native void nativeOnActivityStarted(Activity activity);

    private static native void nativeOnActivityStopped(Activity activity);



}
