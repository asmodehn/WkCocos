package com.gameparkstudio.wkcocos.app;

import android.app.NotificationManager;
import android.os.Bundle;
import com.gameparkstudio.wkcocos.lib.MainActivity;
import com.gameparkstudio.wkcocos.lib.WkJniHelper;

public class AppActivity extends MainActivity {

    @Override
    protected void onCreate(final Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        //TODO : can we do that in MainActivity in lib ??
        WkJniHelper.getInstance().setActivity(this);
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
        return "0";
    }

    public int getVersionCode()
    {
        return 0;
    }
}
