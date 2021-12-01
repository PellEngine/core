package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

srcs = [
  "asset_reader_android.cpp",
  "asset_reader.cpp",
  "io_locator.cpp",
  "logger.cpp",
  "safe_queue.ipp",
]

hdrs = [
  "asset_reader_android.h",
  "asset_reader.h",
  "io_locator.h",
  "logger.h",
  "safe_queue.h",
]

touch_srcs = [
  "touch/touch_system_android.cpp",
  "touch/touch_system.cpp",
  "touch/touchpad.cpp"
]

touch_hdrs = [
  "touch/touch_event.h",
  "touch/touch_system_android.h",
  "touch/touch_system.h",
  "touch/touchpad.h"
]

cc_binary(
  name = "libio.so",
  linkshared = True,
  srcs = srcs + hdrs + touch_srcs + touch_hdrs,
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
  hdrs = hdrs + touch_hdrs
)