package com.gameparkstudio.wkcocos.lib;

import android.app.Activity;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

//This is a singleton. ref : http://howtodoinjava.com/2012/10/22/singleton-design-pattern-in-java
//when used via getInstance(), it will retrieve the context from the main activity
//This context can be used to launch other activities
public class PushNotificationsManager{

String TAG = "wkcocosPN";
protected boolean mDebugEnabled = true;

/*** Singleton design with MainActivity Context Access ***/
	//managing serialization versions
	private static final long serialVersionUID = 1L;

	//clean singleton implementation
	private static class LazyHolder
	{
		private static final PushNotificationsManager INSTANCE = new PushNotificationsManager();
	}

	private Context appContext;
    private Class<?> appPNClickActivity;
    private int appIcon;

	private PushNotificationsManager()
	{
	    TAG += "/" + getClass().getSimpleName();
		appContext = MainActivity.getContext();
	}
	
	public static PushNotificationsManager getInstance()
	{
		return LazyHolder.INSTANCE;
	}
	
	//this avoids issues with serialization
	protected Object readResolve()
	{
		return getInstance();
	}
/*** ***/

    public boolean setActivity(Class pnClickActivity)
    {
        if ( Activity.class.isAssignableFrom(pnClickActivity)) {
            appPNClickActivity = pnClickActivity;
            return true;
        }
        return false;
    }

    public void setIcon(int drawable)
    {
        appIcon = drawable;
    }

    public boolean schedule(long when, String title, String message)
    {
        debugLog("Calling schedule with when =" + when + " title = " + title + " message = " + message );

        NotificationManager WKNM=(NotificationManager)appContext.getSystemService(Context.NOTIFICATION_SERVICE);
        NotificationCompat.Builder WKBuilder =
                new NotificationCompat.Builder(appContext)
                        .setSmallIcon(appIcon)
                        .setContentTitle(title)
                        .setContentText(message)
                        .setWhen(when);

        Intent WKIntent = new Intent(appContext, appPNClickActivity );
        PendingIntent WKPI=PendingIntent.getActivity(
                appContext, 0, WKIntent, 0);
        WKBuilder.setContentIntent(WKPI);
        WKNM.notify(0, WKBuilder.build());

        return true;
    }
	
	protected void debugLog(String message) {
        if (mDebugEnabled) {
            Log.d(TAG, message);
        }
    }
}
