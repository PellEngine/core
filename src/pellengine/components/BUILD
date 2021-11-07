package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "components",
  hdrs = [
    "sprite.h",
    "transform.h",
  ],
  deps = [
    "@glm//:glm",
    "//src/pellengine/graphics:graphics_fwd"
  ]
)