load(":config.bzl", "os_name", "android_version", "ndk_root", "compilers_root", "clang_version")

global_cflags = [
  "-fpic",
  "-Os",
  "-Wall",
  "-c",
  "-D ANDROID",
  "-D ANDROID_FULLSCREEN",
  "-D DEBUG",
  "-D PLATFORM=\"ANDROID\"",
  "-DVK_USE_PLATFORM_ANDROID_KHR",
  "-I" + ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/usr/include/android"
]

global_ldflags = [
  "-shared",
  "-s",
  "-lm",
  "-landroid",
  "-llog",
  "-L" + ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/usr/lib/aarch64-linux-android/" + android_version
]

global_includes = [
  ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/usr/include/android",
  ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/usr/include",
  ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/usr/local/include",
  ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/usr/include/c++/v1",
  ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/lib64/clang/" + clang_version + "/include",
]