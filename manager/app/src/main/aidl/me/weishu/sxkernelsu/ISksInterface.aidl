// ISksInterface.aidl
package me.weishu.sxkernelsu;

import android.content.pm.PackageInfo;
import rikka.parcelablelist.ParcelableListSlice;

interface ISksInterface {
    ParcelableListSlice<PackageInfo> getPackages(int flags);

    int[] getUserIds();
}