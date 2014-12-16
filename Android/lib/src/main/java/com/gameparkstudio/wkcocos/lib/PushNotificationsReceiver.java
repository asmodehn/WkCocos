package com.gameparkstudio.wkcocos.lib;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class PushNotificationsReceiver extends BroadcastReceiver{
    @Override
    public void onReceive(Context context, Intent intent)
    {
        Intent pni = new Intent(context, PushNotificationsService.class);
        pni.putExtras(intent.getExtras()); //forward extra arguments
        context.startService(pni);
    }
}
