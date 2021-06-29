package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
  name = "libpellengine_vulkan.so",
  linkshared = True,
  srcs = [
    "vulkan_android_wrapper.cpp",
    "vulkan_android_wrapper.h",
  ]
)

cc_library(
  name = "vulkan",
  srcs = ["libpellengine_vulkan.so"],
  hdrs = [
    "vulkan_android_wrapper.h",
    "vulkan_wrapper.h",
  ]
)