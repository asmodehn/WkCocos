package com.gameparkstudio.wkcocos.lib;

import android.app.Service;
import android.app.NotificationManager;
import android.os.IBinder;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.app.PendingIntent;

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

        NotificationManager WKNM = (NotificationManager)this.getApplicationContext().getSystemService(this.getApplicationContext().NOTIFICATION_SERVICE);
        NotificationCompat.Builder WKBuilder =
                new NotificationCompat.Builder(this.getApplicationContext())
                        .setSmallIcon(R.drawable.ic_launcher)
                        .setContentTitle(PushNotificationsManager.getInstance().WKTitle)
                        .setContentText(PushNotificationsManager.getInstance().WKMessage);

        Intent WKIntent = new Intent(this.getApplicationContext(), PushNotificationsManager.getInstance().appPNClickActivity);
        PendingIntent WKPI = PendingIntent.getActivity(this.getApplicationContext(), 0, WKIntent, 0);
        WKBuilder.setContentIntent(WKPI);
        WKNM.notify(0, WKBuilder.build());
        
        return START_STICKY;
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
    }

}