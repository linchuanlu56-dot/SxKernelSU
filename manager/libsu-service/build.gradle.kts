@file:Suppress("UnstableApiUsage")

plugins {
    alias(libs.plugins.agp.lib)
}

val androidCompileSdkVersion: Int by rootProject.extra
val androidCompileSdkVersionMinor: Int by rootProject.extra
val androidBuildToolsVersion: String by rootProject.extra
val androidMinSdkVersion: Int by rootProject.extra
val androidSourceCompatibility: JavaVersion by rootProject.extra
val androidTargetCompatibility: JavaVersion by rootProject.extra

android {
    namespace = "com.topjohnwu.superuser.ipc"

    compileSdk {
        version = release(androidCompileSdkVersion) {
            minorApiLevel = androidCompileSdkVersionMinor
        }
    }
    buildToolsVersion = androidBuildToolsVersion

    defaultConfig {
        minSdk = androidMinSdkVersion
    }

    buildFeatures {
        aidl = true
        buildConfig = false
    }

    compileOptions {
        sourceCompatibility = androidSourceCompatibility
        targetCompatibility = androidTargetCompatibility
    }
}

dependencies {
    compileOnly("androidx.annotation:annotation:1.6.0")
    api(libs.com.github.topjohnwu.libsu.core)
}
