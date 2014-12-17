package com.gameparkstudio.wkcocos.lib;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;

import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;

public class WkDownloaderReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        try {
            DownloaderClientMarshaller.startDownloadServiceIfRequired(context,
                    intent, WkDownloaderService.class);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
    }
}

