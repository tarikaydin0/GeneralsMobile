plugins {
    id("com.android.application")
}

android {
    namespace = "com.generals.mobile"
    compileSdk = 33

    buildFeatures {
        prefab = true
    }
    defaultConfig {
        applicationId = "com.generals.mobile"
        minSdk = 26
        targetSdk = 33
        versionCode = 1
        versionName = "1.0"

        externalNativeBuild {
            cmake {
                arguments("-DANDROID_STL=c++_shared")
            }
        }
        ndk {
            abiFilters.addAll(listOf("arm64-v8a", "x86_64"))
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }
    externalNativeBuild {
        cmake {
            path("../../CMakeLists.txt")
            version = "3.31.6"
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
}

dependencies {
    implementation("androidx.appcompat:appcompat:1.6.1")
    implementation("com.google.android.material:material:1.9.0")
    implementation("androidx.constraintlayout:constraintlayout:2.1.4")
    implementation("androidx.games:games-activity:2.0.2")
    implementation("com.google.oboe:oboe:1.9.3")

    constraints {
        implementation("org.jetbrains.kotlin:kotlin-stdlib:1.8.10") {
            because("kotlin-stdlib-jdk7/8 are now part of kotlin-stdlib")
        }
        implementation("org.jetbrains.kotlin:kotlin-stdlib-jdk7:1.8.10") {
            because("kotlin-stdlib-jdk7 is now a part of kotlin-stdlib")
        }
        implementation("org.jetbrains.kotlin:kotlin-stdlib-jdk8:1.8.10") {
            because("kotlin-stdlib-jdk8 is now a part of kotlin-stdlib")
        }
    }
}

configurations.all {
    resolutionStrategy {
        force("androidx.activity:activity:1.7.2")
        force("androidx.activity:activity-ktx:1.7.2")
    }
}
