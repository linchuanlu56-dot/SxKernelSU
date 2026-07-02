// ISksInterface.aidl
package com.sxkernelsu;

import android.content.pm.PackageInfo;
import rikka.parcelablelist.ParcelableListSlice;

interface ISksInterface {
    ParcelableListSlice<PackageInfo> getPackages(int flags);

    int[] getUserIds();
}