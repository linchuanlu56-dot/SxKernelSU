package me.weishu.kernelsu.ui.theme

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb

// Anime / kawaii color palette ✨
val animeColorOptions = listOf(
    Color(0xFFFF6B9D).toArgb(),  // Sakura pink
    Color(0xFFC44AFF).toArgb(),  // Neon purple
    Color(0xFFFF44AA).toArgb(),  // Magical girl pink
    Color(0xFFAA66FF).toArgb(),  // Lavender
    Color(0xFF44AAFF).toArgb(),  // Aqua blue (anime sky)
    Color(0xFFFF8888).toArgb(),  // Coral
    Color(0xFFCC88FF).toArgb(),  // Soft violet
    Color(0xFFFFAAAA).toArgb(),  // Light pink
    Color(0xFF88DDFF).toArgb(),  // Pastel blue
    Color(0xFFFFB347).toArgb(),  // Anime orange
    Color(0xFFE8A0FF).toArgb(),  // Pastel purple
    Color(0xFFFF6F91).toArgb(),  // Watermelon
    Color(0xFF7EC8E3).toArgb(),  // Miku blue
    Color(0xFFFFD700).toArgb(),  // Anime gold
    Color(0xFFFF9CA8).toArgb(),  // Soft pink
)

val keyColorOptions = animeColorOptions

// Anime accent colors (used for decorations)
object AnimeColors {
    val sparklePink = Color(0xFFFF6B9D)
    val magicalPurple = Color(0xFFC44AFF)
    val starGold = Color(0xFFFFD700)
    val candyPink = Color(0xFFFFB6C1)
    val mikuTeal = Color(0xFF39C5BB)
    val pastelLavender = Color(0xFFE8C8FF)
    val sakura = Color(0xFFFFB7C5)
    val twilight = Color(0xFF6A0DAD)
}
