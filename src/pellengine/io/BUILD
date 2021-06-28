package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
  name = "libio.so",
  linkshared = True,
  srcs = [
    "asset_reader.cpp",
    "asset_reader.h",
    "logger.cpp",
    "logger.h",
  ]
)

cc_library(
  name = "io",
  srcs = ["libio.so"],
  hdrs = [
    "asset_reader.h",
    "logger.h",
  ]
)