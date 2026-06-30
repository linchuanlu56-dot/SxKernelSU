package me.weishu.kernelsu.ui.component

import androidx.compose.animation.core.LinearEasing
import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.animateFloat
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.Path
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.graphics.drawscope.Fill
import kotlin.math.cos
import kotlin.math.sin
import kotlin.random.Random

private val starColors = listOf(
    Color(0xFFFF6B9D).copy(alpha = 0.5f),
    Color(0xFFC44AFF).copy(alpha = 0.45f),
    Color(0xFFFFD700).copy(alpha = 0.4f),
    Color(0xFFFFB6C1).copy(alpha = 0.5f),
    Color(0xFF88DDFF).copy(alpha = 0.4f),
    Color(0xFFFF9CA8).copy(alpha = 0.5f),
)

/**
 * ✨ Anime floating sparkles background overlay
 */
@Composable
fun SparkleBackground(
    modifier: Modifier = Modifier,
    starCount: Int = 24,
) {
    val transition = rememberInfiniteTransition(label = "sparkle")
    val phase by transition.animateFloat(
        initialValue = 0f,
        targetValue = (2 * Math.PI).toFloat(),
        animationSpec = infiniteRepeatable(
            animation = tween(8000, easing = LinearEasing),
            repeatMode = RepeatMode.Restart
        ),
        label = "phase"
    )

    Canvas(modifier = modifier.fillMaxSize()) {
        val random = Random(42)
        val stars = List(starCount) { i ->
            val x = random.nextFloat() * size.width
            val y = random.nextFloat() * size.height
            val size2 = (2f + random.nextFloat() * 4f)
            val speed = 0.5f + random.nextFloat() * 1.5f
            val angle = random.nextFloat() * (2 * Math.PI).toFloat()
            val color = starColors[i % starColors.size]
            Triple(Offset(x, y), size2 to speed, color to angle)
        }

        stars.forEach { (pos, props, colorInfo) ->
            val (starSize, speed) = props
            val (color, baseAngle) = colorInfo
            val twinkle = (sin(phase * speed + baseAngle) + 1f) / 2f
            val alpha = 0.1f + twinkle * 0.5f

            drawStar(
                center = pos,
                radius = starSize * (0.8f + twinkle * 0.4f),
                color = color.copy(alpha = alpha),
                rotation = phase * speed * 30f
            )
        }
    }
}

private fun DrawScope.drawStar(
    center: Offset,
    radius: Float,
    color: Color,
    rotation: Float,
    points: Int = 4,
) {
    if (radius < 0.5f) return
    val path = Path()
    val rotRad = Math.toRadians(rotation.toDouble())
    val outerR = radius
    val innerR = radius * 0.4f

    for (i in 0 until points * 2) {
        val angle = rotRad + i * Math.PI / points
        val r = if (i % 2 == 0) outerR else innerR
        val x = center.x + (r * cos(angle)).toFloat()
        val y = center.y + (r * sin(angle)).toFloat()

        if (i == 0) path.moveTo(x, y)
        else path.lineTo(x, y)
    }
    path.close()

    drawPath(path, color, style = Fill)
}
