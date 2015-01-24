package com.asmodehn.wkcocos.lib;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.igaworks.IgawCommon;
import com.igaworks.displayad.IgawDisplayAd;
import com.igaworks.displayad.common.DAErrorCode;
import com.igaworks.displayad.interfaces.IBannerEventCallbackListener;
import com.igaworks.displayad.interfaces.IInterstitialEventCallbackListener;
import com.igaworks.displayad.view.BannerContainerView;

/**
 * Manage Advertisement library
 */
public class WkAd implements IBannerEventCallbackListener, IInterstitialEventCallbackListener {
    private MainActivity mainActivity;
    private String bannerKey;
    private String interstitialKey;

    android.widget.LinearLayout bannerLayout;
    private android.widget.RelativeLayout adTopLayout;
    private BannerContainerView bannerView;
    // Need handler for callbacks to the UI thread
    final Handler mHandler = new Handler();

    // Create runnable for posting
    final Runnable adBannerShow = new Runnable() {
        public void run() {
            adTopLayout.setVisibility(View.VISIBLE);
            adTopLayout.requestLayout();
        }
    };
    final Runnable adBannerHide = new Runnable() {
        public void run() {
            adTopLayout.setVisibility(View.INVISIBLE);
            adTopLayout.requestLayout();
        }
    };
    final Runnable adInterstitialShow = new Runnable() {
        public void run() {
            IgawDisplayAd.showInterstitialAd(mainActivity, interstitialKey);
        }
    };

    public WkAd(MainActivity activity, String bannerID, String interstitialID)
    {
        mainActivity = activity;
        bannerKey = bannerID;
        interstitialKey = interstitialID;


        adTopLayout = new android.widget.RelativeLayout(mainActivity);
        android.widget.RelativeLayout.LayoutParams param = new android.widget.RelativeLayout.LayoutParams(android.widget.RelativeLayout.LayoutParams.MATCH_PARENT, android.widget.RelativeLayout.LayoutParams.MATCH_PARENT);
        adTopLayout.setLayoutParams(param);
        bannerLayout = new android.widget.LinearLayout(mainActivity);
        param = new android.widget.RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        param.addRule(android.widget.RelativeLayout.ALIGN_PARENT_BOTTOM);
        param.addRule(RelativeLayout.CENTER_HORIZONTAL);
        bannerLayout.setLayoutParams(param);
        adTopLayout.addView(bannerLayout);

        bannerView = new BannerContainerView(mainActivity);
        adTopLayout.setVisibility(View.INVISIBLE);
        bannerLayout.addView(bannerView);

        // Ad init
        IgawCommon.startApplication(mainActivity);
        IgawDisplayAd.init(mainActivity);
        IgawDisplayAd.setBannerEventCallbackListener(mainActivity, bannerKey, this);
        IgawDisplayAd.setInterstitialEventCallbackListener(mainActivity, interstitialKey, this);

        // setup location
        // Acquire a reference to the system Location Manager
        LocationManager locationManager = (LocationManager) mainActivity.getSystemService(Context.LOCATION_SERVICE);

        // Define a listener that responds to location updates
        LocationListener locationListener = new LocationListener() {
            public void onLocationChanged(Location location) {
                IgawDisplayAd.setLocation(location.getLatitude(), location.getLongitude());
            }

            public void onStatusChanged(String provider, int status, Bundle extras) {}

            public void onProviderEnabled(String provider) {}

            public void onProviderDisabled(String provider) {}
        };

        // Register the listener with the Location Manager to receive location updates
        // we dont want to be accurate, its just for damn advertising.
        // so just make an update every 30 minute
        // if there is at least a kilometer difference
        locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 30*60*1000, 1000, locationListener);

    }

    public View getUI()
    {
        return adTopLayout;
    }

    public void pause(){
        IgawCommon.endSession();
        IgawDisplayAd.pauseBannerAd(mainActivity, bannerKey);

    }

    public void resume(){
        IgawCommon.startSession(mainActivity);
        IgawDisplayAd.startBannerAd(mainActivity, bannerKey, bannerView);

    }

    public void destroy(){
        IgawDisplayAd.stopBannerAd(mainActivity);
        IgawDisplayAd.destroy();
    }

    public void showAdBanner(int x, int y) {
        adTopLayout.layout(
                x - adTopLayout.getWidth() / 2,
                y - adTopLayout.getHeight() / 2,
                x + adTopLayout.getWidth() / 2,
                y + adTopLayout.getHeight() / 2
        );
        mHandler.post(adBannerShow);
    }

    public void hideAdBanner() {
        mHandler.post(adBannerHide);
    }

    public void showInterstitialAd()
    {
        mHandler.post(adInterstitialShow);
    }

    @Override
    public void OnBannerAdReceiveSuccess() {
        // TODO Auto-generated method stub
        //Toast.makeText(mainActivity, "OnBannerAdReceiveSuccess", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void OnBannerAdReceiveFailed(DAErrorCode errorCode) {
        // TODO Auto-generated method stub
        //Toast.makeText(mainActivity, "OnBannerAdReceiveFailed: " + errorCode.getErrorMessage(), Toast.LENGTH_SHORT).show();
    }

    @Override
    public void OnInterstitialReceiveSuccess() {
        // TODO Auto-generated method stub
        //Toast.makeText(mainActivity, "OnInterstitialReceiveSuccess", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void OnInterstitialReceiveFailed(DAErrorCode errorCode) {
        // TODO Auto-generated method stub
        //Toast.makeText(mainActivity, "OnInterstitialReceiveFailed", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void OnInterstitialClosed() {
        // TODO Auto-generated method stub
        //Toast.makeText(mainActivity, "OnInterstitialClosed", Toast.LENGTH_SHORT).show();
    }

}