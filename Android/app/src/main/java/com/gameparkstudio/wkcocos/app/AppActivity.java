package com.gameparkstudio.wkcocos.app;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHandler;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxVideoHelper;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.soomla.cocos2dx.common.ServiceManager;
import com.soomla.cocos2dx.store.StoreService;

public class AppActivity extends Cocos2dxActivity {

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

    @Override protected void onPause() { super.onPause(); ServiceManager.getInstance().onPause(); }

    @Override protected void onResume() { ServiceManager.getInstance().onResume(); super.onResume(); }

}
