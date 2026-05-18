# Hanoi Towers Slideshow

A small Qt 6 / C++ desktop accessory that shows the shortest solution for the Tower of Hanoi as a slideshow.

## Features

- Disk count from 1 to 16
- Step-by-step forward/back controls
- Auto-play with adjustable speed
- Loop playback
- Always-on-top option
- System / Light / Dark theme selection
- Fixed virtual canvas scaled to the window size
- Lightweight state rendering with `QPainter`

## Requirements

- Qt 6
- CMake
- Ninja
- A C++17 compiler

The included PowerShell scripts are configured for this local Qt layout:

```powershell
C:\Qt\6.9.2\mingw_64
C:\Qt\Tools\mingw1310_64
C:\Qt\Tools\CMake_64
C:\Qt\Tools\Ninja
```

If your Qt installation is elsewhere, edit `build.ps1` and `run.ps1`.

## Build

```powershell
.\build.ps1
```

## Run

```powershell
.\run.ps1
```

## Release Builds

GitHub Actions can build release artifacts for:

- Windows x64 zip
- Linux x86_64 AppImage

Run the `Release` workflow manually from GitHub Actions, or push a version tag:

```powershell
git tag v0.1.0
git push origin v0.1.0
```

ARM64 release artifacts are intentionally left out for now.

## Manual CMake Build

```powershell
$env:Path = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;$env:Path"

C:\Qt\Tools\CMake_64\bin\cmake.exe -S . -B build-qt -G Ninja `
    -DCMAKE_MAKE_PROGRAM="C:\Qt\Tools\Ninja\ninja.exe" `
    -DCMAKE_CXX_COMPILER="C:\Qt\Tools\mingw1310_64\bin\g++.exe" `
    -DCMAKE_PREFIX_PATH="C:\Qt\6.9.2\mingw_64"

C:\Qt\Tools\CMake_64\bin\cmake.exe --build build-qt
```

## Notes

The number of moves grows as `2^n - 1`. At the default speed of one move per second:

- 15 disks: 32,767 moves, about 9 hours
- 16 disks: 65,535 moves, about 18 hours
