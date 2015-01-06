package com.gameparkstudio.wkcocos.testapp;

import android.app.NotificationManager;
import android.os.Bundle;
import com.gameparkstudio.wkcocos.lib.MainActivity;
import com.gameparkstudio.wkcocos.lib.WkDownloaderActivity;
import com.google.android.vending.expansion.downloader.Helpers;

public class AppActivity extends MainActivity {

    @Override
    protected void onCreate(final Bundle savedInstanceState) {

        WkDownloaderActivity.setInfo(new DLInfo());

        super.onCreate(savedInstanceState);
    }

    @Override protected void onPause() {
        super.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
    }

    @Override
    public String getVersionName()
    {
        //gradle
        String versionName = BuildConfig.VERSION_NAME;
        return versionName;
    }

    @Override
    public int getVersionCode()
    {
        //gradle // careful : current package !
        int versionCode = BuildConfig.VERSION_CODE;
        return versionCode;
    }

    @Override
    public String getLVLKey()
    {
        return "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAl+3XzvgIDy5FnT47omyrAtNBs3cVLJEq5GQQjHOIiF7xAqPJM5Toi9ejhRJuxqqrPgERLLJgqGO8Lv/tYMMvm2Tona0PE+6+yfkxdMfSIBQnqSHyyyPsnqCO7jXdLYYmn904UwPpamdoaR724RNAfJgcSEtmOBcw760kvcVG62SHeaK0DjWjgouW1XO5lWvvUGlh+tEV8T4e/7LDyXFwRlTA8++HjvTB4xH9fK7AXphJLLMuEEwNczBpBkztk8mW+6DAwfDESGz2hRP/XyvYZGuH9T/9H6BFabmXxSUuIIZZeVycF1/2U3rOOBh19E2xTtCeMCG/QvYc4Rk2EeoqaQIDAQAB";
    }

    @Override
    public byte[] getSALT()
    {
        return new byte[] { 1, 42, -12, -1, 54, 98, -100, -12, 43, 2, -8, -4, 9, 5, -106, -42, -33, 45, -1, 84 };
    }

}
