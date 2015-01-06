package com.gameparkstudio.wkcocos.testapp;

import com.gameparkstudio.wkcocos.lib.WkDownloaderInfo;

public class DLInfo implements WkDownloaderInfo {

    @Override
    public XAPKFile getMainXAPK() {
        return new XAPKFile(
                40074, // the version of the APK that the file was uploaded against
                // it should be the version code added to the obb filename
                // so it s possible that it doesn't match current BuildConfig.VERSION_CODE
                151642L, // the length of the file in bytes
                false //TMP : ignoring verification
        );

    }

    @Override
    public XAPKFile getPatchXAPK() {
        //return null if no XAPK is needed
        return null;
    }
}
