package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
  name = "libio.so",
  linkshared = True,
  srcs = [
    "asset_reader_android.cpp",
    "asset_reader_android.h",
    "asset_reader.cpp",
    "asset_reader.h",
    "io_locator.cpp",
    "io_locator.h",
    "logger.cpp",
    "logger.h",
  ],
  deps = [
    "//src/pellengine/graphics:graphics_fwd",
    "//src/pellengine/vulkan:vulkan",
    "@glm//:glm",
    "@ctti//:ctti",
    "@stb//:stb",
  ]
)

cc_library(
  name = "io",
  srcs = ["libio.so"],
  hdrs = [
    "asset_reader_android.h",
    "asset_reader.h",
    "io_locator.h",
    "logger.h",
  ]
)