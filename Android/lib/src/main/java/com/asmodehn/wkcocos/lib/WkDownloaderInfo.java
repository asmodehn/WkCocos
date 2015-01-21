package com.asmodehn.wkcocos.lib;

import android.os.Parcel;
import android.os.Parcelable;

public interface WkDownloaderInfo {
    /**
     * This is a little helper class that demonstrates simple testing of an
     * Expansion APK file delivered by Market. You may not wish to hard-code
     * things such as file lengths into your executable... and you may wish to
     * turn this code off during application development.
     */
    public static class XAPKFile implements Parcelable {
        public final boolean mCheckEnabled;
        public final int mFileVersion;
        public final long mFileSize;
        private String mFilePath; //null if file not present, "" if file not needed, otherwise contains the path of the downloaded file.

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

 /* everything below here is for implementing Parcelable */

        // 99.9% of the time you can just ignore this
        public int describeContents() {
            return 0;
        }

        // write your object's data to the passed-in Parcel
        public void writeToParcel(Parcel out, int flags) {
            out.writeByte((byte) (mCheckEnabled? 1:0));
            out.writeInt(mFileVersion);
            out.writeLong(mFileSize);
            out.writeString(mFilePath);
        }

        // this is used to regenerate your object. All Parcelables must have a CREATOR that implements these two methods
        public static final Parcelable.Creator<XAPKFile> CREATOR = new Parcelable.Creator<XAPKFile>() {
            public XAPKFile createFromParcel(Parcel in) {
                return new XAPKFile(in);
            }

            public XAPKFile[] newArray(int size) {
                return new XAPKFile[size];
            }
        };

        // example constructor that takes a Parcel and gives you an object populated with it's values
        private XAPKFile(Parcel in) {
            mCheckEnabled = in.readByte() != 0;
            mFileVersion = in.readInt();
            mFileSize = in.readLong();
            mFilePath = in.readString();
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
