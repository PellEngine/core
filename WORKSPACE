load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "com_grail_bazel_compdb",
  strip_prefix = "bazel-compilation-database-master",
  urls = ["https://github.com/grailbio/bazel-compilation-database/archive/master.tar.gz"],
)

new_local_repository(
  name = "ctti",
  path = "src/ctti/",
  build_file = "src/third_party/ctti.BUILD"
)

new_local_repository(
  name = "glm",
  path = "src/glm/",
  build_file = "src/third_party/glm.BUILD"
)