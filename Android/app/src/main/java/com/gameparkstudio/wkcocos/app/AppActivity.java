package com.gameparkstudio.wkcocos.app;

//import android.content.pm.ApplicationInfo;
//import android.content.pm.PackageManager;
//import android.support.v7.app.ActionBarActivity;
//import android.os.Bundle;
//import android.view.Menu;
//import android.view.MenuItem;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;

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

    @Override protected void onPause()
    {
        super.onPause();
        ServiceManager.getInstance().onPause();
        NM=(NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
        NotificationCompat.Builder mBuilder =
                new NotificationCompat.Builder(this)
                        .setSmallIcon(android.R.drawable.stat_notify_more) //what are available options to set here?
                        .setContentTitle("WkCocos notification")
                        .setContentText("Some notification text");
        PendingIntent pending=PendingIntent.getActivity(
                getApplicationContext(), 0, new Intent(), 0);
        mBuilder.setContentIntent(pending);
        NM.notify(0, mBuilder.build());
    }

    @Override protected void onResume() { ServiceManager.getInstance().onResume(); super.onResume(); }

}
