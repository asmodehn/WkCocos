package com.asmodehn.wkcocos.lib;

import android.app.IntentService;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

public class PushNotificationsService extends IntentService {

    private static final String TAG = PushNotificationsService.class.getSimpleName();

    public PushNotificationsService() {
        super("PushNotificationsService");
    }

    @Override
    protected void onHandleIntent(Intent intent){
        Log.d(TAG, "onHandleIntent");
        NotificationCompat.Builder WKBuilder;
        WKBuilder = new NotificationCompat.Builder(this.getApplicationContext())
                .setSmallIcon(R.drawable.ic_launcher)
                .setWhen(intent.getLongExtra("when", System.currentTimeMillis()))
                .setContentTitle(intent.getStringExtra("title"))
                .setContentText(intent.getStringExtra("message"));

        Log.d(TAG, "Notification built");
        //get Main activity class
        String packageName = this.getPackageName();
        Intent launchIntent = this.getPackageManager().getLaunchIntentForPackage(packageName);
        String launchactivity = launchIntent.getComponent().getClassName();
        try {
            //prepate intent
            Intent ResumeIntent = new Intent(this, Class.forName(launchactivity));
            ResumeIntent.setAction(Intent.ACTION_MAIN);
            ResumeIntent.addCategory(Intent.CATEGORY_LAUNCHER);
            ResumeIntent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);

            Log.d(TAG, "Intent set to resume : " + launchactivity);

            PendingIntent ResumePI = PendingIntent.getActivity(this.getApplicationContext(), 0, ResumeIntent, PendingIntent.FLAG_UPDATE_CURRENT);

            WKBuilder.setContentIntent(ResumePI);

            NotificationManager WKNM = (NotificationManager) this.getApplicationContext().getSystemService(this.getApplicationContext().NOTIFICATION_SERVICE);
            WKNM.notify(intent.getIntExtra("id", 0), WKBuilder.build());
            Log.d(TAG, "Notification done.");
        }
        catch (ClassNotFoundException e) {
            Log.e(TAG,"Launch Activity class " + launchactivity + " not found !");
            e.printStackTrace();
        }
    }

}

