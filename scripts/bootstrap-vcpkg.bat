@echo off
setlocal EnableExtensions

rem ============================================================================
rem OpenGLToy / bootstrap-vcpkg.bat
rem
rem Purpose:
rem   1. Reuse VCPKG_ROOT from the caller environment when possible.
rem   2. If no usable VCPKG_ROOT exists on the current machine, automatically
rem      prepare a local copy under extern\vcpkg inside the project.
rem   3. Return the final usable VCPKG_ROOT back to the parent batch script.
rem
rem Key point:
rem   - The script must use:
rem         endlocal & set "VCPKG_ROOT=..."
rem     so the variable can escape the local setlocal scope.
rem   - If VCPKG_ROOT is only set inside the local scope, the outer script will
rem     not see it after this script exits, and CMAKE_TOOLCHAIN_FILE will lose
rem     its required path prefix.
rem ============================================================================

rem Normalize the parent directory of this script as the project root.
set "ROOT_DIR=%~dp0.."
for %%I in ("%ROOT_DIR%") do set "ROOT_DIR=%%~fI"

rem The convention for the project-private vcpkg location.
set "LOCAL_VCPKG_ROOT=%ROOT_DIR%\extern\vcpkg"

rem ---------------------------------------------------------------------------
rem Priority #1:
rem Reuse VCPKG_ROOT from the user environment or CI environment when it is
rem already available and valid.
rem
rem If scripts\buildsystems\vcpkg.cmake exists under that root, we treat it as
rem a usable vcpkg installation. This avoids cloning a separate copy for every
rem project.
rem ---------------------------------------------------------------------------
if defined VCPKG_ROOT (
    if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
        echo [OpenGLToy] Reusing VCPKG_ROOT=%VCPKG_ROOT%
        endlocal & set "VCPKG_ROOT=%VCPKG_ROOT%" & exit /b 0
    )
)

rem ---------------------------------------------------------------------------
rem Priority #2:
rem If the outer environment does not provide a valid VCPKG_ROOT, prepare a
rem local copy inside the current project.
rem
rem This gives the project a self-bootstrapping capability: on a new machine,
rem the project can clone and initialize vcpkg by itself.
rem ---------------------------------------------------------------------------
if not exist "%LOCAL_VCPKG_ROOT%" (
    echo [OpenGLToy] Cloning vcpkg into %LOCAL_VCPKG_ROOT%
    git clone https://github.com/microsoft/vcpkg.git "%LOCAL_VCPKG_ROOT%" || exit /b 1
)

rem bootstrap-vcpkg.bat generates vcpkg.exe. If that file does not exist yet,
rem bootstrap has not finished and must be executed now.
if not exist "%LOCAL_VCPKG_ROOT%\vcpkg.exe" (
    echo [OpenGLToy] Bootstrapping vcpkg
    call "%LOCAL_VCPKG_ROOT%\bootstrap-vcpkg.bat" -disableMetrics || exit /b 1
)

rem Return the project-local vcpkg root back to the outer script.
echo [OpenGLToy] VCPKG_ROOT=%LOCAL_VCPKG_ROOT%
endlocal & set "VCPKG_ROOT=%LOCAL_VCPKG_ROOT%" & exit /b 0