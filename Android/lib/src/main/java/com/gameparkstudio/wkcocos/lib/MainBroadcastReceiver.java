package com.gameparkstudio.wkcocos.lib;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;

public class MainBroadcastReceiver extends BroadcastReceiver {

    public void onReceive(Context context, Intent intent) {
        Intent WK = new Intent(context, MainService.class);
        context.startService(WK);
    }
}