import org.jetbrains.kotlin.gradle.dsl.JvmTarget
import java.util.Properties

plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

// Torch runs on the device to turn the user's ROM into mk64.o2r, and it reads
// its extraction recipes off the filesystem. Ship them as APK assets; the
// launcher unpacks them into the app's external files directory on first run.
val repositoryRoot: File = rootProject.projectDir.parentFile

// Release signing: android/key.properties locally, the environment on CI.
// The file holds passwords, so it is gitignored and never read into the build
// output.
val keystoreProperties = Properties().apply {
    val file = rootProject.file("key.properties")
    if (file.isFile) file.inputStream().use { load(it) }
}

val stageTorchAssets = tasks.register<Sync>("stageTorchAssets") {
    description = "Stages Torch's extraction inputs into the APK assets."
    into(layout.buildDirectory.dir("generated/torchAssets"))
    from(File(repositoryRoot, "config.yml"))
    from(File(repositoryRoot, "yamls")) { into("yamls") }
    from(File(repositoryRoot, "meta")) { into("meta") }
}

// Derived from the task rather than named as a plain path, so every consumer
// picks up the dependency. Naming the directory directly only looks fine until
// something other than the asset merge reads it — release builds run lint-vital
// over the source sets, and that failed on the missing dependency.
val stagedTorchAssets: Provider<File> = stageTorchAssets.map { it.destinationDir }

android {
    namespace = "com.izzy.kart"
    compileSdk = 36
    ndkVersion = "30.0.15729638"

    defaultConfig {
        applicationId = "com.izzy.kart"
        minSdk = 24
        targetSdk = 36
        versionCode = 7
        versionName = "2.0.0"

        ndk {
            abiFilters += listOf("arm64-v8a", "armeabi-v7a")
        }

        externalNativeBuild {
            cmake {
                arguments += listOf(
                    "-DANDROID_STL=c++_shared",
                    "-DUSE_OPENGLES=ON",
                    "-DSDL_SHARED=ON",
                    "-DSDL_STATIC=OFF",
                    "-DHAVE_LD_VERSION_SCRIPT=OFF"
                )
                targets += "Spaghettify"
            }
        }
    }

    signingConfigs {
        create("release") {
            val configuredStore = keystoreProperties.getProperty("storeFile")
            if (configuredStore != null) {
                // A relative path in key.properties reads against the file's own
                // directory, which is what someone editing it would expect.
                storeFile = rootProject.file(configuredStore)
                storePassword = keystoreProperties.getProperty("storePassword")
                keyAlias = keystoreProperties.getProperty("keyAlias")
                keyPassword = keystoreProperties.getProperty("keyPassword")
            } else {
                storeFile = file(System.getenv("KEYSTORE_FILE") ?: "release.keystore")
                storePassword = System.getenv("KEYSTORE_PASSWORD")
                keyAlias = System.getenv("KEY_ALIAS")
                keyPassword = System.getenv("KEY_PASSWORD")
            }
        }
    }

    buildTypes {
        debug {
            isJniDebuggable = true
            applicationIdSuffix = ".debug"
        }
        release {
            // The APK is almost entirely native code and assets, so shrinking
            // the tiny Kotlin layer buys nothing and only risks stripping the
            // classes the JNI bridge looks up by name.
            isMinifyEnabled = false
            signingConfig = signingConfigs.getByName("release")
            ndk {
                debugSymbolLevel = "SYMBOL_TABLE"
            }
        }
    }

    externalNativeBuild {
        cmake {
            path = File(repositoryRoot, "CMakeLists.txt")
            version = "3.30.3"
        }
    }

    sourceSets.named("main") {
        assets.srcDir(stagedTorchAssets)
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    lint {
        abortOnError = false
    }
}

kotlin {
    compilerOptions {
        jvmTarget.set(JvmTarget.JVM_17)
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.13.1")
    implementation("androidx.activity:activity-ktx:1.9.3")
    implementation("androidx.constraintlayout:constraintlayout:2.1.4")
}
