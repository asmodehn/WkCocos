package com.gameparkstudio.wkcocos.lib;

public interface WkDownloaderInfo {
    /**
     * This is a little helper class that demonstrates simple testing of an
     * Expansion APK file delivered by Market. You may not wish to hard-code
     * things such as file lengths into your executable... and you may wish to
     * turn this code off during application development.
     */
    public static class XAPKFile {
        public final boolean mCheckEnabled;
        public final int mFileVersion;
        public final long mFileSize;
        private String mFilePath; //null if file not present, "" if file not needed, otherwise contains the path oft he downloaded file.

        public XAPKFile(int fileVersion, long fileSize,  boolean enableCheck ) {
            mCheckEnabled = enableCheck;
            mFileVersion = fileVersion;
            mFileSize = fileSize;
            mFilePath = ""; //initialized to "" before check can be done by the downloader
        }

        public XAPKFile(int fileVersion, long fileSize){
            this(fileVersion,fileSize,true);
        }

        void setFilePath(String fpath)
        {
            mFilePath = fpath;
        }

        String getFilePath(){
            return mFilePath;
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
    public abstract XAPKFile getMainXAPK();
    public abstract XAPKFile getPatchXAPK();

}
