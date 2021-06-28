# Build architectures
echo "Building for arm64-v8a..."
echo ""
bazel build //src/pellengine:build_all --config=android_arm64_config
echo ""
echo "Building for armeabi-v7a...\n"
echo ""
bazel build //src/pellengine:build_all --config=android_armeabi_config

# Copy shared libraries to results folder
rm -rf ./build/
mkdir ./build/

copy_shared_libraries() {
  local bazel_arch=$1
  local android_arch=$2
  local bazel_prefix="./bazel-out/$bazel_arch/bin/src/pellengine"
  local android_prefix="./build/android/lib/$android_arch"

  mkdir -p "./build/android/lib/$android_arch/"

  cp -rp "$bazel_prefix/graphics/libgraphics.so" "$android_prefix/libgraphics.so"
  cp -rp "$bazel_prefix/io/libio.so" "$android_prefix/libio.so"
  cp -rp "$bazel_prefix/systems/libsprite_batch.so" "$android_prefix/libsprite_batch.so"
  cp -rp "$bazel_prefix/vulkan/libvulkan.so" "$android_prefix/libvulkan.so"
}

echo ""
echo "Copying build products..."
copy_shared_libraries "arm-fastbuild" "arm64-v8a"
copy_shared_libraries "armeabi_v7a-fastbuild" "armeabi-v7a"

echo "Build succeded!"