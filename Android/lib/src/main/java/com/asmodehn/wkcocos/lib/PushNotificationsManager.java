package com.asmodehn.wkcocos.lib;

import android.app.Activity;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.app.AlarmManager;

import java.lang.System;

public class PushNotificationsManager{

    public static final String WKCOCOS_ACTION_GENERIC_PN = "com.asmodehn.wkcocos.PN";

    //to manage alarms
    AlarmManager am;

    public Activity appPNClickActivity;

    //Setting the Main Activity upon creation, for later revival via scheduled PN
    PushNotificationsManager(Activity pnClickActivity) {
        appPNClickActivity = pnClickActivity;
        am = (AlarmManager) appPNClickActivity.getSystemService(appPNClickActivity.ALARM_SERVICE);
    }

    public void schedule(int id, long when, String title, String message) {

        //preparing intent for PN alarm
        long time = System.currentTimeMillis() + when * 1000;
        Intent PNIntent = new Intent(appPNClickActivity , PushNotificationsReceiver.class);
        PNIntent.setAction(WKCOCOS_ACTION_GENERIC_PN + id); //setting action to differentiate each PN
        PNIntent.setFlags(Intent.FLAG_INCLUDE_STOPPED_PACKAGES); //for the intent to wake up the receiver even if the app is stopped
        PNIntent.putExtra("id",id);
        PNIntent.putExtra("when",time);
        PNIntent.putExtra("title",title);
        PNIntent.putExtra("message",message);
        PendingIntent PNPI = PendingIntent.getBroadcast(appPNClickActivity, 0, PNIntent, 0);
        am.set(AlarmManager.RTC, time, PNPI);

    }

    public void cancel(int id) {

        //preparing intent for PN alarm
        Intent PNIntent = new Intent(appPNClickActivity , PushNotificationsReceiver.class);
        PNIntent.setAction(WKCOCOS_ACTION_GENERIC_PN + id); //setting action to differentiate each PN
        PendingIntent PNPI = PendingIntent.getBroadcast(appPNClickActivity, 0, PNIntent, PendingIntent.FLAG_UPDATE_CURRENT);
        am.cancel(PNPI);

        //cancel existing matching PN
        NotificationManager WKNM = (NotificationManager) appPNClickActivity.getSystemService(appPNClickActivity.NOTIFICATION_SERVICE);
        WKNM.cancel(id);
    }

}
