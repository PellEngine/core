load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl", "tool_path", "feature", "flag_group", "flag_set")
load(":flags.bzl", "global_cflags", "global_ldflags", "global_includes")
load(":config.bzl", "os_name", "android_version", "ndk_root", "compilers_root", "clang_version", "clang_extension")

def _impl(ctx):
  tool_paths = [
    tool_path(
      name = "gcc",
      path = compilers_root + "aarch64-linux-android" + android_version + "-clang++" + clang_extension
    ),
    tool_path(
      name = "ld",
      path = compilers_root + "aarch64-linux-android" + "-ld",
    ),
    tool_path(
      name = "ar",
      path = compilers_root + "aarch64-linux-android" + "-ar",
    ),
    tool_path(
      name = "cpp",
      path = "/bin/false",
    ),
    tool_path(
      name = "gcov",
      path = "/bin/false",
    ),
    tool_path(
      name = "nm",
      path = "/bin/false",
    ),
    tool_path(
      name = "objdump",
      path = "/bin/false",
    ),
    tool_path(
      name = "strip",
      path = "/bin/false",
    ),
  ]

  cflags = [
    "-m64"
  ]

  toolchain_compiler_flags = feature(
    name = "compiler_flags",
    enabled = True,
    flag_sets = [
      flag_set(
        actions = [
          ACTION_NAMES.assemble,
          ACTION_NAMES.preprocess_assemble,
          ACTION_NAMES.linkstamp_compile,
          ACTION_NAMES.c_compile,
          ACTION_NAMES.cpp_compile,
          ACTION_NAMES.cpp_header_parsing,
          ACTION_NAMES.cpp_module_compile,
          ACTION_NAMES.cpp_module_codegen,
          ACTION_NAMES.lto_backend,
          ACTION_NAMES.clif_match
        ],
        flag_groups = [
          flag_group(
            flags = cflags + global_cflags,
          ),
        ],
      ),
    ],
  )
  
  toolchain_linker_flags = feature(
    name = "linker_flags",
    enabled = True,
    flag_sets = [
      flag_set(
        actions = [
          ACTION_NAMES.linkstamp_compile,
        ],
        flag_groups = [
          flag_group(
            flags = global_ldflags
          )
        ]
      ),
    ],
  )

  dynamic_library_feature = feature(
    name = "supports_dynamic_linker",
    enabled = True,
  )

  return cc_common.create_cc_toolchain_config_info(
    ctx = ctx,
    toolchain_identifier = "arm-toolchain",
    cxx_builtin_include_directories = global_includes,
    host_system_name = "local",
    target_system_name = "local",
    target_cpu = "arm",
    target_libc = "unknown",
    compiler = "clang",
    abi_version = "unkown",
    abi_libc_version = "unkown",
    tool_paths = tool_paths,
    builtin_sysroot = ndk_root + "/toolchains/llvm/prebuilt/" + os_name + "/sysroot/",
    features = [
      toolchain_compiler_flags,
      toolchain_linker_flags,
      dynamic_library_feature,
    ]
  )

arm_cc_toolchain_config = rule(
  implementation = _impl,
  attrs = {},
  provides = [CcToolchainConfigInfo],
)