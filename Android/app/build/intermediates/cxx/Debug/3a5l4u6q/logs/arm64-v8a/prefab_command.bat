@echo off
"D:\\Android\\Android Studio\\jbr\\bin\\java" ^
  --class-path ^
  "C:\\Users\\devacc\\.gradle\\caches\\modules-2\\files-2.1\\com.google.prefab\\cli\\2.1.0\\aa32fec809c44fa531f01dcfb739b5b3304d3050\\cli-2.1.0-all.jar" ^
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
  c++_shared ^
  --ndk-version ^
  26 ^
  --output ^
  "C:\\Users\\devacc\\AppData\\Local\\Temp\\agp-prefab-staging9769519145246928248\\staged-cli-output" ^
  "C:\\Users\\devacc\\.gradle\\caches\\transforms-4\\e30f927a99e95a5b411a14cd922413d2\\transformed\\jetified-games-activity-2.0.2\\prefab" ^
  "C:\\Users\\devacc\\.gradle\\caches\\transforms-4\\c63bf5ce74ef4599b64d0fbe178d9f41\\transformed\\jetified-oboe-1.9.3\\prefab"
