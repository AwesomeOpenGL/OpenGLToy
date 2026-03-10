@echo off
setlocal EnableExtensions

rem ============================================================================
rem OpenGLToy / build-windows.bat
rem
rem Purpose:
rem   1. Provide a unified configure + build entry on Windows.
rem   2. Automatically prepare vcpkg.
rem   3. Automatically select a suitable generator (VS2022 by default).
rem   4. Automatically prepare the MSVC developer environment when Ninja is used.
rem
rem Arguments:
rem   %1 = CONFIG    (Debug / Release), default: Debug
rem   %2 = TRIPLET   (for example x64-windows), default: x64-windows
rem   %3 = GENERATOR (for example "Visual Studio 17 2022" / Ninja),
rem                   default: Visual Studio 17 2022
rem
rem Design principle:
rem   - Prefer out-of-the-box usability, so the default generator is the Visual
rem     Studio generator rather than Ninja.
rem   - Only when the user explicitly requests Ninja do we load vcvars / VsDevCmd
rem     to make sure cl.exe / rc.exe / mt.exe are available.
rem ============================================================================

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Debug"

set "TRIPLET=%~2"
if "%TRIPLET%"=="" set "TRIPLET=x64-windows"

set "GENERATOR=%~3"
if "%GENERATOR%"=="" set "GENERATOR=Visual Studio 17 2022"

rem Normalize the project root and the build directory.
set "ROOT_DIR=%~dp0.."
for %%I in ("%ROOT_DIR%") do set "ROOT_DIR=%%~fI"

set "BUILD_DIR=%ROOT_DIR%\out\build\windows-%CONFIG%"
set "LOCAL_VCPKG_ROOT=%ROOT_DIR%\extern\vcpkg"

rem Make sure vcpkg is available and obtain a valid VCPKG_ROOT.
call "%ROOT_DIR%\scripts\bootstrap-vcpkg.bat" || exit /b 1
if not defined VCPKG_ROOT set "VCPKG_ROOT=%LOCAL_VCPKG_ROOT%"

if not exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo [OpenGLToy] ERROR: vcpkg toolchain file was not found at %VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
    exit /b 1
)

echo [OpenGLToy] Generator=%GENERATOR%
echo [OpenGLToy] Triplet=%TRIPLET%
echo [OpenGLToy] BuildDir=%BUILD_DIR%

set "EXTRA_ARGS="

echo %GENERATOR% | find /I "Ninja" >nul
if %ERRORLEVEL%==0 (
    rem ------------------------------------------------------------------------
    rem The Ninja generator does not automatically select a compiler environment
    rem the way the Visual Studio generator does.
    rem
    rem Therefore we must explicitly load the Visual Studio 2022 developer
    rem environment so that the following tools are available:
    rem   - cl.exe : C/C++ compiler
    rem   - rc.exe : Windows resource compiler
    rem   - mt.exe : manifest tool
    rem ------------------------------------------------------------------------
    call "%ROOT_DIR%\scripts\setup-msvc-env.bat" || exit /b 1
) else (
    echo %GENERATOR% | find /I "Visual Studio" >nul
    if %ERRORLEVEL%==0 (
        rem The Visual Studio generator needs an explicit target architecture,
        rem otherwise it may fall back to Win32 on some setups.
        set "EXTRA_ARGS=-A x64"
    )
)

rem ---------------------------------------------------------------------------
rem Meaning of cmake --fresh:
rem   - Delete the old cache so stale compiler paths, stale toolchain paths, or
rem     previously failed configuration results do not keep polluting this run.
rem   - This is very helpful when diagnosing CMake or compiler selection issues.
rem
rem We explicitly pass:
rem   - CMAKE_TOOLCHAIN_FILE:
rem       tells CMake to attach the vcpkg toolchain during the configure phase.
rem   - VCPKG_TARGET_TRIPLET:
rem       tells vcpkg which target ABI/platform the dependencies are prepared for.
rem
rem Once the toolchain is attached, find_package(glfw3 / fmt / glm / ...) will
rem resolve packages through the vcpkg-managed prefix instead of requiring manual
rem glfw3_DIR or similar package-root variables.
rem ---------------------------------------------------------------------------
cmake --fresh -S "%ROOT_DIR%" -B "%BUILD_DIR%" ^
    -G "%GENERATOR%" %EXTRA_ARGS% ^
    -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET=%TRIPLET% || exit /b 1

rem Enter the actual build phase:
rem   - compile this project's own source code
rem   - link against third-party headers/libraries/imported targets prepared by vcpkg
cmake --build "%BUILD_DIR%" --config %CONFIG% --parallel || exit /b 1
exit /b 0