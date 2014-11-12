package com.gameparkstudio.wkcocos.lib;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.app.AlarmManager;
import java.lang.System;
import java.util.TreeMap;

//This is a singleton. ref : http://howtodoinjava.com/2012/10/22/singleton-design-pattern-in-java
//when used via getInstance(), it will retrieve the context from the main activity
//This context can be used to launch other activities
public class PushNotificationsManager{

    public TreeMap<Integer, String> WKTitle = new TreeMap<Integer, String>();
    public TreeMap<Integer, String> WKMessage = new TreeMap<Integer, String>();
    public Class<?> appPNClickActivity;

    private static class LazyHolder {
        private static final PushNotificationsManager INSTANCE = new PushNotificationsManager();
    }

    public static PushNotificationsManager getInstance() {
        return LazyHolder.INSTANCE;
    }

    public boolean setActivity(Class pnClickActivity) {
        if ( Activity.class.isAssignableFrom(pnClickActivity)) {
            appPNClickActivity = pnClickActivity;
            return true;
        }
        return false;
    }

    public boolean schedule(long when, String title, String message) {

        long time = System.currentTimeMillis() + when * 1000;
        final int intent_id = (int) time;

        WKTitle.put(intent_id, title);
        WKMessage.put(intent_id, message);

        Intent WKIntent = new Intent(MainActivity.getContext(), MainBroadcastReceiver.class);
        PendingIntent WKPI = PendingIntent.getBroadcast(MainActivity.getContext(), intent_id, WKIntent, 0);

        AlarmManager alarmManager = (AlarmManager)MainActivity.getContext().getSystemService(MainActivity.getContext().ALARM_SERVICE);
        alarmManager.set(AlarmManager.RTC, time, WKPI);

        return true;
    }
	
}