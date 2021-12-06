load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "ctti",
  hdrs = glob(["include/**"]),
  include_prefix = "ctti",
  strip_include_prefix = "include/ctti",
  includes = ["include"],
  copts = ["-Iinclude/"],
  visibility = ["//visibility:public"]
)