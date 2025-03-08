@echo off
rem
rem  Master build file. Execute this script to create all output libraries
rem  Environment variable DEV_ROOT points to the root of development tree.
rem

rem VS2017U2 contains vswhere.exe
if "%VSWHERE%"=="" set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
  echo Not found VSWHERE.exe!
  echo Cannot setup environment.
  goto :EOF
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set VSInstallDir=%%i
)

rem setup environment for Visual Studio 2017 and later
call "%VSInstallDir%\common7\tools\vsmsbuildcmd.bat"


if "%~1"=="" (msbuild build.proj) else (msbuild build.proj)
