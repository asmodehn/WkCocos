package com.gameparkstudio.wkcocos.lib;

import android.app.Activity;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.app.AlarmManager;
import android.content.IntentFilter;
import android.support.v4.app.NotificationCompat;

import java.lang.System;

//This is a singleton. ref : http://howtodoinjava.com/2012/10/22/singleton-design-pattern-in-java
//when used via getInstance(), it will retrieve the context from the main activity
//This context can be used to launch other activities
public class PushNotificationsManager{

    public static final String WKCOCOS_ACTION_GENERIC_PN = "com.gameparkstudio.wkcocos.PN";

    private static class LazyHolder {
        private static final PushNotificationsManager INSTANCE = new PushNotificationsManager();
    }

    public static PushNotificationsManager getInstance() {
        return LazyHolder.INSTANCE;
    }

    //to manage alarms
    AlarmManager am;

    public Activity appPNClickActivity;

    //to receive alarm intents
    BroadcastReceiver br;

    //Setting the Main Activity upon creation, for later revival via scheduled PN
    public boolean setActivity(Activity pnClickActivity) {
        appPNClickActivity = pnClickActivity;
        am = (AlarmManager) appPNClickActivity.getSystemService(appPNClickActivity.ALARM_SERVICE);

        //we have an activity, we can now respond whenreceiving alarm :
        br = new BroadcastReceiver() {
            @Override
            public void onReceive(Context c, Intent i) {
                NotificationCompat.Builder WKBuilder;
                WKBuilder = new NotificationCompat.Builder(appPNClickActivity)
                        .setSmallIcon(R.drawable.ic_launcher)
                        .setWhen(i.getLongExtra("when",System.currentTimeMillis()))
                        .setContentTitle(i.getStringExtra("title"))
                        .setContentText(i.getStringExtra("message"));

                Intent ResumeIntent = new Intent(appPNClickActivity,appPNClickActivity.getClass());
                ResumeIntent.setAction(Intent.ACTION_MAIN);
                ResumeIntent.addCategory(Intent.CATEGORY_LAUNCHER);
                ResumeIntent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);

                PendingIntent ResumePI = PendingIntent.getActivity(appPNClickActivity, 0, ResumeIntent, PendingIntent.FLAG_UPDATE_CURRENT);

                WKBuilder.setContentIntent(ResumePI);

                NotificationManager WKNM = (NotificationManager)appPNClickActivity.getSystemService(appPNClickActivity.NOTIFICATION_SERVICE);
                WKNM.notify(i.getIntExtra("id",0), WKBuilder.build());

            }

        };

        return true;
    }

    public void schedule(int id, long when, String title, String message) {

        long time = System.currentTimeMillis() + when * 1000;

        Intent PNIntent = new Intent( WKCOCOS_ACTION_GENERIC_PN + id ); //adding the id to have multiple intents
        appPNClickActivity.registerReceiver(br, new IntentFilter(WKCOCOS_ACTION_GENERIC_PN + id));//registering receiver for this new PN

        PNIntent.putExtra("id",id);
        PNIntent.putExtra("when",time);
        PNIntent.putExtra("title",title);
        PNIntent.putExtra("message",message);
        PendingIntent PNPI = PendingIntent.getBroadcast( appPNClickActivity , 0 , PNIntent, 0);
        am.set(AlarmManager.RTC, time, PNPI);

    }
	
}