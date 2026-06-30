package com.sxkernelsu.ui.component

import androidx.compose.animation.core.Spring
import androidx.compose.animation.core.spring
import androidx.compose.animation.core.tween
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

// ✨ Anime-inspired animation specs
object AnimeAnim {
    val bouncy = spring<Float>(
        dampingRatio = Spring.DampingRatioMediumBouncy,
        stiffness = Spring.StiffnessLow
    )
    val springy = spring<Float>(
        dampingRatio = Spring.DampingRatioLowBouncy,
        stiffness = Spring.StiffnessMediumLow
    )
    val fastTween = tween<Float>(durationMillis = 300)
    val smoothTween = tween<Float>(durationMillis = 500)
}

// 🎀 Anime-style card shape dimensions
object AnimeCard {
    val shape = 16.dp
    val smallShape = 12.dp
    val padding = PaddingValues(horizontal = 12.dp, vertical = 16.dp)
    val innerPadding = PaddingValues(14.dp)
}
