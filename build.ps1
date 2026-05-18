$ErrorActionPreference = "Stop"

$qtRoot = "C:\Qt\6.9.2\mingw_64"
$mingwBin = "C:\Qt\Tools\mingw1310_64\bin"
$ninjaBin = "C:\Qt\Tools\Ninja"
$cmake = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$ninja = "C:\Qt\Tools\Ninja\ninja.exe"

$env:Path = "$mingwBin;$ninjaBin;$env:Path"

& $cmake -S . -B build-qt -G Ninja `
    -DCMAKE_MAKE_PROGRAM="$ninja" `
    -DCMAKE_CXX_COMPILER="$mingwBin\g++.exe" `
    -DCMAKE_PREFIX_PATH="$qtRoot"

& $cmake --build build-qt
