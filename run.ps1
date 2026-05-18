$ErrorActionPreference = "Stop"

$qtBin = "C:\Qt\6.9.2\mingw_64\bin"
$mingwBin = "C:\Qt\Tools\mingw1310_64\bin"
$exe = Join-Path $PSScriptRoot "build-qt\HanoiTowersSlideshow.exe"

if (-not (Test-Path -LiteralPath $exe)) {
    & (Join-Path $PSScriptRoot "build.ps1")
}

$env:Path = "$qtBin;$mingwBin;$env:Path"
& $exe
