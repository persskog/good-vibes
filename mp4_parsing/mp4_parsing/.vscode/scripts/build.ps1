Param(
    [string]$Solution = "mp4_parsing.vcxproj",
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$pf86 = ${env:ProgramFiles(x86)}
$possible = @(
    "$pf86\Microsoft Visual Studio\Installer\vswhere.exe",
    "$env:ProgramFiles\Microsoft Visual Studio\Installer\vswhere.exe",
    "vswhere.exe"
)

$vswhere = $possible | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $vswhere) {
    Write-Error "vswhere.exe not found. Install Visual Studio or add vswhere to PATH."
    exit 1
}

$inst = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath 2>$null
if (-not $inst) {
    Write-Error "No Visual Studio installation with MSBuild found."
    exit 1
}

$vsdev = Join-Path $inst 'Common7\Tools\VsDevCmd.bat'
if (-not (Test-Path $vsdev)) {
    Write-Error "VsDevCmd.bat not found at $vsdev"
    exit 1
}

$msbuildCmd = "msbuild `"$Solution`" /p:Configuration=$Configuration /p:Platform=$Platform /m /restore"
$cmd = "call `"$vsdev`" -arch=amd64 && $msbuildCmd"

Write-Host "Running: $cmd"
$proc = Start-Process -FilePath cmd.exe -ArgumentList '/c', $cmd -NoNewWindow -Wait -PassThru
exit $proc.ExitCode
