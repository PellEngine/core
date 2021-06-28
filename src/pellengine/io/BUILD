package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "io",
  srcs = [
    "asset_reader.cpp",
    "logger.cpp",
  ],
  hdrs = [
    "asset_reader.h",
    "logger.h",
  ]
)

filegroup(
  name = "libio",
  srcs = [":io"],
  output_group = "dynamic_library",
)

genrule(
  name = "readelf_io",
  srcs = [":libio"],
  outs = ["readelf_io.txt"],
  cmd = "readelf -a $(SRCS) > $@",
)