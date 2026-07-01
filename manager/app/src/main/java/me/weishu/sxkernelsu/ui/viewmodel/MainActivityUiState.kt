package com.sxkernelsu.ui.viewmodel

import androidx.compose.runtime.Immutable
import com.sxkernelsu.ui.UiMode
import com.sxkernelsu.ui.theme.AppSettings

@Immutable
data class MainActivityUiState(
    val appSettings: AppSettings,
    val pageScale: Float,
    val enableBlur: Boolean,
    val enableFloatingBottomBar: Boolean,
    val enableFloatingBottomBarBlur: Boolean,
    val uiMode: UiMode,
)
