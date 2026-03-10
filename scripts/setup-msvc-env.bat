@echo off
setlocal EnableExtensions

rem ============================================================================
rem OpenGLToy / setup-msvc-env.bat
rem
rem Purpose:
rem   1. Check whether the current shell already has a complete MSVC build
rem      environment.
rem   2. If not, automatically locate Visual Studio 2022 and load the developer
rem      command environment.
rem
rem Why check cl / rc / mt:
rem   - cl.exe : compiles C/C++
rem   - rc.exe : compiles Windows resource scripts
rem   - mt.exe : processes manifests
rem
rem If any one of them is missing, CMake may fail during try_compile, resource
rem compilation, or link-time manifest processing.
rem ============================================================================

where cl >nul 2>nul
if %ERRORLEVEL%==0 (
    where rc >nul 2>nul
    if %ERRORLEVEL%==0 (
        where mt >nul 2>nul
        if %ERRORLEVEL%==0 (
            echo [OpenGLToy] Existing MSVC environment detected.
            endlocal & exit /b 0
        )
    )
)

rem vswhere is the official Microsoft tool used to locate Visual Studio installs.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [OpenGLToy] ERROR: vswhere.exe not found. Please install Visual Studio 2022 with the C++ workload.
    endlocal & exit /b 1
)

rem Query the latest Visual Studio 2022 installation that includes the
rem VC.Tools.x86.x64 component.
for /f "usebackq tokens=*" %%I in (`"%VSWHERE%" -latest -version [17.0,18.0^) -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_INSTALL_DIR=%%I"

if not defined VS_INSTALL_DIR (
    echo [OpenGLToy] ERROR: Visual Studio 2022 with VC tools was not found.
    endlocal & exit /b 1
)

rem Prefer VsDevCmd because it prepares a more complete developer environment.
rem If it is not available, fall back to vcvars64.bat.
if exist "%VS_INSTALL_DIR%\Common7\Tools\VsDevCmd.bat" (
    echo [OpenGLToy] Loading VS2022 developer environment: %VS_INSTALL_DIR%
    call "%VS_INSTALL_DIR%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul
) else if exist "%VS_INSTALL_DIR%\VC\Auxiliary\Build\vcvars64.bat" (
    echo [OpenGLToy] Loading VC tools environment: %VS_INSTALL_DIR%
    call "%VS_INSTALL_DIR%\VC\Auxiliary\Build\vcvars64.bat" >nul
) else (
    echo [OpenGLToy] ERROR: Unable to locate VsDevCmd.bat or vcvars64.bat.
    endlocal & exit /b 1
)

where cl >nul 2>nul
if not %ERRORLEVEL%==0 (
    echo [OpenGLToy] ERROR: cl.exe is still unavailable after loading the Visual Studio environment.
    endlocal & exit /b 1
)

where rc >nul 2>nul
if not %ERRORLEVEL%==0 (
    echo [OpenGLToy] ERROR: rc.exe is still unavailable after loading the Visual Studio environment.
    endlocal & exit /b 1
)

where mt >nul 2>nul
if not %ERRORLEVEL%==0 (
    echo [OpenGLToy] ERROR: mt.exe is still unavailable after loading the Visual Studio environment.
    endlocal & exit /b 1
)

echo [OpenGLToy] MSVC environment is ready.
endlocal & exit /b 0