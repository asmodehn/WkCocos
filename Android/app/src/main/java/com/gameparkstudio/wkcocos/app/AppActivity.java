package com.gameparkstudio.wkcocos.app;

import android.app.NotificationManager;
import android.os.Bundle;
import com.gameparkstudio.wkcocos.lib.MainActivity;

public class AppActivity extends MainActivity {

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override protected void onPause() {
        super.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        //TODO : JNI binding to remove this from here and make it doable in C++ app
        NotificationManager WKNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        WKNM.cancelAll();
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
}
