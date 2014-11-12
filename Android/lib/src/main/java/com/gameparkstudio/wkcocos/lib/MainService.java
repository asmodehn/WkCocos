package com.gameparkstudio.wkcocos.lib;

import android.app.Service;
import android.app.NotificationManager;
import android.os.IBinder;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.app.PendingIntent;

import static com.gameparkstudio.wkcocos.lib.PushNotificationsManager.*;

public class MainService extends Service {

    @Override
    public IBinder onBind(Intent arg0) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @SuppressWarnings("static-access")
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);

        final int intent_id = getInstance().WKTitle.firstKey();

        NotificationCompat.Builder WKBuilder;
        WKBuilder = new NotificationCompat.Builder(this)
                .setSmallIcon(R.drawable.ic_launcher)
                .setContentTitle(getInstance().WKTitle.pollFirstEntry().getValue())
                .setContentText(getInstance().WKMessage.pollFirstEntry().getValue());

        Intent WKIntent = new Intent(MainActivity.getContext(), getInstance().appPNClickActivity);

        PendingIntent WKPI;
        WKPI = PendingIntent.getActivity(this, 0, WKIntent, 0);

        WKBuilder.setContentIntent(WKPI);

        NotificationManager WKNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        WKNM.notify(intent_id, WKBuilder.build());

        return START_STICKY;
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
    }

}