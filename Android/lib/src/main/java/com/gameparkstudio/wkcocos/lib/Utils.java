package com.gameparkstudio.wkcocos.lib;

import android.webkit.WebSettings;
import android.webkit.WebView;

import java.util.Locale;

public class Utils {
    public static String getVersionName()
    {
        //gradle
        String version = BuildConfig.VERSION_NAME;
        //not gradle
        //String versionName = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
        return version;
    }


    public static int getVersionCode()
    {
        //gradle
        int vcode = BuildConfig.VERSION_CODE;
        //not gradle
        //int versionCode = getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
        return vcode;
    }

    public static void showUrl(String url, WebView wv ,  boolean enable_javascript)
    {
        //TODO : get current activity ?

        if ( enable_javascript ) {
            WebSettings webSettings = wv.getSettings();
            webSettings.setJavaScriptEnabled(true);
        }
        wv.loadUrl(url);
    }

    public static String getLanguage()
    {
        return Locale.getDefault().getLanguage();
    }

}