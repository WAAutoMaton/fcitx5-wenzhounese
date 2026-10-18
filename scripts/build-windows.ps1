[CmdletBinding(SupportsShouldProcess, ConfirmImpact = 'Low')]
param(
    [Parameter(Mandatory)][string]$CorePrefix,
    [string]$WindowsSourceDirectory = '',
    [string]$SourceDirectory = '',
    [string]$BuildRoot = '',
    [string]$Prefix = '',
    [string]$MSYS2Root = 'C:/msys64',
    [string]$DependencyPrefix = '',
    [ValidateRange(1, 256)][int]$Jobs = 6
)

$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
if (!$SourceDirectory) { $SourceDirectory = $root }
if (!$BuildRoot) { $BuildRoot = Join-Path $root 'build/windows' }
if (!$WindowsSourceDirectory) { $WindowsSourceDirectory = Join-Path (Split-Path $root -Parent) 'fcitx5-windows' }
$WindowsSourceDirectory = (Resolve-Path -LiteralPath $WindowsSourceDirectory).Path.Replace('\', '/')
if (!$DependencyPrefix) { $DependencyPrefix = Join-Path $WindowsSourceDirectory 'build/deps/clang64' }
$toolchain = "$WindowsSourceDirectory/cmake/windows-addon-toolchain.cmake"
if (!(Test-Path -LiteralPath $toolchain -PathType Leaf)) {
    throw 'WindowsSourceDirectory must contain the generic Windows addon toolchain.'
}
$CorePrefix = (Resolve-Path -LiteralPath $CorePrefix).Path.Replace('\', '/')
$SourceDirectory = (Resolve-Path -LiteralPath $SourceDirectory).Path.Replace('\', '/')
$BuildRoot = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($BuildRoot).Replace('\', '/')
if (!$Prefix) { $Prefix = "$BuildRoot/package" }
$Prefix = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Prefix).Replace('\', '/')
$DependencyPrefix = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($DependencyPrefix).Replace('\', '/')
if (!(Test-Path -LiteralPath "$CorePrefix/lib/cmake/LibIMECore/LibIMECoreConfig.cmake") -or
    !(Test-Path -LiteralPath "$CorePrefix/lib/cmake/Fcitx5Core/Fcitx5CoreConfig.cmake")) {
    throw 'CorePrefix must contain the matching installed Core/libime SDK, not just runtime DLLs.'
}
if ($Prefix -ieq $CorePrefix -or $BuildRoot -ieq $CorePrefix) {
    throw 'Use a separate plugin prefix and build directory.'
}
if (!$PSCmdlet.ShouldProcess($Prefix, "Build the independent Wenzhounese plugin using SDK '$CorePrefix'")) { return }

function Invoke-PluginCommand([string]$Executable, [string[]]$Arguments) {
    & $Executable @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Executable failed ($LASTEXITCODE): $($Arguments -join ' ')" }
}

$oldPath = $env:PATH
$oldRoot = $env:MSYS2_ROOT
$oldPkgConfig = $env:PKG_CONFIG_PATH
try {
    $sysroot = "$MSYS2Root/clang64"
    $env:MSYS2_ROOT = $MSYS2Root
    $env:PATH = "$CorePrefix/bin;$BuildRoot/bin;$DependencyPrefix/bin;$sysroot/bin;$MSYS2Root/usr/bin;$oldPath"
    $env:PKG_CONFIG_PATH = "$DependencyPrefix/lib/pkgconfig;$sysroot/lib/pkgconfig"
    Invoke-PluginCommand 'cmake' @('-S', $SourceDirectory, '-B', $BuildRoot, '-G', 'Ninja',
        "-DCMAKE_TOOLCHAIN_FILE=$toolchain",
        "-DFCITX5_WINDOWS_SDK=$CorePrefix", "-DWINDOWS_DEPENDENCY_PREFIX=$DependencyPrefix",
        '-DARCH=AMD64', '-DCMAKE_BUILD_TYPE=Release', '-DENABLE_TEST=OFF',
        "-DCMAKE_INSTALL_PREFIX=$Prefix", '-DCMAKE_INSTALL_LIBDIR=lib',
        "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$BuildRoot/bin", "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=$BuildRoot/bin",
        "-DCMAKE_C_COMPILER=$sysroot/bin/clang.exe", "-DCMAKE_CXX_COMPILER=$sysroot/bin/clang++.exe",
        "-DCMAKE_MAKE_PROGRAM=$sysroot/bin/ninja.exe", '-DCMAKE_CXX_FLAGS=-fexperimental-library')
    Invoke-PluginCommand 'cmake' @('--build', $BuildRoot, '-j', "$Jobs")
    Invoke-PluginCommand 'cmake' @('--install', $BuildRoot)
    foreach ($file in @('bin/libIMEWenzhounese.dll', 'lib/fcitx5/libwenzhounese.dll',
            'share/fcitx5/addon/wenzhounese.conf', 'share/fcitx5/inputmethod/wenzhounese.conf',
            'share/libime/wenzhounese.dict', 'lib/libime/wenzhounese.lm', 'lib/libime/wenzhounese.lm.predict')) {
        if (!(Test-Path -LiteralPath "$Prefix/$file" -PathType Leaf)) { throw "Missing plugin output: $file" }
    }
    Write-Output "Wenzhounese plugin ready: $Prefix"
    Write-Output 'Copy bin, lib and share into plugins/wenzhounese under Core or %LOCALAPPDATA%/Fcitx5, then restart Core and enable Wenzhounese in Settings.'
} finally {
    $env:PATH = $oldPath
    $env:MSYS2_ROOT = $oldRoot
    $env:PKG_CONFIG_PATH = $oldPkgConfig
}
