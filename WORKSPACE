load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
  name = "com_grail_bazel_compdb",
  strip_prefix = "bazel-compilation-database-master",
  urls = ["https://github.com/grailbio/bazel-compilation-database/archive/master.tar.gz"],
)

git_repository(
  name = "rules_vulkan",
  remote = "https://github.com/PellEngine/rules_vulkan.git",
  tag = "v0.0.3",
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

new_local_repository(
  name = "stb",
  path = "src/stb/",
  build_file = "src/third_party/stb.BUILD"
)

load("@rules_vulkan//vulkan:repositories.bzl", "vulkan_repositories")
vulkan_repositories()