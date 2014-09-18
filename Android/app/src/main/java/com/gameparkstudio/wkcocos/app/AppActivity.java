package com.gameparkstudio.wkcocos.app;

//import android.content.pm.ApplicationInfo;
//import android.content.pm.PackageManager;
//import android.support.v7.app.ActionBarActivity;
//import android.os.Bundle;
//import android.view.Menu;
//import android.view.MenuItem;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
//import android.view.View;
import android.content.Context;
import android.content.Intent;

//import org.cocos2dx.lib.Cocos2dxHandler;
//import org.cocos2dx.lib.Cocos2dxHelper;
//import org.cocos2dx.lib.Cocos2dxVideoHelper;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.soomla.cocos2dx.common.ServiceManager;
import com.soomla.cocos2dx.store.StoreService;

public class AppActivity extends Cocos2dxActivity {

    NotificationManager NM;

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {

        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);

        // initialize services
        final ServiceManager serviceManager = ServiceManager.getInstance();
        serviceManager.setActivity(this);
        serviceManager.setGlSurfaceView(glSurfaceView);
        serviceManager.registerService(StoreService.getInstance());

        return glSurfaceView;
    }

    @SuppressWarnings("deprecation")
    @Override protected void onPause()
    {
        super.onPause();
        ServiceManager.getInstance().onPause();
        NM=(NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
        Notification notify=new Notification(android.R.drawable.stat_notify_more,
                "WkCocos notification", System.currentTimeMillis());
        PendingIntent pending=PendingIntent.getActivity(
                getApplicationContext(), 0, new Intent(), 0);
        notify.setLatestEventInfo(getApplicationContext(), "New notification",
                "Some notification text", pending);
        NM.notify(0, notify);
    }

    @Override protected void onResume() { ServiceManager.getInstance().onResume(); super.onResume(); }

}
