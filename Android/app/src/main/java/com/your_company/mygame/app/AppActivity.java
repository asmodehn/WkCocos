package com.your_company.mygame.app;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {

    //we override this because we need to load 2 shared libraries
    @Override
    protected void onLoadNativeLibraries() {
        try {
            ApplicationInfo ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;
            try {
                //String libName = bundle.getString("android.app.lib_name");
                //System.loadLibrary(libName);
                String appName = bundle.getString("android.app.app_name");
                System.loadLibrary(appName);
            } catch (Exception e) {
                // ERROR
                e.printStackTrace();
            }
        } catch (PackageManager.NameNotFoundException e) {
            // ERROR
            e.printStackTrace();
        }
    }

}
