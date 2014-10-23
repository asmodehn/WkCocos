package com.gameparkstudio.wkcocos.app;

import android.os.Bundle;
import com.gameparkstudio.wkcocos.lib.MainActivity;
//import com.gameparkstudio.wkcocos.lib.PushNotificationsManager;

public class AppActivity extends MainActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(final Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        //TODO : can we do that in MainActivity in lib ??
        //PushNotificationsManager.getInstance().setActivity(AppActivity.class);
    }

    @Override protected void onPause()
    {
        super.onPause();
        //TODO : JNI binding to remove this from here and make it doable in C++ app
        //TODO : FIX "when" doesnt seem to work ?

    }

    @Override protected void onResume() {

        super.onResume();
        //TODO : cancel remaining PN
    }
}
