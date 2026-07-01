package com.sxkernelsu.ui.component.uninstalldialog

import androidx.compose.runtime.Composable
import com.sxkernelsu.ui.LocalUiMode
import com.sxkernelsu.ui.UiMode

@Composable
fun UninstallDialog(
    show: Boolean,
    onDismissRequest: () -> Unit
) {
    when (LocalUiMode.current) {
        UiMode.Miuix -> UninstallDialogMiuix(show, onDismissRequest)
        UiMode.Material -> UninstallDialogMaterial(show, onDismissRequest)
    }
}
