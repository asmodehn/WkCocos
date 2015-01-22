package com.asmodehn.wkcocos.lib;


import android.app.Activity;
import android.util.Log;

//This is a singleton. ref : http://howtodoinjava.com/2012/10/22/singleton-design-pattern-in-java
//when used via getInstance(), it will retrieve the context from the main activity
//This context can be used to launch other activities
public class WkJniHelper {

        private static class LazyHolder {
            private static final WkJniHelper INSTANCE = new WkJniHelper();
        }

        public static WkJniHelper getInstance() {
            return LazyHolder.INSTANCE;
        }

        public MainActivity appMainActivity;

        //to receive alarm intents
        PushNotificationsManager pnMgr;

        //Setting the Main Activity upon creation, for later revival via scheduled PN
        public boolean setActivity(MainActivity mainActivity) {
            appMainActivity = mainActivity;

            pnMgr = new PushNotificationsManager(appMainActivity);

            return true;
        }

        public Activity getAppMainActivity(){
            return appMainActivity;
        }

        public PushNotificationsManager getPNMgr(){
            return pnMgr;
        }

        public void showAdBanner(int x, int y)
        {
            appMainActivity.getAd().showAdBanner(x, y);
        }
        public void hideAdBanner()
        {
            appMainActivity.getAd().hideAdBanner();
        }
        public void showInterstitialAd()
    {
        appMainActivity.getAd().showInterstitialAd();
    }
}
