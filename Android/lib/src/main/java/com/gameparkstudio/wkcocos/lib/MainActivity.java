package com.gameparkstudio.wkcocos.lib;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.content.Intent;
import android.net.Uri;
import android.os.Messenger;
import android.os.SystemClock;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.android.vending.expansion.zipfile.ZipResourceFile;
import com.google.android.vending.expansion.downloader.Constants;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.soomla.cocos2dx.common.ServiceManager;
import com.soomla.cocos2dx.store.StoreService;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.io.DataInputStream;
import java.io.IOException;
import java.util.zip.CRC32;

public abstract class MainActivity extends Cocos2dxActivity implements IDownloaderClient {

    private final static String TAG = MainActivity.class.getSimpleName();

    private Cocos2dxWebViewHelper mWebViewHelper = null;

    private static Activity me = null;

    private static final String LOG_TAG = "MainActivity";

    /**
     * Determine if the files are present and match the requirements. Free
     * applications should definitely consider doing this, as this allows the
     * application to be launched for the first time without having a network
     * connection present. Paid applications that use LVL should probably do at
     * least one LVL check that requires the network to be present, so this is
     * not as necessary.
     *
     * @return true if they are present.
     */
    abstract boolean expansionFilesDelivered();

    @Override
    protected void onCreate(final Bundle savedInstanceState) {

        //Needed before Cocos start to download all needed assets

        WkDownloaderService.setPublicKey(getLVLKey());
        WkDownloaderService.setSALT(new byte[] { 1, 42, -12, -1, 54, 98, -100, -12, 43, 2, -8, -4, 9, 5, -106, -42, -33, 45, -1, 84 });


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
    public abstract String getVersionName();
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
    public abstract int getVersionCode();
    //{
        //gradle // careful : current package !
        //int vcode = BuildConfig.VERSION_CODE;
        //not gradle
        //int versionCode = getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
        //return versionCode;
    //}

    /**
     * returns the Google play license key
     * needs to be implemented in the main package
     * @return string holding the license key
     */
    public abstract String getLVLKey();

    //TODO : test it ! maybe use new JniHelper for this...
    public static void openURL(String url) {
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        me.startActivity(i);
    }

}
