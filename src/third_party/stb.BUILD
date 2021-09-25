load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "stb",
  hdrs = [
    "stb_image.h",
  ],
  include_prefix = "stb",
  strip_include_prefix = "",
  includes = ["./"],
  copts = ["-I./", "-w"],
  visibility = ["//visibility:public"]
)