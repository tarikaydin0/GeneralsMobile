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
  "C:\\Users\\devacc\\AppData\\Local\\Temp\\agp-prefab-staging563951805087717876\\staged-cli-output" ^
  "C:\\Users\\devacc\\.gradle\\caches\\transforms-4\\12c6d70a655c41cdc47d69d9061cf9ba\\transformed\\games-activity-3.0.0\\prefab" ^
  "C:\\Users\\devacc\\.gradle\\caches\\transforms-4\\fb7ebd35f1e18b19bf4a866044f1eb64\\transformed\\oboe-1.9.3\\prefab"
