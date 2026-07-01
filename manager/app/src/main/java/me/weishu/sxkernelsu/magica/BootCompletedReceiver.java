package com.sxkernelsu.magica;

import static com.sxkernelsu.magica.AppZygotePreload.TAG;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import me.weishu.sxkernelsu.ui.util.SksCliKt;

public class BootCompletedReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent == null) {
            return;
        }
        var action = intent.getAction();
        if (!Intent.ACTION_LOCKED_BOOT_COMPLETED.equals(action)
                && !Intent.ACTION_BOOT_COMPLETED.equals(action)
                && !"com.sxkernelsu.magica.LAUNCH".equals(action)) {
            return;
        }
        if (SksCliKt.rootAvailable()) return;
        try {
            context.startService(new Intent(context, MagicaService.class));
            Log.i(TAG, "MagicaService started from boot action: " + action);
        } catch (Throwable e) {

            Log.e(TAG, "Failed to start MagicaService from boot action: " + action, e);
        }
    }
}
