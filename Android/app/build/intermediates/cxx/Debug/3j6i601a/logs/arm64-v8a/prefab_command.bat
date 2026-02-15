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
  c++_static ^
  --ndk-version ^
  26 ^
  --output ^
  "C:\\Users\\devacc\\AppData\\Local\\Temp\\agp-prefab-staging6587239277523509539\\staged-cli-output" ^
  "C:\\Users\\devacc\\.gradle\\caches\\transforms-4\\e32ed7c37b1556c25d8771d5019236d4\\transformed\\games-activity-3.0.0\\prefab" ^
  "C:\\Users\\devacc\\.gradle\\caches\\transforms-4\\b0b5ae9ae8e2085b572ad9fe03ca41a5\\transformed\\oboe-1.9.3\\prefab"
