package com.asmodehn.wkcocos.lib;

import com.google.android.vending.expansion.downloader.impl.DownloaderService;

public class WkDownloaderService extends DownloaderService {
    // You must use the public key belonging to your publisher account
    public static String BASE64_PUBLIC_KEY;
    // You should also modify this salt
    public static byte[] SALT;

    public static void setPublicKey(String BASE64_PUBLIC_KEY) {
        WkDownloaderService.BASE64_PUBLIC_KEY = BASE64_PUBLIC_KEY;
    }

    @Override
    public String getPublicKey() {
        return BASE64_PUBLIC_KEY;
    }

    public static void setSALT(byte[] SALT) {
        WkDownloaderService.SALT = SALT;
    }

    @Override
    public byte[] getSALT() {
        return SALT;
    }

    @Override
    public String getAlarmReceiverClassName() {
        return WkDownloaderReceiver.class.getName();
    }
}
