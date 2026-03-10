#!/usr/bin/env bash
set -euo pipefail

# ==============================================================================
# OpenGLToy / bootstrap-vcpkg.sh
#
# Purpose:
#   1. Reuse VCPKG_ROOT from the outer environment when possible.
#   2. If no usable vcpkg exists, clone and bootstrap one under extern/vcpkg
#      inside the project.
#   3. Print the final VCPKG_ROOT so the caller can verify which vcpkg instance
#      is being used.
#
# Important note:
#   An exported variable in a shell script only affects the current process and
#   its child processes.
#
#   If this script is executed as a child process, the parent shell will not
#   automatically inherit the exported variable. For this reason, the Linux build
#   script typically bootstraps vcpkg and then calls CMake inside the same shell
#   flow.
# ==============================================================================

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LOCAL_VCPKG_ROOT="${ROOT_DIR}/extern/vcpkg"

# Reuse an externally prepared vcpkg first when it is already valid.
if [[ -n "${VCPKG_ROOT:-}" && -f "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" ]]; then
    echo "[OpenGLToy] Reusing VCPKG_ROOT=${VCPKG_ROOT}"
    exit 0
fi

# If the project-local vcpkg directory does not exist yet, clone it.
if [[ ! -d "${LOCAL_VCPKG_ROOT}" ]]; then
    echo "[OpenGLToy] Cloning vcpkg into ${LOCAL_VCPKG_ROOT}"
    git clone https://github.com/microsoft/vcpkg.git "${LOCAL_VCPKG_ROOT}"
fi

# If the vcpkg frontend has not been generated yet, bootstrap it now.
if [[ ! -x "${LOCAL_VCPKG_ROOT}/vcpkg" ]]; then
    echo "[OpenGLToy] Bootstrapping vcpkg"
    "${LOCAL_VCPKG_ROOT}/bootstrap-vcpkg.sh" -disableMetrics
fi

export VCPKG_ROOT="${LOCAL_VCPKG_ROOT}"
echo "[OpenGLToy] VCPKG_ROOT=${VCPKG_ROOT}"