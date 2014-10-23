package com.gameparkstudio.wkcocos.lib;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;

public class MainReceiver extends BroadcastReceiver {

    public void onReceive(Context context, Intent intent)
    {
        Intent WK = new Intent(context, MainAlarmService.class);
        context.startService(WK);

    }

}