package com.gameparkstudio.wkcocos.lib;

import android.app.Activity;
import android.os.Bundle;
import android.content.Intent;
import android.net.Uri;

import com.soomla.cocos2dx.common.ServiceManager;
import com.soomla.cocos2dx.store.StoreService;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

public abstract class MainActivity extends Cocos2dxActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    private Cocos2dxWebViewHelper mWebViewHelper = null;

    private static Activity me = null;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        me = this;

        if(mWebViewHelper == null) {
            mWebViewHelper = new Cocos2dxWebViewHelper(mFrameLayout);
        }

        WkJniHelper.getInstance().setActivity(this);
    }

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {

        Cocos2dxGLSurfaceView glSurfaceView = super.onCreateView();

        // initialize services
        final ServiceManager serviceManager = ServiceManager.getInstance();
        serviceManager.setActivity(this);
        serviceManager.setGlSurfaceView(glSurfaceView);
        serviceManager.registerService(StoreService.getInstance());

        return glSurfaceView;
    }

    @Override protected void onPause() {
        super.onPause();
        ServiceManager.getInstance().onPause();
    }

    @Override protected void onResume() {
        ServiceManager.getInstance().onResume();
        super.onResume();
    }

    /**
     * returns the version name of the main packaged app
     * needs to be implemented in the main package
     * @return string holding the main package app version Name
     */
    //public abstract String getVersionName();
    //{
        //gradle
        //String version = BuildConfig.VERSION_NAME;
        //not gradle
        //String versionName = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
        //return versionName;
    //}

    /**
     * returns the version code of the main packaged app
     * needs to be implemented in the main package
     * @return string holding the main package app version Code
     */
    //public abstract int getVersionCode();
    //{
        //gradle // careful : current package !
        //int vcode = BuildConfig.VERSION_CODE;
        //not gradle
        //int versionCode = getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
        //return versionCode;
    //}

    //TODO : test it ! maybe use new JniHelper for this...
    public static void openURL(String url) {
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        me.startActivity(i);
    }

}
