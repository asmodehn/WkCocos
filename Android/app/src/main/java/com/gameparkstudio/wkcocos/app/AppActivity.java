package com.gameparkstudio.wkcocos.app;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import com.gameparkstudio.wkcocos.lib.MainActivity;

public class AppActivity extends MainActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    @Override protected void onPause()
    {
        super.onPause();
        NotificationManager WKNM=(NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
        NotificationCompat.Builder WKBuilder =
                new NotificationCompat.Builder(this)
                        .setSmallIcon(android.R.drawable.stat_notify_more) //what are available options to set here?
                        .setContentTitle("WkCocos title")
                        .setContentText("WkCocos text");
        Intent WKIntent = new Intent(this, AppActivity.class);
        PendingIntent WKPI=PendingIntent.getActivity(
                getApplicationContext(), 0, WKIntent, 0);
        WKBuilder.setContentIntent(WKPI);
        WKNM.notify(0, WKBuilder.build());
    }

    @Override protected void onResume() {
        //Log.d(TAG, "Resuming app version " + Utils.getVersionName() );
        super.onResume();
    }
}
