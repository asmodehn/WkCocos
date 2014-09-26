package com.gameparkstudio.wkcocos.app;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.gameparkstudio.wkcocos.lib.Cocos2dxWebViewHelper;
import com.gameparkstudio.wkcocos.lib.MainActivity;
import com.gameparkstudio.wkcocos.lib.PushNotificationsManager;
import com.gameparkstudio.wkcocos.lib.Utils;

public class AppActivity extends MainActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //TODO : can we do that in MainActivity in lib ??
        PushNotificationsManager.getInstance().setActivity(AppActivity.class);
    }

    @Override protected void onPause()
    {
        super.onPause();
        //TODO : JNI binding to remove this from here and make it doable in C++ app
        //TODO : FIX "when" doesnt seem to work ?
        PushNotificationsManager.getInstance().Schedule(42, "Title Wkcocos", "UsefulMessage", R.drawable.ic_launcher);
    }

    @Override protected void onResume() {
        //Log.d(TAG, "Resuming app version " + Utils.getVersionName() );
        super.onResume();
        //TODO : cancel remaining PN
    }
}
