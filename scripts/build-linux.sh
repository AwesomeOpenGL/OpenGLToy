#!/usr/bin/env bash
set -euo pipefail

# ==============================================================================
# OpenGLToy / build-linux.sh
#
# Purpose:
#   1. Provide a unified configure + build entry on Linux.
#   2. Prepare the vcpkg toolchain within the current script flow.
#   3. Invoke CMake to configure and build the project.
# ==============================================================================

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CONFIG="${1:-Debug}"
TRIPLET="${2:-x64-linux}"
GENERATOR="${3:-Ninja}"
BUILD_DIR="${ROOT_DIR}/out/build/linux-${CONFIG}"

# If the current shell does not already have a valid VCPKG_ROOT, bootstrap a
# local vcpkg instance first.
#
# This reduces new-machine setup cost because the user does not need to export
# VCPKG_ROOT in advance.
if [[ -z "${VCPKG_ROOT:-}" || ! -f "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" ]]; then
    echo "[OpenGLToy] VCPKG_ROOT is not set or invalid. Running local bootstrap..."
    # shellcheck disable=SC1091
    source "${ROOT_DIR}/scripts/bootstrap-vcpkg.sh"
fi

# Configure phase:
#   - attach the vcpkg toolchain
#   - make find_package(...) search the vcpkg-managed prefix path
#   - generate imported targets for the current triplet
cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
    -G "${GENERATOR}" \
    -DCMAKE_BUILD_TYPE="${CONFIG}" \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET="${TRIPLET}"

# Build phase:
#   - compile OpenGLToy's own source code
#   - link against third-party library targets prepared by vcpkg
cmake --build "${BUILD_DIR}" --parallel