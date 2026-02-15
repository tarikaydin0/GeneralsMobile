@echo off
"D:\\Android\\Android Studio\\jbr\\bin\\java" ^
  --class-path ^
  "C:\\Users\\devacc\\.gradle\\caches\\modules-2\\files-2.1\\com.google.prefab\\cli\\2.0.0\\f2702b5ca13df54e3ca92f29d6b403fb6285d8df\\cli-2.0.0-all.jar" ^
  com.google.prefab.cli.AppKt ^
  --build-system ^
  cmake ^
  --platform ^
  android ^
  --abi ^
  arm64-v8a ^
  --os-version ^
  26 ^
  --stl ^
  c++_static ^
  --ndk-version ^
  25 ^
  --output ^
  "C:\\Users\\devacc\\AppData\\Local\\Temp\\agp-prefab-staging10075042644490599674\\staged-cli-output" ^
  "C:\\Users\\devacc\\.gradle\\caches\\9.0.0\\transforms\\e0ac6f19f7a0afa3f36c3db941ad7ae4\\transformed\\games-activity-3.0.0\\prefab" ^
  "C:\\Users\\devacc\\.gradle\\caches\\9.0.0\\transforms\\0221cfbc4f68b16c8fa1fca4535c0c77\\transformed\\oboe-1.9.3\\prefab"
