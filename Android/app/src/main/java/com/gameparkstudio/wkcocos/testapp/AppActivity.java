package com.gameparkstudio.wkcocos.testapp;

import android.app.NotificationManager;
import android.os.Bundle;
import com.gameparkstudio.wkcocos.lib.MainActivity;
import com.google.android.vending.expansion.downloader.Helpers;

public class AppActivity extends MainActivity {

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
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


    /**
     * This is a little helper class that demonstrates simple testing of an
     * Expansion APK file delivered by Market. You may not wish to hard-code
     * things such as file lengths into your executable... and you may wish to
     * turn this code off during application development.
     */
    private static class XAPKFile {
        public final boolean mIsMain;
        public final int mFileVersion;
        public final long mFileSize;

        XAPKFile(boolean isMain, int fileVersion, long fileSize) {
            mIsMain = isMain;
            mFileVersion = fileVersion;
            mFileSize = fileSize;
        }
    }

    /**
     * Here is where you place the data that the validator will use to determine
     * if the file was delivered correctly. This is encoded in the source code
     * so the application can easily determine whether the file has been
     * properly delivered without having to talk to the server. If the
     * application is using LVL for licensing, it may make sense to eliminate
     * these checks and to just rely on the server.
     */
    private static final XAPKFile[] xAPKS = {
            new XAPKFile(
                    true, // true signifies a main file
                    3, // the version of the APK that the file was uploaded
                    // against
                    687801613L // the length of the file in bytes
            ),
            new XAPKFile(
                    false, // false signifies a patch file
                    4, // the version of the APK that the patch file was uploaded
                    // against
                    512860L // the length of the patch file in bytes
            )
    };

    /**
     * Go through each of the APK Expansion files defined in the structure above
     * and determine if the files are present and match the required size. Free
     * applications should definitely consider doing this, as this allows the
     * application to be launched for the first time without having a network
     * connection present. Paid applications that use LVL should probably do at
     * least one LVL check that requires the network to be present, so this is
     * not as necessary.
     *
     * @return true if they are present.
     */
    @Override
    protected boolean expansionFilesDelivered() {
        //TMP : For now we assume the file we get is always correct.
        //for (XAPKFile xf : xAPKS) {
        //    String fileName = Helpers.getExpansionAPKFileName(this, xf.mIsMain, xf.mFileVersion);
        //    if (!Helpers.doesFileExist(this, fileName, xf.mFileSize, false))
        //        return false;
        //}
        return false;
    }
}
