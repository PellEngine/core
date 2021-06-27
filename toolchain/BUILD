package(default_visibility = ["//visibility:public"])
load(":arm_cc_toolchain_config.bzl", "arm_cc_toolchain_config")
load(":armeabi_cc_toolchain_config.bzl", "armeabi_cc_toolchain_config")

filegroup(name = "empty")
arm_cc_toolchain_config(name = "arm_toolchain_config")
armeabi_cc_toolchain_config(name = "armeabi_toolchain_config")

cc_toolchain_suite(
  name = "android_arm64_suite",
  toolchains = {
    "arm": ":arm_toolchain"
  },
)

cc_toolchain_suite(
  name = "android_armeabi_suite",
  toolchains = {
    "armeabi_v7a": ":armeabi_toolchain"
  }
)

cc_toolchain(
  name = "arm_toolchain",
  toolchain_identifier = "arm-toolchain",
  toolchain_config = ":arm_toolchain_config",
  all_files = ":empty",
  compiler_files = ":empty",
  dwp_files = ":empty",
  linker_files = ":empty",
  objcopy_files = ":empty",
  strip_files = ":empty",
  supports_param_files = 0,
)

cc_toolchain(
  name = "armeabi_toolchain",
  toolchain_identifier = "armeabi-toolchain",
  toolchain_config = ":armeabi_toolchain_config",
  all_files = ":empty",
  compiler_files = ":empty",
  dwp_files = ":empty",
  linker_files = ":empty",
  objcopy_files = ":empty",
  strip_files = ":empty",
  supports_param_files = 0,
)